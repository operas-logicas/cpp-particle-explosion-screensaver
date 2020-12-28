// Screen.h

#ifndef SCREEN_H_
#define SCREEN_H_

#include <SDL.h>

namespace rjm {

class Screen {
public:
	const static int SCREEN_WIDTH = 960;
	const static int SCREEN_HEIGHT = 600;

private:
	SDL_Window *m_window[2];
	SDL_Renderer *m_renderer[2];
	SDL_Texture *m_texture[2];
	Uint32 *m_buffer1;
	Uint32 *m_buffer2;

public:
	Screen();
	bool init();
	void clear();
	void update();
	void setPixel(int x, int y, Uint8 red, Uint8 green, Uint8 blue);
	void boxBlur();
	bool processEvents(int elapsed);
	void blank(int elapsed);
	void close();

};


} /* namespace rjm */

#endif /* SCREEN_H_ */