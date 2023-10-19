#pragma once
#include <list>
#include "Particle.h"
#include "ParticleGenerator.h"
#include "GaussianParticleGenerator.h"
#include "UniformParticleGenerator.h"
using namespace std;

class Firework : public Particle {
protected:
	ParticleType type;
	list<ParticleGenerator*> gen;
	
public:
	Firework(ParticleType Type, PxTransform Transform, Vector3 Dir = Vector3(0.0f, 1.0f, 0.0f), bool Active = true);

	list<Particle*> explode();
	void addGenerator(ParticleGenerator* p);
	void removeGenerators();
	virtual Firework* clone() const;
};