#pragma once
#include <list>
#include "Particle.h"
#include "ParticleGenerator.h"
using namespace std;

class Firework : public Particle {
protected:
	unsigned type;
	list<ParticleGenerator*> gen;

public:
	Firework();
	//list<Particle*> explode();
	void explode();
	void addGenerator(ParticleGenerator* p);
	virtual Particle* clone() const;
};

