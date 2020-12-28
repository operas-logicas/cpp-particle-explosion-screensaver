// Swarm.cpp

#include "Swarm.h"

namespace rjm {

Swarm::Swarm(): lastTime(0) {
	m_particles = new Particle[NPARTICLES];

}

Swarm::~Swarm() {
	delete [] m_particles;
}

void Swarm::update(int elapsed) {

	int interval = elapsed - lastTime;

	for(int i=0; i<NPARTICLES; i++) {
		m_particles[i].update(interval);
	}

	lastTime = elapsed;
}

} /* namespace rjm */