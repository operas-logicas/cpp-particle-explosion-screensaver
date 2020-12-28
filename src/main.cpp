
// main.cpp

#include <iostream>
#include <iomanip>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include "Particle.cpp"
#include "Screen.cpp"
#include "Swarm.cpp"

using namespace std;
using namespace rjm;

int main(int argc, char* argv[]) {

	const int TIMEOUT = 300000; // 5 min

	srand(time(NULL));

	Screen screen;

	if(screen.init() == false) {
		cout << "Error initializing SDL." << endl;
	}

	const int HALF_SCREEN_WIDTH = Screen::SCREEN_WIDTH/2;
	const int HALF_SCREEN_HEIGHT = Screen::SCREEN_HEIGHT/2;

	Swarm swarm;

	while(true) {
		// Update particles
		int elapsed = SDL_GetTicks();

		//screen.clear();
		swarm.update(elapsed);

		// Draw particles
		unsigned char red = (1 + sin(elapsed * 0.0001)) * 128;
		unsigned char green = (1 + sin(elapsed * 0.0002)) * 128;
		unsigned char blue = (1 + sin(elapsed * 0.0003)) * 128;

		const Particle * const p_particles = swarm.getParticles();

		for(int i=0; i<Swarm::NPARTICLES; i++) {
			Particle particle = p_particles[i];

			int x = (particle.m_x + 1) * HALF_SCREEN_WIDTH;
			int y = particle.m_y * HALF_SCREEN_WIDTH + HALF_SCREEN_HEIGHT;

			screen.setPixel(x, y, red, green, blue);
		}

		screen.boxBlur();

		// Draw the screen
		screen.update();

		// Check for messages/events
		if(elapsed > TIMEOUT) {
			screen.blank(elapsed);
			break;
		}

		if(screen.processEvents(elapsed) == false) {
			screen.close();
			break;
		}


	}


	return 0;
}