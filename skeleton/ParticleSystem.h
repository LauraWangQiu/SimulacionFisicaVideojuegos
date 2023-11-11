#pragma once
#include <list>
#include "RenderUtils.hpp"
#include "Particle.h"
#include "Firework.h"
#include "GaussianParticleGenerator.h"
#include "UniformParticleGenerator.h"
#include "GravityForceGenerator.h"
#include "ParticleDragGenerator.h"
#include "WindGenerator.h"
#include "ParticleForceRegistry.h"
using namespace std;
using namespace physx;

class ParticleSystem {
protected:
	list<string> listOfGeneratorNames;
	list<Particle*> listOfParticles;
	list<ParticleGenerator*> listOfParticleGenerators;
	Particle* originParticle;
	int numParticles, numMaxParticles;
	Vector3 gravity;
	PxTransform origin;

	ParticleGenerator* fireworkGenerator = nullptr;
	ParticleGenerator* fireGenerator = nullptr;
	ParticleGenerator* waterfallGenerator = nullptr;
	ParticleGenerator* steamGenerator = nullptr;
	ParticleGenerator* squirtGenerator = nullptr;
	ParticleGenerator* windGenerator = nullptr;

	list<ForceGenerator*> listOfForceGenerators;
	ParticleForceRegistry particleForceRegistry;

	ForceGenerator* gravityForceGenerator = nullptr;
	ForceGenerator* gravityForceGenerator2 = nullptr;
	ForceGenerator* particleDragForceGenerator = nullptr;

	ForceGenerator* windForceGenerator = nullptr;
	ForceGenerator* whirlWindsForceGenerator = nullptr;
	ForceGenerator* explosionsForceGenerator = nullptr;

public:
	ParticleSystem(const Vector3& g = { 0.0f, -9.8f, 0.0f});
	~ParticleSystem();
	void addGeneratorName(string name);
	void addParticleGenerator(ParticleGenerator* pg);
	void addForceGenerator(ForceGenerator* fg);

	// GIZMO
	void addOrigin();

	void addParticle(ParticleType Type, PxTransform Transform, Vector3 Dir = Vector3(0.0f, 0.0f, 1.0f));
	void addParticle(PxTransform Transform, Vector3 Dir = Vector3(0.0f, 1.0f, 0.0f), float Mass = 1.0f, float Velc = 10.0f,
		Vector3 Acc = Vector3(0.0f, 0.0f, 0.0f), float Damping = 0.99f, Vector3 Size = Vector3(1.0f, 1.0f, 1.0f),
		float Time = 1.0f, Vector4 Color = Vector4(1.0f, 1.0f, 1.0f, 1.0f), int NumDivisions = 0);
	void addParticle(Particle* p);
	void addParticles(list<Particle*> list);

	void update(double t);
	void onParticleDeath(Particle* p);

	inline void increaseNumParticles() { ++numParticles; }
	inline void decreaseNumParticles() { --numParticles; }
	inline void updateNumParticles(int n) { numParticles += n; }
	inline void updateNumParticlesText() { num_particles = to_string(numParticles); }
	inline void setNumMaxParticles(int max) { numMaxParticles = max; }

	inline int getNumMaxParticles() { return numMaxParticles; }

	// GENERADORES DE PARTICULAS
	ParticleGenerator *getParticleGenerator(const string& name);
	inline void activateParticleGenerator(const string& name) {
		getParticleGenerator(name)->setActive(true);
	}
	inline void deactivateParticleGenerator(const string& name) {
		getParticleGenerator(name)->setActive(false);
	}

	void generateFireworkSystem();
	void generateFireSystem();
	void generateWaterfallSystem();
	void generateSteamSystem();
	void generateSquirtSystem();
	void generateWindSystem();

	inline void switchFireworkSystem() { 
		if (!fireworkGenerator->getActive()) {
			fireworkGenerator->setActive(true);
			fireworkGenerator->getModel()->setParticleType(FIREWORK);
		}
		else {
			switch (fireworkGenerator->getModel()->getParticleType()) {
			case FIREWORK: fireworkGenerator->getModel()->setParticleType(FIREWORK2); break;
			case FIREWORK2: fireworkGenerator->getModel()->setParticleType(FIREWORK3); break;
			case FIREWORK3: fireworkGenerator->setActive(!fireworkGenerator->getActive()); break;
			}
		}
	}

	inline void activateFireSystem() { fireGenerator->setActive(!fireGenerator->getActive()); }
	inline void activateWaterfallSystem() { waterfallGenerator->setActive(!waterfallGenerator->getActive()); }
	inline void activateSteamSystem() { steamGenerator->setActive(!steamGenerator->getActive()); }
	inline void activateSquirtSystem() { squirtGenerator->setActive(!squirtGenerator->getActive()); }
	inline void activateWindSystem() { windGenerator->setActive(!windGenerator->getActive()); }

	inline void increaseSquirtVel() {
		Vector3 newVel = Vector3(squirtGenerator->getModel()->getVel().x, squirtGenerator->getModel()->getVel().y + SQUIRT_INCREASE_VEL, squirtGenerator->getModel()->getVel().z);
		squirtGenerator->getModel()->setVel(newVel);
	}
	inline void decreaseSquirtVel() {
		Vector3 newVel = Vector3(squirtGenerator->getModel()->getVel().x, squirtGenerator->getModel()->getVel().y - SQUIRT_INCREASE_VEL, squirtGenerator->getModel()->getVel().z);
		squirtGenerator->getModel()->setVel(newVel);
	}
	inline void resetSquirtVel() { 
		Vector3 newVel = Vector3(squirtGenerator->getModel()->getVel().x, SQUIRT_INITIAL_VEL, squirtGenerator->getModel()->getVel().z);
		squirtGenerator->getModel()->setVel(newVel);
	}

	void addFirework(ParticleType Type, PxTransform Transform, Vector3 Dir);
	
	// GENERADORES DE FUERZAS
	inline ParticleForceRegistry getParticleForceRegistry() const { return particleForceRegistry; }
	void addForces(Particle* p);

	void removeForceGenerator(ForceGenerator* fg);

	void generateGravityForce();
	void generateGravityForce2();
	void generateDragForce();
	void generateWindForce();
	void generateWhirlWindsForce();
	void generateExplosionsForce();

	void switchGravityForce();
	void activateDragForce();
	void activateWindForce();
	void activateWhirlWindsForce();
	void activateExplosionsForce();
};
