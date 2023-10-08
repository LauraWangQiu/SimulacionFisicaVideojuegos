#pragma once
#include <list>
#include "RenderUtils.hpp"
#include "Particle.h"
#include "ParticleGenerator.h"
using namespace std;
using namespace physx;

class GaussianParticleGenerator : public ParticleGenerator {
protected:
	Vector3 stdDevPos, stdDevVel;
	double stdDevTime;

	particlePalettes palettes;

public:
	GaussianParticleGenerator(string Name, Vector3 MeanPos, Vector3 MeanVel, double GenerationProbability, int NumParticles, Particle* Model, 
		Vector3 StdDevPos, Vector3 StdDevVel, double StdDevTime, bool Active = true);
	~GaussianParticleGenerator();

	virtual list<Particle*> generateParticles();

	// Getters
	Vector3 getStdDevPos() const { return stdDevPos; }
	Vector3 getStdDevVel() const { return stdDevVel; }
	double getStdDevTime() const { return stdDevTime; }
	// Setters
	void setStdDevPos(Vector3 StdDevPos) { stdDevPos = StdDevPos; }
	void setStdDevVel(Vector3 StdDevVel) { stdDevVel = StdDevVel; }
	void setStdDevTime(double StdDevTime) { stdDevTime = StdDevTime; }
};

