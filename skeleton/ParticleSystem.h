#pragma once
#include <list>
#include "RenderUtils.hpp"
#include "Particle.h"
#include "Firework.h"
#include "GaussianParticleGenerator.h"
#include "UniformParticleGenerator.h"
using namespace std;
using namespace physx;

class ParticleSystem {
protected:
	list<Particle*> listOfParticles;
	list<ParticleGenerator*> listOfParticleGenerators;
	int numParticles, numMaxParticles;
	//Vector3 gravity;

	ParticleGenerator* fireworkGenerator;
	//vector<Firework*> fireworkPool;
	ParticleGenerator* fireGenerator;
	//vector<Firework*> firePool;
	ParticleGenerator* waterfallGenerator;
	//vector<Firework*> waterfallPool;
public:
	ParticleSystem(const Vector3& g = { 0.0f, -9.8f, 0.0f});
	~ParticleSystem();

	void addParticle(ParticleType Type, PxTransform Transform, Vector3 Dir = Vector3(0.0f, 0.0f, 1.0f), float Time = 5.0f, PxReal Size = 1.0f, Vector4 Color = Vector4(255.0, 255.0, 255.0, 255.0));
	void addParticles(list<Particle*> list);

	void update(double t);

	void onParticleDeath(Particle* p);

	ParticleGenerator *getParticleGenerator(const string& name);
	inline void activateParticleGenerator(const string& name) {
		getParticleGenerator(name)->setActive(true);
	}
	inline void deactivateParticleGenerator(const string& name) {
		getParticleGenerator(name)->setActive(false);
	}

	inline void increaseNumParticles() { ++numParticles; }
	inline void decreaseNumParticles() { --numParticles; }
	inline void setNumMaxParticles(int max) { numMaxParticles = max; }
	inline int getNumMaxParticles() { return numMaxParticles; }

	void generateFireworkSystem();
	void generateFireSystem();
	void generateWaterfallSystem();
};

