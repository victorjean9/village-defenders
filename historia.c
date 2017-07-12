int legendas(SDL_Surface* gScreen, int pessoa, char num[], int tempo, char legenda[]){ //FUNÇÃO PARA EXIBIR LEGENDAS!
	static int tempoCorrido = 0; //Tempo de duração
	TTF_Font* font = TTF_OpenFont("fonte/ARIAL.TTF", 30); //Atribuindo a fonte ARIAL tamanho 40 á variavel 'font'; // Preparando o tipo da fonte a ser usada
	SDL_Color corAmarela = { 0xff,0xd7,0x00}; // COR DA FONTE AMARELO
	SDL_Color corBranca = { 0xff,0xff,0xff}; // COR DA FONTE BRANCO
	SDL_Rect localNome = { 250, 500, 0, 0 }; //Posição do nome
	SDL_Rect localTexto = { 250, 550, 0, 0 }; //Posição do texto
	SDL_Surface* texto = TTF_RenderUTF8_Blended_Wrapped(font, legenda, corBranca, SCREEN_WIDTH - 350); //Legenda
	SDL_BlitSurface(texto, NULL, gScreen, &localTexto); //Exibe legenda na tela
	SDL_Surface* nome; //Variável para nome 
	SDL_Surface* foto; //Variável para foto
	
	SDL_Rect posicao; //Posição da foto
	posicao.x = 0; //Empurrar 0 pixels da esquerda da tela para a direita
    posicao.y = 450; //Empurrar 450 pixels do topo da tela para baixo
    posicao.w = 250; //Tamanho da foto do personagem
    posicao.h = 250; //Altura da foto do personagem
	
	switch(pessoa){ //Verificar qual pessoa que é desejada
		case 0: // 0 - Corsela
			foto = IMG_Load("img/backgrounds/corsela_cut.png"); //Carregando foto da Corsela
			nome = TTF_RenderUTF8_Blended(font, "Corsela", corAmarela); //Carregando nome da Corsela
			break;
		case 1: // 0 - Arkantos
			foto = IMG_Load("img/backgrounds/arkantos_cut.png"); //Carregando foto do Arkantos
			nome = TTF_RenderUTF8_Blended(font, "Arkantos", corAmarela); //Carregando nome do Arkantos
			break;
		case 2: // 0 - Ragnar
			foto = IMG_Load("img/backgrounds/ragnar_cut.png"); //Carregando foto do Ragnar
			nome = TTF_RenderUTF8_Blended(font, "Ragnar", corAmarela); //Carregando nome do Ragnar
			break;
		case 3: // 0 - Lucas
			foto = IMG_Load("img/backgrounds/lucas_cut.png"); //Carregando foto do Lucas
			nome = TTF_RenderUTF8_Blended(font, "Lucas", corAmarela); //Carregando nome do Lucas
			break;
		case 4: // 0 - NINGUEM
			foto = IMG_Load("img/backgrounds/lucas_cut.png"); //Precisa carregar alguma coisa pra nao dar erro
			nome = TTF_RenderUTF8_Blended(font, "", corAmarela); //Precisa carregar alguma coisa pra nao dar erro
			break;
	}
	if(pessoa != 4){ //SE PESSOA NÃO FOR NINGUÉM, exibir alguém!
		SDL_BlitSurface(foto, NULL, gScreen, &posicao); //Exiba a foto do personagem
		SDL_BlitSurface(nome, NULL, gScreen, &localNome); //Exiba o nome do personagem
	}
	
	if((tempoCorrido == 0) && (num != "0")) {  //Se o tempo não acabar e se o audio tiver som
		if(pessoa == 0){ //CORSELA
			char arquivo[] = "som/vozes/corsela/aman";
			strcat(arquivo, num);
			strcat(arquivo, ".wav");
			playSound( arquivo, SDL_MIX_MAXVOLUME);
		} else if(pessoa == 1){ //ARKANTOS
			char arquivo[] = "som/vozes/arkantos/arka";
			strcat(arquivo, num);
			strcat(arquivo, ".wav");
			playSound( arquivo, SDL_MIX_MAXVOLUME);
		} else if(pessoa == 2){ //RAGNAR
			char arquivo[] = "som/vozes/ragnar/ajax";
			strcat(arquivo, num);
			strcat(arquivo, ".wav");
			playSound( arquivo, SDL_MIX_MAXVOLUME);
		} else if(pessoa == 3) { //LUCAS
			char arquivo[] = "som/vozes/lucas/odys";
			strcat(arquivo, num);
			strcat(arquivo, ".wav");
			playSound( arquivo, SDL_MIX_MAXVOLUME);
		} else { //EFEITOS
			char arquivo[] = "som/vozes/efeitos/";
			strcat(arquivo, num);
			strcat(arquivo, ".wav");
			playSound( arquivo, SDL_MIX_MAXVOLUME);
		}
	}
	SDL_FreeSurface(foto); //Libera foto
	SDL_FreeSurface(nome); //Libera nome
	SDL_FreeSurface(texto); //Libera texto
	SDL_FreeSurface(gScreen); //Apague a gScreen
	if(tempoCorrido == tempo){ //Se o tempo de exibição chegar no tempo informado, retornar 1 para prosseguir com a legenda!
		tempoCorrido = 0;
		return 1;
	} else { // Se não, continuar exibindo legenda!
		tempoCorrido++;
		return 0;
	}
}

int historia(SDL_Event event, SDL_Surface* gScreen) {
	int gameRunning = 2;//Variável que enquanto for != 0 o jogo continua aberto, 1-TELA INCIAL, 2-HISTORIA, 3-JOGO, 4-FINAL
	static int inicioMusica = 0; //Variavel para evitar que a musica se repete a cada segundo 0 - Iniciar musica, 1 - Nao executar musica pois ela ja esta tocando
	static int inicioFala = 0; //Variavel para evitar que a musica se repete a cada segundo 0 - Iniciar musica, 1 - Nao executar musica pois ela ja esta tocando
	static float varia = 0; //Variável para fazer a animação de esmaecer o background
	static int bg = 0; //determinar qual background q será apresentado
	static int espera = 0; //Variável para congelar a tela por um determinado tempo
	static int tipo = 0; //determinar qual musica que será tocada
	static SDL_Surface* gBackground = NULL; //Surface que vai conter o background de fundo
	static SDL_Surface* gCasa = NULL; // Surface que vai conter o background da casa!
	
	if(inicioMusica == 0){ //Se nao ha musica, execute
		if (tipo == 0) //Musica inicial
			playMusic("som/musicas/historia.wav", SDL_MIX_MAXVOLUME); //Iniciando a música de fundo do jogo
		else if (tipo == 1) //Musica da casa
			playMusic("som/musicas/dialogos.wav", SDL_MIX_MAXVOLUME);
		else //MUsica de ataque
			playMusic("som/musicas/ataque.wav", SDL_MIX_MAXVOLUME/2);
		inicioMusica = 1; //Musica iniciada
	}
	
	if(SDL_PollEvent(&event)){ //Checar se há eventos
		const Uint8* currentKeyStates = SDL_GetKeyboardState( NULL ); //Verifica se houve evento de KEYDOWN = tecla pressionada
		if((event.type == SDL_QUIT) || (currentKeyStates[ SDL_SCANCODE_ESCAPE ])) //Se alguém apertou o X da janela...
			gameRunning = 0; //Fechar o jogo
		if(currentKeyStates[ SDL_SCANCODE_RETURN ]){  //Botão ENTER pressionado
			gameRunning = 3;  //MUDAR PARA O JOGO = 3
			inicioMusica = 0;
		}
	}
	
	SDL_Rect bg_position; //Posição do background!
	bg_position.x = 0;  //Empurrar 0 pixels da esquerda da tela para a direita
    bg_position.y = varia; //Empurrar um numero variavel de pixels do topo da tela para baixo para fazer animações
    bg_position.w = SCREEN_WIDTH; // background com o mesmo tamanho da tela
    bg_position.h = SCREEN_HEIGHT; // background com a mesma altura da tela
	
	if(bg == 0)
		gBackground = IMG_Load("img/backgrounds/fadeout.jpg");
	else if(bg == 1)
		gBackground = IMG_Load("img/backgrounds/fadein.jpg");
	else if(bg == 2)
		gBackground = IMG_Load("img/backgrounds/historia.jpg");
	else if(bg == 3)
		gBackground = IMG_Load("img/backgrounds/fadeOutHist.jpg");
	else if(bg == 4)
		gBackground = IMG_Load("img/backgrounds/fadeOutHist.jpg");
	SDL_BlitSurface(gBackground, NULL, gScreen, &bg_position); //Imprimir FUNDO
	
	if(bg == 0){
		if(varia > -7000)
			varia-=700;
		else if(varia == -7000){
			espera++;
			TTF_Font* font = TTF_OpenFont("fonte/ARIAL.TTF", 30); //Atribuindo a fonte ARIAL tamanho 40 á variavel 'font'; // Preparando o tipo da fonte a ser usada
			SDL_Color corBranca = { 0xff,0xff,0xff}; // COR DA FONTE BRANCO
			SDL_Rect local = { 350, 350, 0, 0 };
			SDL_Surface* pressEnter = TTF_RenderUTF8_Blended(font, "Pressione ENTER se quiser pular a história!", corBranca);
			SDL_BlitSurface(pressEnter, NULL, gScreen, &local); //Imprimir FONTE
			SDL_FreeSurface(pressEnter); //Apague o texto
		}
		if(espera == 40){
			varia = 0;
			bg++;
		}
	} else if (bg == 1){
		if(varia > -6300)
			varia-=700;
		else if(varia == -6300){
			varia=0;
			bg++;
		}
	} else if (bg == 2){
		if(varia > -4100)
			varia-=5.5;
		else{
			varia=0;
			bg++;
		}
	} else if (bg == 3){
		if(inicioFala == 0){ //Se nao ha musica, execute
			playSound("som/vozes/comeconasaldeias.wav", SDL_MIX_MAXVOLUME); //Som de nao seleção
			inicioFala = 1;
		}
		if(varia > -7000)
			varia-=700;
		else if(varia == -7000){
			bg++;
			tipo = 1;
			inicioMusica = 0;
		}
	} else {
		gCasa = IMG_Load("img/backgrounds/casa.jpg");
		SDL_BlitSurface(gCasa, NULL, gScreen, &bg_position); 
		static int fala = 0;
		switch(fala){
			case 0:
				fala += legendas(gScreen, 0, "098", 10, "Arkantos!");
				break;
			case 1:
				fala += legendas(gScreen, 0, "061", 10, "O que está fazendo aqui?");
				break;
			case 2:
				fala += legendas(gScreen, 1, "10", 10, "Estamos de volta!");
				break;
			case 3:
				fala += legendas(gScreen, 0, "086", 10, "Era o que estávamos esperando!");
				break;
			case 4:
				fala += legendas(gScreen, 2, "50", 10, "Obrigado meu amigo!");
				break;
			case 5:
				fala += legendas(gScreen, 2, "51", 15, "Devemos unir nossas forças com você no comando!");
				break;
			case 6:
				fala += legendas(gScreen, 0, "077", 25, "Sim! Tenho amigos aqui, vão nos ajudar!");
				break;
			case 7:
				fala += legendas(gScreen, 2, "135", 10, "O que faremos?");
				break;
			case 8:
				fala += legendas(gScreen, 0, "115", 20, "Precisamos defender essas muralhas o máximo possível!");
				break;
			case 9:
				fala += legendas(gScreen, 1, "0", 10, "...");
				break;
			case 10:
				fala += legendas(gScreen, 2, "56", 10, "O Que foi Arkantos?");
				break;
			case 11:
				fala += legendas(gScreen, 1, "91", 30, "Há mais áreas cercadas por aqui... E mais tripulantes a resgatar!");
				break;
			case 12:
				fala += legendas(gScreen, 2, "55", 10, "Isso pode ser bastante útil!");
				break;
			case 13:
				fala += legendas(gScreen, 1, "89", 20, "Há uma aldeia perto daqui. Há mais porcos lá.");
				break;
			case 14:
				fala += legendas(gScreen, 0, "087", 10, "Leva tempo...");
				break;
			case 15:
				fala += legendas(gScreen, 1, "111", 30, "Sim, velhos inimigos... Ainda estão por aqui!");
				break;
			case 16:
				fala += legendas(gScreen, 2, "70", 30, "Podemos atraí-los para fora de seus acampamentos e armar uma emboscada em campo aberto!");
				break;
			case 17:
				fala += legendas(gScreen, 1, "53", 10, "Isso mesmo!");
				break;
			case 18:
				fala += legendas(gScreen, 1, "70", 20, "Levaremos isso em conta! Obrigado!");
				break;
			case 19:
				fala += legendas(gScreen, 0, "071", 10, "Façam silêncio aí!");
				break;
			case 20:
				fala += legendas(gScreen, 0, "090", 10, "Estão nos observando!");
				break;
			case 21:
				fala += legendas(gScreen, 4, "porta", 30, "");
				break;
			case 22:
				fala += legendas(gScreen, 1, "210", 20, "Esse som parece familiar...");
				break;
			case 23:
				fala += legendas(gScreen, 3, "50", 20, "Não poderia deixá-los receber todas as glórias!");
				break;
			case 24:
				fala += legendas(gScreen, 1, "130", 20, "Bem! Alguém precisava vir ajudar a pôr um fim nisso!");
				break;
			case 25:
				fala += legendas(gScreen, 0, "079", 20, "Junte-se a nós, é hora de planejar o ataque!");
				break;
			case 26:
				fala += legendas(gScreen, 3, "70", 10, "Boa ideia!");
				break;
			case 27:
				fala += legendas(gScreen, 3, "58", 10, "Para onde Arkantos?");
				break;
			case 28:
				fala += legendas(gScreen, 1, "196", 10, "Onde???");
				break;
			case 29:
				fala += legendas(gScreen, 1, "73", 20, "Vasculhem a área, e descubram se há recursos disponiveis!");
				break;
			case 30:
				fala += legendas(gScreen, 1, "74", 20, "Parece haver bastante madeira por aqui!");
				break;
			case 31:
				fala += legendas(gScreen, 0, "078", 20, "Por hora estamos salvos, faremos nosso plano pela manhã.");
				break;
			case 32:
				tipo = 2;
				inicioMusica = 0;
				fala += legendas(gScreen, 4, "explosao", 10, "");
				break;
			case 33:
				fala += legendas(gScreen, 1, "210", 20, "Esse som parece familiar!");
				break;
			case 34:
				fala += legendas(gScreen, 2, "62", 20, "Arkantos! Base inimiga a frente!");
				break;
			case 35:
				fala += legendas(gScreen, 3, "86", 10, "Tarde demais...");
				break;
			case 36:
				fala += legendas(gScreen, 0, "135", 20, "Mais gigantes e trolls horríveis estão a caminho!");
				break;
			case 37:
				fala += legendas(gScreen, 1, "250", 10, "Calma! O que é isso?");
				break;
			case 38:
				fala += legendas(gScreen, 1, "277", 10, "Que criaturas são essas?");
				break;
			case 39:
				fala += legendas(gScreen, 2, "68", 10, "Eu não estou gostando disso...");
				break;
			case 40:
				fala += legendas(gScreen, 2, "156", 10, "Protejam os aldeões!");
				break;
			case 41:
				fala += legendas(gScreen, 2, "159", 10, "Aí vem eles!");
				break;
			case 42:
				fala += legendas(gScreen, 0, "134", 20, "Argh! Essas coisas horríveis atiram pedras!");
				break;
			case 43:
				fala += legendas(gScreen, 1, "306", 30, "Nossa base está sendo atacada, as extremidades da muralha não podem ser destruídas!");
				break;
			case 44:
				fala += legendas(gScreen, 2, "170", 20, "Então, o que estamos esperando?");
				break;
			case 45:
				fala += legendas(gScreen, 0, "095", 30, "Argh! Quando decidirem quem vai primeiro estarei esperando lá embaixo!");
				break;
			case 46:
				fala += legendas(gScreen, 1, "1231", 10, "Não! Preciso de você aqui!");
				break;
			case 47:
				fala += legendas(gScreen, 1, "1232", 20, "Não se preocupe, essa missão não será longa!");
				break;
			case 48:
				fala += legendas(gScreen, 1, "161", 10, "Vamos rápido! MOVAM-SE");
				break;
			case 49:
				gameRunning = 3;  //MUDAR PARA O JOGO = 3
				break;
		}
	}		
	SDL_FreeSurface(gCasa); //Apague imagem de fundo para economizar memoria
	SDL_FreeSurface(gBackground); //Apague imagem de fundo para economizar memoria
	SDL_FreeSurface(gScreen); //Apague a gScreen
	return gameRunning;
}