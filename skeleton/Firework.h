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
	Firework(ParticleType Type, PxTransform Transform, Vector3 Dir = Vector3(0.0f, 1.0f, 0.0f), float Time = 1.0f, PxReal Size = 1.0f, Vector4 Color = Vector4(1.0f, 1.0f, 1.0f, 1.0f));

	list<Particle*> explode();
	void addGenerator(ParticleGenerator* p);
	void removeGenerators();
	virtual Firework* clone() const;
};