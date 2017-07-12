/*
 * Simple-SDL2-Audio
 *
 * Copyright 2016 Jake Besworth
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include "audio.h"

/*
 * Native WAVE format
 *
 * On some GNU/Linux you can identify a files properties using:
 *      mplayer -identify music.wav
 *
 * On some GNU/Linux to convert any music to this or another specified format use:
 *      ffmpeg -i in.mp3 -acodec pcm_s16le -ac 2 -ar 48000 out.wav
 */
/* SDL_AudioFormat of files, such as s16 little endian */
#define AUDIO_FORMAT AUDIO_S16LSB

/* Frequency of the file */
#define AUDIO_FREQUENCY 48000

/* 1 mono, 2 stereo, 4 quad, 6 (5.1) */
#define AUDIO_CHANNELS 2

/* Specifies a unit of audio data to be used at a time. Must be a power of 2 */
#define AUDIO_SAMPLES 4096

/* Flags OR'd together, which specify how SDL should behave when a device cannot offer a specific feature
 * If flag is set, SDL will change the format in the actual audio file structure (as opposed to gDevice->want)
 *
 * Note: If you're having issues with Emscripten / EMCC play around with these flags
 *
 * 0                                    Allow no changes
 * SDL_AUDIO_ALLOW_FREQUENCY_CHANGE     Allow frequency changes (e.g. AUDIO_FREQUENCY is 48k, but allow files to play at 44.1k
 * SDL_AUDIO_ALLOW_FORMAT_CHANGE        Allow Format change (e.g. AUDIO_FORMAT may be S32LSB, but allow wave files of S16LSB to play)
 * SDL_AUDIO_ALLOW_CHANNELS_CHANGE      Allow any number of channels (e.g. AUDIO_CHANNELS being 2, allow actual 1)
 * SDL_AUDIO_ALLOW_ANY_CHANGE           Allow all changes above
 */
#define SDL_AUDIO_ALLOW_CHANGES SDL_AUDIO_ALLOW_ANY_CHANGE
/*
 * Queue structure for all loaded sounds
 *
 */
typedef struct sound
{
    uint32_t length;
    uint32_t lengthTrue;
    uint8_t * bufferTrue;
    uint8_t * buffer;
    uint8_t loop;
    uint8_t fade;
    uint8_t volume;

    SDL_AudioSpec audio;

    struct sound * next;
} Sound;

/*
 * Definition for the game global sound device
 *
 */
typedef struct privateAudioDevice
{
    SDL_AudioDeviceID device;
    SDL_AudioSpec want;
    uint8_t audioEnabled;
} PrivateAudioDevice;

/*
 * Add a sound to the end of the queue
 *
 * @param root      Root of queue
 * @param new       New Sound to add
 *
 */
static void addSound(Sound * root, Sound * new);

/*
 * Frees as many chained Sounds as given
 *
 * @param sound     Chain of sounds to free
 *
 */
static void freeSound(Sound * sound);

/*
 * Create a Sound object
 *
 * @param filename      Filename for the WAVE file to load
 * @param loop          Loop 0, ends after playing, 1 refreshes
 * @param volume        Volume, read playSound()
 *
 * @return returns a new Sound or NULL on failure
 *
 */
static Sound * createSound(const char * filename, uint8_t loop, int volume);

/*
 * Audio callback function for OpenAudioDevice
 *
 * @param userdata      Points to linked list of sounds to play, first being a placeholder
 * @param stream        Stream to mix sound into
 * @param len           Length of sound to play
 *
 */
static inline void audioCallback(void * userdata, uint8_t * stream, int len);

static PrivateAudioDevice * gDevice;

void playSound(const char * filename, int volume)
{
    Sound * new;

    if(!gDevice->audioEnabled)
    {
        return;
    }

    new = createSound(filename, 0, volume);

    SDL_LockAudioDevice(gDevice->device);
    addSound((Sound *) (gDevice->want).userdata, new);

    SDL_UnlockAudioDevice(gDevice->device);
}

void playMusic(const char * filename, int volume)
{
    Sound * global;
    Sound * new;
    uint8_t music;

    if(!gDevice->audioEnabled)
    {
        return;
    }

    music = 0;

    /* Create new music sound with loop */
    new = createSound(filename, 1, volume);

    /* Lock callback function */
    SDL_LockAudioDevice(gDevice->device);
    global = ((Sound *) (gDevice->want).userdata)->next;

    /* Find any existing musics, 0, 1 or 2 */
    while(global != NULL)
    {
        /* Phase out any current music */
        if(global->loop == 1 && global->fade == 0)
        {
            if(music)
            {
                global->length = 0;
                global->volume = 0;
            }

            global->fade = 1;
        }
        /* Set flag to remove any queued up music in favour of new music */
        else if(global->loop == 1 && global->fade == 1)
        {
            music = 1;
        }

        global = global->next;
    }

    addSound((Sound *) (gDevice->want).userdata, new);

    SDL_UnlockAudioDevice(gDevice->device);
}

void initAudio(void)
{
    Sound * global;
    gDevice = calloc(1, sizeof(PrivateAudioDevice));

    if(gDevice == NULL)
    {
        fprintf(stderr, "[%s: %d]Fatal Error: Memory c-allocation error\n", __FILE__, __LINE__);
        return;
    }

    gDevice->audioEnabled = 0;

    if(!(SDL_WasInit(SDL_INIT_AUDIO) & SDL_INIT_AUDIO))
    {
        fprintf(stderr, "[%s: %d]Error: SDL_INIT_AUDIO not initialized\n", __FILE__, __LINE__);
        return;
    }

    SDL_memset(&(gDevice->want), 0, sizeof(gDevice->want));

    (gDevice->want).freq = AUDIO_FREQUENCY;
    (gDevice->want).format = AUDIO_FORMAT;
    (gDevice->want).channels = AUDIO_CHANNELS;
    (gDevice->want).samples = AUDIO_SAMPLES;
    (gDevice->want).callback = audioCallback;
    (gDevice->want).userdata = calloc(1, sizeof(Sound));

    global = (gDevice->want).userdata;

    if(global == NULL)
    {
        fprintf(stderr, "[%s: %d]Error: Memory allocation error\n", __FILE__, __LINE__);
        return;
    }

    global->buffer = NULL;
    global->next = NULL;

    /* want.userdata = new; */
    if((gDevice->device = SDL_OpenAudioDevice(NULL, 0, &(gDevice->want), NULL, SDL_AUDIO_ALLOW_CHANGES)) == 0)
    {
        fprintf(stderr, "[%s: %d]Warning: failed to open audio device: %s\n", __FILE__, __LINE__, SDL_GetError());
    }
    else
    {
        /* Set audio device enabled global flag */
        gDevice->audioEnabled = 1;

        /* Unpause active audio stream */
        unpauseAudio();
    }
}

void endAudio(void)
{
    if(gDevice->audioEnabled)
    {
        pauseAudio();

        freeSound((Sound *) (gDevice->want).userdata);

        /* Close down audio */
        SDL_CloseAudioDevice(gDevice->device);
    }

    free(gDevice);
}

void pauseAudio(void)
{
    if(gDevice->audioEnabled)
    {
        SDL_PauseAudioDevice(gDevice->device, 1);
    }
}

void unpauseAudio(void)
{
    if(gDevice->audioEnabled)
    {
        SDL_PauseAudioDevice(gDevice->device, 0);
    }
}

static Sound * createSound(const char * filename, uint8_t loop, int volume)
{
    Sound * new = calloc(1, sizeof(Sound));

    if(new == NULL)
    {
        fprintf(stderr, "[%s: %d]Error: Memory allocation error\n", __FILE__, __LINE__);
        return NULL;
    }

    new->next = NULL;
    new->loop = loop;
    new->fade = 0;
    new->volume = volume;

    if(SDL_LoadWAV(filename, &(new->audio), &(new->bufferTrue), &(new->lengthTrue)) == NULL)
    {
        fprintf(stderr, "[%s: %d]Warning: failed to open wave file: %s error: %s\n", __FILE__, __LINE__, filename, SDL_GetError());
        free(new);
        return NULL;
    }

    new->buffer = new->bufferTrue;
    new->length = new->lengthTrue;
    (new->audio).callback = NULL;
    (new->audio).userdata = NULL;

    return new;
}

static inline void audioCallback(void * userdata, uint8_t * stream, int len)
{
    Sound * sound = (Sound *) userdata;
    Sound * previous = sound;
    int tempLength;
    uint8_t music = 0;

    /* Silence the main buffer */
    SDL_memset(stream, 0, len);

    /* First one is place holder */
    sound = sound->next;

    while(sound != NULL)
    {
        if(sound->length > 0)
        {
            if(sound->fade == 1 && sound->loop == 1)
            {
                music = 1;
                sound->volume--;

                if(sound->volume == 0)
                {
                    sound->length = 0;
                }
            }

            if(music && sound->loop == 1 && sound->fade == 0)
            {
                tempLength = 0;
            }
            else
            {
                tempLength = ((uint32_t) len > sound->length) ? sound->length : (uint32_t) len;
            }

            SDL_MixAudioFormat(stream, sound->buffer, AUDIO_FORMAT, tempLength, sound->volume);

            sound->buffer += tempLength;
            sound->length -= tempLength;

            previous = sound;
            sound = sound->next;
        }
        else if(sound->loop == 1 && sound->fade == 0)
        {
            sound->buffer = sound->bufferTrue;
            sound->length = sound->lengthTrue;
        }
        else
        {
            previous->next = sound->next;
            SDL_FreeWAV(sound->bufferTrue);
            free(sound);

            sound = previous->next;
        }
    }
}

static void addSound(Sound * root, Sound * new)
{
    if(root == NULL)
    {
        return;
    }

    while(root->next != NULL)
    {
        root = root->next;
    }

    root->next = new;
}

static void freeSound(Sound * sound)
{
    Sound * temp;

    while(sound != NULL)
    {
        SDL_FreeWAV(sound->bufferTrue);

        temp = sound;
        sound = sound->next;

        free(temp);
    }
}
