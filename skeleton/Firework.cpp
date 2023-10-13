#include "Firework.h"

Firework::Firework() : Particle(FIREWORK, PxTransform(0.0f,0.0f,0.0f)) {}

void Firework::explode() {}

//list<Particle*> Firework::explode() {
//	
//}

void Firework::addGenerator(ParticleGenerator* p) {
	gen.push_back(p);
}

Particle* Firework::clone() const {
	return new Particle(FIREWORK, PxTransform(0.0f, 0.0f, 0.0f));
}