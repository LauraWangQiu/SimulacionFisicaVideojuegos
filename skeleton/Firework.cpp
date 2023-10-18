#include "Firework.h"

Firework::Firework(ParticleType Type, PxTransform Transform, Vector3 Dir, float Time, PxReal Size, Vector4 Color) : type(Type), Particle(Type, Transform, Dir, Time, Size, Color) {}

list<Particle*> Firework::explode() {
	list<Particle*> list;

	while (!gen.empty()) {
		auto c = gen.front();
		if (c != nullptr) {
			auto l = c->generateParticles(this);
			for (auto it = l.begin(); it != l.end(); ++it)
				list.push_back(*it);
			gen.pop_front();
		}
	}

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
	return new Firework(type, transform, vel, time, size, color);
}