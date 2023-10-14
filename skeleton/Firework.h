#pragma once
#include <list>
#include "Particle.h"
#include "ParticleGenerator.h"
using namespace std;

class Firework : public Particle {
protected:
	//unsigned type;
	list<ParticleGenerator*> gen;
	int numDivisions;

public:
	Firework(PxTransform Transform, Vector3 Vel = Vector3(0.0f, 1.0f, 0.0f), float Time = 1.0f, PxReal Size = 1.0f, Vector4 Color = Vector4(255.0, 255.0, 255.0, 255.0));
	list<Particle*> explode();
	void addGenerator(ParticleGenerator* p);
	void removeGenerators();
	virtual Firework* clone() const;
};

