#pragma once
#include <list>
#include "RenderUtils.hpp"
#include "Particle.h"
#include "Projectile.h"
#include "GaussianParticleGenerator.h"
#include "UniformParticleGenerator.h"
using namespace std;
using namespace physx;

class ParticleSystem {
protected:
	list<Particle*> listOfParticles;
	list<Projectile*> listOfProjectiles;

	list<ParticleGenerator*> listOfParticleGenerators;

public:
	ParticleSystem();
	~ParticleSystem();

	void addParticle(currentSimpleParticleType Type, PxTransform Transform, Vector3 Dir = Vector3(0.0f, 0.0f, 1.0f), float Time = 5.0f, PxReal Size = 1.0f, Vector4 Color = Vector4(255.0, 255.0, 255.0, 255.0));
	void addProjectile(currentShotType Type, PxTransform Transform, Vector3 Dir = Vector3(0.0f, 0.0f, 1.0f), float Time = 5.0f, PxReal Size = 1.0f, Vector4 Color = Vector4(255.0, 255.0, 255.0, 255.0));

	void update(double t);

	ParticleGenerator *getParticleGenerator(string name);
	void generateFireworkSystem();
};

