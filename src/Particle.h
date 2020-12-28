// Particle.h

#ifndef PARTICLE_H_
#define PARTICLE_H_

#include <SDL.h>

namespace rjm {

struct Particle {
	double m_x;
	double m_y;

private:
	double m_speed;
	double m_direction;

private:
	void init();

public:
	Particle();
	void update(int interval);

};


} /* namespace rjm */

#endif /* PARTICLE_H_ */