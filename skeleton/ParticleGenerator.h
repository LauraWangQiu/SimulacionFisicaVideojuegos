#pragma once
#include <string>
#include <list>
#include <random>
#include <math.h>
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
	bool active;

	mt19937 rd;
	default_random_engine generator;
	uniform_real_distribution<double> u{ 0.0, 1.0};
	normal_distribution<double> n{ 0.0, 1.0 };

	particlePalettes palettes;
public:
	ParticleGenerator(string Name, Vector3 MeanPos, Vector3 MeanVel, double GenerationProbability, int NumParticles, Particle* Model, bool Active = true);
	~ParticleGenerator();

	virtual list<Particle*> generateParticles() = 0;
	virtual list<Particle*> generateParticles(Particle* deadP) = 0;

	// Getters
	inline string getName() const { return name; }
	inline Vector3 getMeanPos() const { return meanPos; }
	inline Vector3 getMeanVel() const { return meanVel; }
	inline double getGenerationProbability() const { return generationProbability; }
	inline int getNumParticles() const { return numParticles; }
	inline Particle* getModel() const { return model; }
	inline bool getActive() const { return active; }
	// Setters
	inline void setName(string Name) { name = Name; }
	inline void setMeanPos(Vector3 MeanPos) { meanPos = MeanPos; }
	inline void setMeanVel(Vector3 MeanVel) { meanVel = MeanVel; }
	inline void setGenerationProbability(double Probability) { generationProbability = Probability; }
	inline void setNumParticles(int NumParticles) { numParticles = NumParticles; }
	inline void setModel(Particle* Model) { model = Model; }
	inline void setActive(bool Active) { active = Active; }
	
	void setParticleColor(Particle* p);

	inline double generateRandomValue(int precision) { return (double)(rand() % (int) pow(10, precision)) / pow(10, precision); }
};

