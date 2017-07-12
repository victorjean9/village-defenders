int gameplay(SDL_Event event, SDL_Surface* gScreen){
	SDL_Surface* gBackground = IMG_Load("img/backgrounds/game.png");
	SDL_BlitSurface(gBackground, NULL, gScreen, NULL); //Imprimir FUNDO
	int gameRunning = 3;  //MUDAR PARA O JOGO = 3
	
	return gameRunning;
}