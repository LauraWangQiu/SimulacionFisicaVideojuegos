#include "ParticleSystem.h"

ParticleSystem::ParticleSystem() {}
ParticleSystem::~ParticleSystem() {}

void ParticleSystem::addParticle(currentSimpleParticleType Type, PxTransform Transform, Vector3 Dir, float Time, PxReal Size, Vector4 Color) {
	listOfParticles.push_back(new Particle(Type, Transform, Dir, Time, Size, Color));
}

void ParticleSystem::addProjectile(currentShotType Type, PxTransform Transform, Vector3 Dir, float Time, PxReal Size, Vector4 Color) {
	listOfProjectiles.push_back(new Projectile(Type, Transform, Dir, Time, Size, Color));
}

void ParticleSystem::update(double t) {
	// Actualizo las particulas y elimino aquellas no vivas
	auto p = listOfParticles.begin();
	while (p != listOfParticles.end()) {
		// Si se termina su tiempo de vida, se elimina
		if (!(*p)->integrate(t)) {
			delete* p;
			p = listOfParticles.erase(p);
		} else ++p;
	}

	// Actualizo las particulas y elimino aquellas no vivas
	auto p2 = listOfProjectiles.begin();
	while (p2 != listOfProjectiles.end()) {
		// Si se termina su tiempo de vida, se elimina
		if (!(*p2)->integrate(t)) {
			delete* p2;
			p2 = listOfProjectiles.erase(p2);
		}
		else ++p2;
	}
}

ParticleGenerator* ParticleSystem::getParticleGenerator(string name) {
	auto it = listOfParticleGenerators.begin();
	while (it != listOfParticleGenerators.end()) {
		if ((*it)->getName() == name) return *it;
	}
}

void ParticleSystem::generateFireworkSystem() {
	listOfParticleGenerators.push_back(new GaussianParticleGenerator("GaussianParticleGenerator", Vector3(0.0f,0.0f,0.0f), Vector3(0.0f, -1.0f, 0.0f), 2, 100,
		new Particle(BASIC, PxTransform(0.0f, 0.0f, 0.0f)), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, -1.0f, 0.0f), 50));
}