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

/*
 * audio.h
 *
 * All audio related functions go here
 *
 */
#ifndef SIMPLE_AUDIO_
#define SIMPLE_AUDIO_

#ifdef __cplusplus
extern "C"
{
#endif

/*
 * Play a wave file currently must be S16LE format 2 channel stereo
 *
 * @param filename      Filename to open, use getAbsolutePath
 * @param volume        Volume 0 - 128. SDL_MIX_MAXVOLUME constant for max volume
 *
 */
void playSound(const char * filename, int volume);

/*
 * Plays a new music, only 1 at a time plays
 *
 * @param filename      Filename of the WAVE file to load
 * @param volume        Volume read playSound for moree
 *
 */
void playMusic(const char * filename, int volume);

/*
 * Free all audio related variables
 * Note, this needs to be run even if initAudio fails, because it frees the global audio device
 *
 */
void endAudio(void);

/*
 * Initialize Audio Variable
 *
 */
void initAudio(void);

/*
 * Pause audio from playing
 *
 */
void pauseAudio(void);

/*
 * Unpause audio from playing
 *
 */
void unpauseAudio(void);

#ifdef __cplusplus
}
#endif

#endif
