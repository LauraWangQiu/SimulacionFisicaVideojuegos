#pragma once
#include <list>
#include "RenderUtils.hpp"
// PARTICULAS
#include "Particle.h"
#include "Firework.h"
// GENERADORES DE PARTICULAS
#include "GaussianParticleGenerator.h"
#include "UniformParticleGenerator.h"
// GENERADORES DE FUERZAS
#include "ParticleForceRegistry.h"
#include "GravityForceGenerator.h"
#include "ParticleDragForceGenerator.h"
#include "WindForceGenerator.h"
#include "WhirlWindForceGenerator.h"
#include "ExplosionForceGenerator.h"
// MUELLES
#include "SpringForceGenerator.h"
// FLOTACIÓN
#include "BuoyancyForceGenerator.h"
using namespace std;
using namespace physx;

class ParticleSystem {
protected:
	list<string> listOfGeneratorNames;
	list<Particle*> listOfParticles;
	int numParticles, numMaxParticles;
	Particle* originParticle = nullptr;
	Vector3 gravity;
	PxTransform origin;

	// GENERADORES DE PARTICULAS
	list<ParticleGenerator*> listOfParticleGenerators;

	ParticleGenerator* fireworkGenerator = nullptr;
	Firework* fireworkModel = nullptr;
	ParticleGenerator* fireGenerator = nullptr;
	Particle* fireModel = nullptr;
	ParticleGenerator* waterfallGenerator = nullptr;
	Particle* waterfallModel = nullptr;
	ParticleGenerator* steamGenerator = nullptr;
	Particle* steamModel = nullptr;
	ParticleGenerator* squirtGenerator = nullptr;
	Particle* squirtModel = nullptr;
	ParticleGenerator* windGenerator = nullptr;
	Particle* windModel = nullptr;
	ParticleGenerator* whirlWindGenerator = nullptr;
	Particle* whirlWindModel = nullptr;

	// GENERADORES DE FUERZAS
	list<ForceGenerator*> listOfForceGenerators;
	ParticleForceRegistry particleForceRegistry;

	GravityForceGenerator* gravityForceGenerator = nullptr;
	GravityForceGenerator* gravityForceGenerator2 = nullptr;
	ParticleDragForceGenerator* particleDragForceGenerator = nullptr;
	WindForceGenerator* windForceGenerator = nullptr;
	WhirlWindForceGenerator* whirlWindsForceGenerator = nullptr;
	ExplosionForceGenerator* explosionsForceGenerator = nullptr; Vector3 explosionOrigin;
	// MUELLES
	SpringForceGenerator* springForceGenerator = nullptr;	Particle* springModel = nullptr;
	SpringForceGenerator* springForceGeneratorPair1 = nullptr;
	SpringForceGenerator* springForceGeneratorPair2 = nullptr;
	SpringForceGenerator* springForceGenerator1 = nullptr;
	SpringForceGenerator* springForceGenerator2 = nullptr;
	SpringForceGenerator* springForceGenerator3 = nullptr;
	SpringForceGenerator* springForceGenerator4 = nullptr;
	SpringForceGenerator* springForceGenerator5 = nullptr;
	SpringForceGenerator* springForceGenerator6 = nullptr;
	// FLOTACIÓN
	BuoyancyForceGenerator* buoyancyForceGenerator = nullptr; Particle* liquidModel = nullptr;

public:
	ParticleSystem(const Vector3& g = { 0.0f, -9.8f, 0.0f});
	~ParticleSystem();

	void update(double t);
	void onParticleDeath(Particle* p);

	// LISTAS
	void addGeneratorName(string name);
	void addParticleGenerator(ParticleGenerator* pg);
	void addForceGenerator(ForceGenerator* fg);

	// ORIGIN
	void addOrigin();

	// FIGURAS
	void addCircle(Vector3 center = Vector3(0.0f, 0.0f, 0.0f));
	void addSphere(Vector3 center = Vector3(0.0f, 0.0f, 0.0f));

	void addParticle(ParticleType Type, PxTransform Transform, Vector3 Dir = Vector3(0.0f, 0.0f, 1.0f));
	void addParticle(PxTransform Transform, Vector3 Dir = Vector3(0.0f, 1.0f, 0.0f), float Mass = 1.0f, float Velc = 10.0f,
		Vector3 Acc = Vector3(0.0f, 0.0f, 0.0f), float Damping = 0.99f, Vector3 Size = Vector3(1.0f, 1.0f, 1.0f),
		float Time = 1.0f, Vector4 Color = Vector4(1.0f, 1.0f, 1.0f, 1.0f), int NumDivisions = 0);
	void addParticle(Particle* p);
	void addParticles(list<Particle*> list);

	inline void increaseNumParticles() { ++numParticles; }
	inline void decreaseNumParticles() { --numParticles; }
	inline void updateNumParticles(int n) { numParticles += n; }
	inline void updateNumParticlesText() { num_particles = to_string(numParticles); }
	inline int getNumMaxParticles() const { return numMaxParticles; }
	inline void setNumMaxParticles(int max) { numMaxParticles = max; }

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
	void generateWhirlWindSystem();

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
	inline void activateWhirlWindSystem() { whirlWindGenerator->setActive(!whirlWindGenerator->getActive()); }

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

	inline void switchGravityForce() {
		if (!gravityForceGenerator->getActive()) {
			cout << "Se activó el primer generador de gravedad\n";
			gravityForceGenerator->setActive(true);
		}
		else if (gravityForceGenerator->getActive() && !gravityForceGenerator2->getActive()) {
			cout << "Se activó el segundo generador de gravedad\n";
			gravityForceGenerator2->setActive(true);
		}
		else if (gravityForceGenerator->getActive() && gravityForceGenerator2->getActive()) {
			cout << "Se desactivaron los generadores de gravedad\n";
			gravityForceGenerator->setActive(false);
			gravityForceGenerator2->setActive(false);
		}
	}
	inline void activateDragForce() { particleDragForceGenerator->setActive(!particleDragForceGenerator->getActive()); }
	inline void activateWindForce() { windForceGenerator->setActive(!windForceGenerator->getActive()); }
	inline void activateWhirlWindsForce() { whirlWindsForceGenerator->setActive(!whirlWindsForceGenerator->getActive()); }
	inline void activateExplosionsForce() { explosionsForceGenerator->setActive(!explosionsForceGenerator->getActive()); }

	// MUELLES
	void generateSpringForce();
	inline void activateSpringForce() { springForceGenerator->setActive(!springForceGenerator->getActive()); }
	inline void increaseSpringForceK() { springForceGenerator->setK(springForceGenerator->getK() + 0.1f); }
	inline void decreaseSpringForceK() { springForceGenerator->setK(springForceGenerator->getK() - 0.1f); }
	void generateSpringDemo();
	void generateSpringSlinky();
	// FLOTACION
	void generateBuoyancyForce();
	inline void activateBuoyancyForce() { buoyancyForceGenerator->setActive(!buoyancyForceGenerator->getActive()); }
};
