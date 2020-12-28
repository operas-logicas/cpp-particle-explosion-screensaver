// Screen.cpp

#include "Screen.h"

namespace rjm {

Screen::Screen() {
	m_window[0] = NULL;
	m_window[1] = NULL;
	m_renderer[0] = NULL;
	m_renderer[1] = NULL;
	m_texture[0] = NULL;
	m_texture[1] = NULL;
	m_buffer1 = NULL;
	m_buffer2 = NULL;

}


bool Screen::init() {

	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		return false;
	}

	m_window[0] = SDL_CreateWindow("Particle Fire Explosion", SDL_WINDOWPOS_UNDEFINED_DISPLAY(0), SDL_WINDOWPOS_UNDEFINED_DISPLAY(0),
		WINDOW_WIDTH_1, WINDOW_HEIGHT_1, SDL_WINDOW_FULLSCREEN | SDL_WINDOW_BORDERLESS | SDL_WINDOW_ALWAYS_ON_TOP);

	m_window[1] = SDL_CreateWindow("Particle Fire Explosion", SDL_WINDOWPOS_UNDEFINED_DISPLAY(1), SDL_WINDOWPOS_UNDEFINED_DISPLAY(1),
		WINDOW_WIDTH_2, WINDOW_HEIGHT_2, SDL_WINDOW_FULLSCREEN | SDL_WINDOW_BORDERLESS | SDL_WINDOW_ALWAYS_ON_TOP);

	if(m_window[0] == NULL) {
		SDL_Quit();
		return false;
	}

	m_renderer[0] = SDL_CreateRenderer(m_window[0], -1, SDL_RENDERER_PRESENTVSYNC);
	if(m_renderer[0] == NULL) {
		SDL_DestroyWindow(m_window[0]);
		SDL_Quit();
		return false;
	}

	m_renderer[1] = SDL_CreateRenderer(m_window[1], -1, SDL_RENDERER_PRESENTVSYNC);
	if(m_renderer[1] == NULL) {
		SDL_DestroyWindow(m_window[1]);
	}

	m_texture[0] = SDL_CreateTexture(m_renderer[0], SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, SCREEN_WIDTH, SCREEN_HEIGHT);
	if(m_texture[0] == NULL) {
		SDL_DestroyRenderer(m_renderer[0]);
		SDL_DestroyWindow(m_window[0]);
		SDL_Quit();
		return false;
	}

	m_texture[1] = SDL_CreateTexture(m_renderer[1], SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, SCREEN_WIDTH, SCREEN_HEIGHT);
	if(m_texture[1] == NULL) {
		SDL_DestroyRenderer(m_renderer[1]);
		SDL_DestroyWindow(m_window[1]);
	}	

	m_buffer1 = new Uint32[SCREEN_WIDTH*SCREEN_HEIGHT];
	m_buffer2 = new Uint32[SCREEN_WIDTH*SCREEN_HEIGHT];

	memset(m_buffer1, 0, SCREEN_WIDTH*SCREEN_HEIGHT*sizeof(Uint32));
	memset(m_buffer2, 0, SCREEN_WIDTH*SCREEN_HEIGHT*sizeof(Uint32));	

	return true;

}


void Screen::clear() {

	memset(m_buffer1, 0, SCREEN_WIDTH*SCREEN_HEIGHT*sizeof(Uint32));
	memset(m_buffer2, 0, SCREEN_WIDTH*SCREEN_HEIGHT*sizeof(Uint32));

}


void Screen::update() {

	SDL_UpdateTexture(m_texture[0], NULL, m_buffer1, SCREEN_WIDTH*sizeof(Uint32));	
	SDL_RenderClear(m_renderer[0]);	
	SDL_RenderCopy(m_renderer[0], m_texture[0], NULL, NULL);	
	SDL_RenderPresent(m_renderer[0]);
	
	if(m_window[1] != NULL) {
		SDL_UpdateTexture(m_texture[1], NULL, m_buffer1, SCREEN_WIDTH*sizeof(Uint32));
		SDL_RenderClear(m_renderer[1]);
		SDL_RenderCopy(m_renderer[1], m_texture[1], NULL, NULL);
		SDL_RenderPresent(m_renderer[1]);
	}

}


void Screen::setPixel(int x, int y, Uint8 red, Uint8 green, Uint8 blue) {

	if(x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT) return;

	Uint32 color = 0;

	color += red;
	color <<= 8;
	color += green;
	color <<= 8;
	color += blue;
	color <<=8;
	color += 0xFF;

	m_buffer1[(y * SCREEN_WIDTH) + x] = color;

}


void Screen::boxBlur() {
	
	Uint32 *temp = m_buffer1;
	m_buffer1 = m_buffer2;
	m_buffer2 = temp;

	for(int y=0; y<SCREEN_HEIGHT; y++) {
		for(int x=0; x<SCREEN_WIDTH; x++) {

			/*
			 * 0 0 0 
			 * 0 1 0
			 * 0 0 0
			 */

			int redTotal = 0;
			int greenTotal = 0;
			int blueTotal = 0;

			for(int row=-1; row<=1; row++) {
				for(int col=-1; col<=1; col++) {
					int currentX = x + col;
					int currentY = y + row;

					if(currentX > 0 && currentX < SCREEN_WIDTH && currentY > 0 && currentY < SCREEN_HEIGHT) {
						Uint32 color = m_buffer2[(currentY * SCREEN_WIDTH) + currentX];
						Uint8 red = color >> 24;
						Uint8 green = color >> 16;
						Uint8 blue = color >> 8;

						redTotal += red;
						greenTotal += green;
						blueTotal += blue;

					}

				}
			}

			Uint8 red = redTotal/9;
			Uint8 green = greenTotal/9;
			Uint8 blue = blueTotal/9;

			setPixel(x, y, red, green, blue);
		}

	}

}


bool Screen::processEvents(int elapsed) {

	SDL_Event event;

	while(SDL_PollEvent(&event)) {
		if(event.type == SDL_QUIT || (event.type == SDL_MOUSEMOTION && elapsed > 1000 ) || (event.type == SDL_KEYDOWN && elapsed > 1000)) {
			return false;
		}

	}

	return true;

}

void Screen::blank(int elapsed) {

	clear();
	update();

	while(true) {
		if(processEvents(elapsed) == false) {
			break;
		}
	}

	close();

}


void Screen::close() {

	delete [] m_buffer1;
	delete [] m_buffer2;	

	SDL_DestroyTexture(m_texture[0]);
	SDL_DestroyRenderer(m_renderer[0]);	
	SDL_DestroyWindow(m_window[0]);	

	if(m_window[1] != NULL) {
		SDL_DestroyTexture(m_texture[1]);
		SDL_DestroyRenderer(m_renderer[1]);
		SDL_DestroyWindow(m_window[1]);
	}

	SDL_Quit();

}


} /* namespace rjm */