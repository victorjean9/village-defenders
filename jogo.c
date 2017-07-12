#include <stdio.h>
#include <stdlib.h>
#include <string.h> //Funções de texto
#include <SDL2/SDL.h> //Funções do SDL
#include <SDL2/SDL_image.h> //Funções de imagem do SDL
#include <SDL2/SDL_ttf.h> //Funções de FONTE do SDL do SDL
#include <SDL2/SDL_mixer.h>  //Funções de audio do SDL
#include "audio.h" //Biblioteca do audio
#include "audio.c" //Funções de audio
#include "variaveis.c" //Função da tela inicial
#include "telaInicial.c" //Função da tela inicial
#include "historia.c" //Função da tela de historia
#include "game.c" //Função da tela de historia

int gameRunning = 1; //Variável que enquanto for != 0 o jogo continua aberto, 1-TELA INCIAL, 2-HISTORIA, 3-JOGO, 4-FINAL

int main( int argc, char* args[] ){
	SDL_Window* window = NULL; //Variavel que vai representar a janela(window)
    
	SDL_Surface* gScreen = NULL; //Surface que vai conter nossa janela(window)
    
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO); //Inicializando o video e o audio do SDL
	TTF_Init(); //Inicializando a fonte
	initAudio(); //Inicializando o audio
	
	window = SDL_CreateWindow( "Village Defenders", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN ); //Criando a Janela
	
	gScreen = SDL_GetWindowSurface( window ); //Pegando a surface da janela
    
	SDL_Event event; //Criando o evento
	
	while(gameRunning != 0){ // Enquanto o jogo estiver rodando faça:
		if(gameRunning == 1) // 1 - Exibir tela inicial
			gameRunning = telaInicial(event, gScreen); //Exibe os botoes na tela
		if(gameRunning == 2) // 2 - Exibir Historia
			gameRunning = historia(event, gScreen); //Exibe a historia na tela
		if(gameRunning == 3) // 2 - Exibir Gameplay!
			gameRunning = gameplay(event, gScreen); //Exibe o Gameplay na tela
		SDL_UpdateWindowSurface( window ); //Atualizar a surface

		SDL_Delay(1);//Espere um segundo antes de atualizar a tela
	}
	endAudio(); // Desligue o audio
	SDL_FreeSurface(gScreen); //Apague a gScreen
	SDL_DestroyWindow( window );//Destruir janela

	SDL_Quit();//Encerrar o SDL

	return 0; //Encerrar o Programa
}
