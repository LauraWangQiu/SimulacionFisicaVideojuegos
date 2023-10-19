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
	Vector3 gravity;

	PxTransform origin;
	ParticleGenerator* fireworkGenerator;
	//vector<Firework*> fireworkPool;
	ParticleGenerator* fireGenerator;
	//vector<Firework*> firePool;
	ParticleGenerator* waterfallGenerator;
	//vector<Firework*> waterfallPool;
	ParticleGenerator* steamGenerator;
	//vector<Firework*> steamPool;
public:
	ParticleSystem(const Vector3& g = { 0.0f, -9.8f, 0.0f});
	~ParticleSystem();

	void addParticle(ParticleType Type, PxTransform Transform, Vector3 Dir = Vector3(0.0f, 0.0f, 1.0f));
	void addParticle(PxTransform Transform, Vector3 Dir = Vector3(0.0f, 1.0f, 0.0f), float Mass = 1.0f, float Velc = 10.0f,
		Vector3 Acc = Vector3(0.0f, 0.0f, 0.0f), float Damping = 0.99f, Vector3 Size = Vector3(1.0f, 1.0f, 1.0f),
		float Time = 1.0f, Vector4 Color = Vector4(1.0f, 1.0f, 1.0f, 1.0f), int NumDivisions = 0);

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
	void generateSteamSystem();

	void addFirework(ParticleType Type, PxTransform Transform, Vector3 Dir);
};

