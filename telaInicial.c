int telaInicial(SDL_Event event, SDL_Surface* gScreen){
	static int posicao = 0; //Posição da seta sobre os botões
	static int inicioMusica = 0; //Variavel para evitar que a musica se repete a cada segundo 0 - Iniciar musica, 1 - Nao executar musica pois ela ja esta tocando
	static SDL_Surface* gBackground = NULL; //Surface que vai conter o background de fundo
	TTF_Font* font = TTF_OpenFont("fonte/ARIAL.TTF", 40); //Atribuindo a fonte ARIAL tamanho 40 á variavel 'font'; // Preparando o tipo da fonte a ser usada
	SDL_Surface* gSeta = IMG_Load("img/sprites/goldenArrow.png"); //Seta amarela
	SDL_Surface* iniciarJogoBTN; // Preparando a fonte para ser apresentada
	SDL_Surface* instrucoesBTN; // Preparando a fonte para ser apresentada
	SDL_Surface* grupoBTN; // Preparando a fonte para ser apresentada
	SDL_Surface* fecharBTN; // Preparando a fonte para ser apresentada
	
	if(inicioMusica == 0){ //Se nao ha musica, execute
		playMusic("som/musicas/inicial.wav", SDL_MIX_MAXVOLUME); //Iniciando a música de fundo do jogo
		inicioMusica = 1; //Musica iniciada
	}
	
	int gameRunning = 1;//Variável que enquanto for != 0 o jogo continua aberto, 1-TELA INCIAL, 2-HISTORIA, 3-JOGO, 4-FINAL
	
	if(SDL_PollEvent(&event)){ //Checar se há eventos
		const Uint8* currentKeyStates = SDL_GetKeyboardState( NULL ); //Verifica se houve evento de KEYDOWN = tecla pressionada
		if((event.type == SDL_QUIT) || (currentKeyStates[ SDL_SCANCODE_ESCAPE ])) //Se alguém apertou o X da janela...
			gameRunning = 0; //Fechar o jogo
		 if(currentKeyStates[ SDL_SCANCODE_UP ]){ //Seta pra cima pressionada
			 if(posicao !=0){ //Se ela já não estiver na primeira posição...
				 posicao--; //Suba de posicao
				 playSound("som/interfaces/select.wav", SDL_MIX_MAXVOLUME); //Som de seleção
			 } else { //POSIÇÃO ACIMA NAO EXISTE
				  playSound("som/interfaces/noselect.wav", SDL_MIX_MAXVOLUME); //Som de nao seleção
			 }
		 } else if(currentKeyStates[ SDL_SCANCODE_DOWN ]){ //Seta pra baixo pressionada
			 if(posicao !=3){ //Se ela já não estiver na ultima posição...
				 posicao++;//Desça de posicao
				 playSound("som/interfaces/select.wav", SDL_MIX_MAXVOLUME); //Som de seleção
			 } else { //POSIÇÃO ABAIXO NAO EXISTE
				  playSound("som/interfaces/noselect.wav", SDL_MIX_MAXVOLUME); //Som de nao seleção
			 }
		 }  else if(currentKeyStates[ SDL_SCANCODE_RETURN ]){  //Botão ENTER pressionado
			 if(posicao == 0){ //Primeira posição (INICIAR O JOGO)
				 playSound("som/interfaces/ok.wav", SDL_MIX_MAXVOLUME); //DUBLAGEM: Devemos unir nossas forças com você no comando!
				 inicioMusica = 0; //Musica de fundo não está tocando mais, quando voltar pra essa tela, tocar de // Muda a tela do jogo para a tela da HISTÓRIA!novo!
				 gameRunning = 2;// Muda a tela do jogo para a tela da HISTÓRIA!
			 }else if (posicao == 3) //Utlima posição (SAIR DO JOGO)
				 gameRunning = 0; //Fechar o jogo
			 else //Não da pra dar ENTER nessa posicao
				 playSound("som/interfaces/noselect.wav", SDL_MIX_MAXVOLUME); //Som de nao seleção
		 }
	}
	
	SDL_Color corAmarela = { 0xff,0xd7,0x00}; // COR DA FONTE AMARELO
	SDL_Color corBranca = { 0xff,0xff,0xff}; // COR DA FONTE BRANCO
	
	SDL_Rect localizacaoPrimeiroBtn = { 100, 400, 0, 0 }; //Preparando o local onde a fonte será colocada
	SDL_Rect localizacaoSegundoBtn = { 100, 460, 0, 0 }; //Preparando o local onde a fonte será colocada
	SDL_Rect localizacaoTerceiroBtn = { 100, 520, 0, 0 }; //Preparando o local onde a fonte será colocada
	SDL_Rect localizacaoQuartoBtn = { 100, 580, 0, 0 }; //Preparando o local onde a fonte será colocada
	
	SDL_Rect seta_position;//posicão da seta
	seta_position.x = 50; //Empurrar 50 pixels da esquerda da tela para a direita
    seta_position.y = 397; //Empurrar 397 pixels do topo da tela para baixo
    seta_position.w = 50; //TAmanho da seta
    seta_position.h = 50; //Altura da seta
	
	iniciarJogoBTN = TTF_RenderUTF8_Blended(font, "Iniciar Jogo", corBranca); //Indicar as propriedades do botao de iniciar (fonte, texto, cor);
	instrucoesBTN = TTF_RenderUTF8_Blended(font, "Instruções", corBranca); //Indicar as propriedades do botao instrucoes (fonte, texto, cor);
	grupoBTN = TTF_RenderUTF8_Blended(font, "Grupo", corBranca); //Indicar as propriedades do botao grupo (fonte, texto, cor);
	fecharBTN = TTF_RenderUTF8_Blended(font, "Sair do Jogo", corBranca); //Indicar as propriedades do botao fechar (fonte, texto, cor);
	
	if(posicao == 0){ //Caso esteja na primeira posição
		iniciarJogoBTN = TTF_RenderUTF8_Blended(font, "Iniciar Jogo", corAmarela); //Mudar a cor do botao para amarelo
		gBackground = IMG_Load("img/backgrounds/tela.png"); //Declaração do Background do jogo
	}
	else if (posicao == 1){ //Caso esteja na segunda posição
		seta_position.y = 457; //Abaixar a seta
		instrucoesBTN = TTF_RenderUTF8_Blended(font, "Instruções", corAmarela); //Mudar a cor do botao para amarelo
		gBackground = IMG_Load("img/backgrounds/instrucoes.png"); //Declaração do Background do jogo
	} else if (posicao == 2){ //Caso esteja na terceira posição
		seta_position.y = 517; //Abaixar a seta
		grupoBTN = TTF_RenderUTF8_Blended(font, "Grupo", corAmarela); //Mudar a cor do botao para amarelo
		gBackground = IMG_Load("img/backgrounds/grupo.png"); //Declaração do Background do jogo
	} else { //Caso esteja na quarta posição
		seta_position.y = 577; //Abaixar a seta
		fecharBTN = TTF_RenderUTF8_Blended(font, "Sair do Jogo", corAmarela); //Mudar a cor do botao para amarelo
		gBackground = IMG_Load("img/backgrounds/tela.png"); //Declaração do Background do jogo
	}
	SDL_BlitSurface(gBackground, NULL, gScreen, NULL); //Imprimir FUNDO
	SDL_BlitSurface(iniciarJogoBTN, NULL, gScreen, &localizacaoPrimeiroBtn); //Imprimir FONTE
	SDL_BlitSurface(instrucoesBTN, NULL, gScreen, &localizacaoSegundoBtn); //Imprimir FONTE
	SDL_BlitSurface(grupoBTN, NULL, gScreen, &localizacaoTerceiroBtn); //Imprimir FONTE
	SDL_BlitSurface(fecharBTN, NULL, gScreen, &localizacaoQuartoBtn); //Imprimir FONTE
	SDL_BlitSurface(gSeta, NULL, gScreen, &seta_position); //Imprimir SETA
	SDL_FreeSurface(gBackground); //Apague imagem de fundo para economizar memoria
	SDL_FreeSurface(gSeta); //Apague seta amarela
	SDL_FreeSurface(iniciarJogoBTN); //Apague o texto
	SDL_FreeSurface(instrucoesBTN); //Apague o texto
	SDL_FreeSurface(grupoBTN); //Apague o texto
	SDL_FreeSurface(fecharBTN); //Apague o texto
	
	TTF_CloseFont(font); //Desligue a fonte
	return gameRunning; //Retorna o estado do jogo
}