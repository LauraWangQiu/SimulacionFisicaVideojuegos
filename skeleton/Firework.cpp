#include "Firework.h"

Firework::Firework(PxTransform Transform, Vector3 Vel, float Time, PxReal Size, Vector4 Color) : numDivisions(0), Particle(FIREWORK, Transform, Vel, Time, Size, Color) {}

list<Particle*> Firework::explode() {
	list<Particle*> list;

	/* Generar lista de particulas de forma radial */

	return list;
}

void Firework::addGenerator(ParticleGenerator* p) {
	gen.push_back(p);
}

void Firework::removeGenerators() {
	while (!gen.empty())
		gen.pop_front();
}

Firework* Firework::clone() const {
	return new Firework(transform, vel, time, size, color);
}