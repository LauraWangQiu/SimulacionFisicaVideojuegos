#pragma once
#include <list>
#include <time.h>
#include "RenderUtils.hpp"
#include "Particle.h"
#include "ParticleGenerator.h"
using namespace std;
using namespace physx;

class UniformParticleGenerator : public ParticleGenerator {
protected:
	Vector3 velWidth, posWidth;

public:
	UniformParticleGenerator(string Name, Vector3 MeanPos, Vector3 MeanVel, double GenerationProbability, int NumParticles, Particle* Model,
		Vector3 VelWidth, Vector3 PosWidth);
	~UniformParticleGenerator();

	list<Particle*> generateParticles();

	// Getters
	Vector3 getVelWidth() const { return velWidth; }
	Vector3 getPosWidth() const { return posWidth; }
	// Setters
	void setVelWidth(Vector3 VelWidth) { velWidth = VelWidth; }
	void setPosWidth(Vector3 PosWidth) { posWidth = PosWidth; }
};

