#include "Firework.h"

Firework::Firework(ParticleType Type, PxTransform Transform, Vector3 Dir, bool Active) : 
	type(Type), Particle(Type, Transform, Dir, Active) {}

Firework::Firework(PxPhysics* GPhysics, PxScene* GScene, ParticleType Type, PxTransform Transform, Vector3 Dir, bool Active) : 
	type(Type), Particle(GPhysics, GScene, Type, Transform, Dir, Active) {}


Firework::~Firework() {
	while (!gen.empty()) {
		gen.pop_front();
	}
}

list<Particle*> Firework::explode(int numParticles) {
	list<Particle*> list;

	while (!gen.empty()) {
		auto c = gen.front();
		if (c != nullptr) {
			c->updateActualNumParticles(numParticles);
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

ParticleGenerator* Firework::getFirstGenerator() const {
	if (!gen.empty()) return gen.front();
	else return nullptr;
}

Firework* Firework::clone() const {
	if (gPhysics == nullptr || gScene == nullptr) return new Firework(type, transform, dir);
	else return new Firework(gPhysics, gScene, type, transform, dir);
}