#include "Firework.h"

Firework::Firework(ParticleType Type, PxTransform Transform, Vector3 Dir, bool Active) : type(Type), Particle(Type, Transform, Dir, Active) {}

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

void Firework::deleteGenerators() {
	while (!gen.empty()) {
		delete gen.front();
		gen.pop_front();
	}
}

ParticleGenerator* Firework::getFirstGenerator() const {
	return gen.front();
}

Firework* Firework::clone() const {
	return new Firework(type, transform, dir);
}