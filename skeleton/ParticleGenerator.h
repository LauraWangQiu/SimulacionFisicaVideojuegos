#pragma once
#include <string>
#include <list>
#include "RenderUtils.hpp"
#include "Particle.h"
using namespace std;
using namespace physx;

class ParticleGenerator {
protected:
	string name;
	Vector3 meanPos, meanVel;
	double generationProbability;
	int numParticles;
	Particle* model;

public:
	ParticleGenerator(string Name, Vector3 MeanPos, Vector3 MeanVel, double GenerationProbability, int NumParticles, Particle* Model);
	~ParticleGenerator();

	virtual list<Particle*> generateParticles() = 0;

	// Getters
	string getName() const { return name; }
	Vector3 getMeanPos() const { return meanPos; }
	Vector3 getMeanVel() const { return meanVel; }
	double getGenerationProbability() const { return generationProbability; }
	int getNumParticles() const { return numParticles; }
	Particle* getModel() const { return model; }
	// Setters
	void setName(string Name) { name = Name; }
	void setMeanPos(Vector3 MeanPos) { meanPos = MeanPos; }
	void setMeanVel(Vector3 MeanVel) { meanVel = MeanVel; }
	void setGenerationProbability(double Probability) { generationProbability = Probability; }
	void setNumParticles(int NumParticles) { numParticles = NumParticles; }
	void setParticle(Particle* Model) { model = Model; }
};

