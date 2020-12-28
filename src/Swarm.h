// Swarm.h

#ifndef SWARM_H_
#define SWARM_H_

#include <SDL.h>
#include "Particle.h"

namespace rjm {

class Swarm {
public:
	const static int NPARTICLES = 3000;

private:
	Particle *m_particles;
	int lastTime;

public:
	Swarm();
	~Swarm();
	const Particle * const getParticles() { return m_particles; };
	void update(int elapsed);

};


} /* namespace rjm */

#endif /* SWARM_H_ */