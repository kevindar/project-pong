#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include <stdio.h>

#define WIDTH		640
#define HEIGHT		480
#define FIRST		300
#define SECOND		350
#define TITLE_SIZE	300, 100
#define MENU_BOX	180, 180

#define WHITE		255, 255, 255, 255
#define RED			255, 50, 50, 255
#define GREEN		50, 255, 50, 255
#define BLUE		50, 50, 255, 255


typedef struct {
	int x, y;
	int select;

	SDL_Renderer* renderer;

	SDL_Surface* background;
	SDL_Texture* backgroundTex;

	SDL_Surface* image;
	SDL_Texture* texture;

	SDL_Surface* text;
	SDL_Texture* titleTex;

	SDL_Surface* player;
	SDL_Texture* playerTex;

	SDL_Surface* difficulty;
	SDL_Texture* difficultyTex;
}Screen;


int menu(SDL_Window* window, SDL_Renderer* renderer, Screen* gameScreen) {
	int done = 0;
	int nplayer = 0;

	while(!done)
	{
		SDL_Event event;
		
		SDL_RenderClear(renderer);

		SDL_SetRenderDrawColor(renderer, WHITE);

		SDL_Rect bg = { 0, 0, WIDTH, HEIGHT };
		SDL_Rect rect = { gameScreen->x, gameScreen->y, 25, 25 };
		SDL_Rect titleBox = { (WIDTH / 4) + 10, HEIGHT / 5, TITLE_SIZE };
		SDL_Rect menuBox = { 230, 230, MENU_BOX };

		SDL_RenderCopy(renderer, gameScreen->backgroundTex, NULL, &bg);
		SDL_RenderCopy(renderer, gameScreen->titleTex, NULL, &titleBox);
		SDL_RenderCopy(renderer, gameScreen->texture, NULL, &rect);
		SDL_RenderCopy(renderer, gameScreen->playerTex, NULL, &menuBox);
		SDL_RenderPresent(renderer);

		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_WINDOWEVENT_CLOSE:
			{
				if (window)
				{
					SDL_DestroyWindow(window);
					window = NULL;
					done = 1;
				}
				break;
			case SDL_KEYDOWN:
			{
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					done = 1;
					break;
				case SDLK_UP:
					nplayer = 1;
					gameScreen->y = FIRST;
					break;
				case SDLK_DOWN:
					nplayer = 2;
					gameScreen->y = SECOND;
					break;
				case SDLK_RETURN:
					done = 1;
					break;
				}
			}
			break;
			}
			case SDL_QUIT:
				done = 1;
				break;
			}
		}
		SDL_Delay(10);
	}
	return nplayer;
}


int menuScreen() {
	Screen gameScreen;
	gameScreen.x = 200;
	gameScreen.y = 300;
	int done = 0;
	int nplayer = 0;
	
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

	Mix_Music* Music;
	Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);
	Music = Mix_LoadMUS("Undertale - Megalovania.wav");
	if (!Music) {
		printf("Mix_LoadMUS(\"music.mp3\"): %s\n", Mix_GetError());
	}
	Mix_PlayMusic(Music, -1);

	SDL_Window* window = SDL_CreateWindow(" SDL2 ",SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, 0);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

	gameScreen.background = IMG_Load("bg.jpg");
	gameScreen.backgroundTex = SDL_CreateTextureFromSurface(renderer, gameScreen.background);

	gameScreen.image = IMG_Load("point.png");
	gameScreen.texture = SDL_CreateTextureFromSurface(renderer, gameScreen.image);

	gameScreen.text = IMG_Load("title.png");
	gameScreen.titleTex = SDL_CreateTextureFromSurface(renderer, gameScreen.text);

	gameScreen.player = IMG_Load("mode.png");
	gameScreen.playerTex = SDL_CreateTextureFromSurface(renderer, gameScreen.player);

	menu(window, renderer, &gameScreen);
	nplayer = menu(window, renderer, &gameScreen);

	Mix_FreeMusic(Music);
	Music = NULL;

	SDL_DestroyWindow(window);
	SDL_DestroyTexture(gameScreen.titleTex);
	SDL_DestroyRenderer(renderer);
	Mix_Quit();
	SDL_Quit();
	
	return nplayer;
}