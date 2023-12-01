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
// FLOTACI�N
#include "BuoyancyForceGenerator.h"
using namespace std;
using namespace physx;

class ParticleSystem {
protected:
	list<string>	listOfGeneratorNames;
	list<Particle*> listOfParticles;
	int				numParticles;
	Particle*		originParticle			= nullptr;
	Vector3			gravity;
	PxTransform		origin;

	// GENERADORES DE PARTICULAS
	list<ParticleGenerator*> listOfParticleGenerators;

	ParticleGenerator*	fireworkGenerator	= nullptr;
	Firework*			fireworkModel		= nullptr;
	ParticleGenerator*	fireGenerator		= nullptr;
	Particle*			fireModel			= nullptr;
	ParticleGenerator*	waterfallGenerator	= nullptr;
	Particle*			waterfallModel		= nullptr;
	ParticleGenerator*	steamGenerator		= nullptr;
	Particle*			steamModel			= nullptr;
	ParticleGenerator*	squirtGenerator		= nullptr;
	Particle*			squirtModel			= nullptr;
	ParticleGenerator*	windGenerator		= nullptr;
	Particle*			windModel			= nullptr;
	ParticleGenerator*	whirlWindGenerator	= nullptr;
	Particle*			whirlWindModel		= nullptr;

	// GENERADORES DE FUERZAS
	list<ForceGenerator*> listOfForceGenerators;
	ParticleForceRegistry particleForceRegistry;

	GravityForceGenerator*		gravityForceGenerator		= nullptr;
	GravityForceGenerator*		gravityForceGenerator2		= nullptr;
	ParticleDragForceGenerator*	particleDragForceGenerator	= nullptr;
	WindForceGenerator*			windForceGenerator			= nullptr;
	WhirlWindForceGenerator*	whirlWindsForceGenerator	= nullptr;
	ExplosionForceGenerator*	explosionsForceGenerator	= nullptr;
	Vector3 explosionOrigin;
	// MUELLES
	SpringForceGenerator* springForceGenerator		= nullptr;	
	Particle*			  springModel				= nullptr;
	SpringForceGenerator* springForceGeneratorPair1 = nullptr;
	SpringForceGenerator* springForceGeneratorPair2 = nullptr;
	SpringForceGenerator* springForceGenerator1		= nullptr;
	SpringForceGenerator* springForceGenerator2		= nullptr;
	SpringForceGenerator* springForceGenerator3		= nullptr;
	SpringForceGenerator* springForceGenerator4		= nullptr;
	SpringForceGenerator* springForceGenerator5		= nullptr;
	SpringForceGenerator* springForceGenerator6		= nullptr;
	// FLOTACI�N
	BuoyancyForceGenerator* buoyancyForceGenerator	= nullptr; 
	Particle*				liquidModel				= nullptr;

	// S�LIDOS R�GIDOS
	PxPhysics*		gPhysics	= nullptr;
	PxScene*		gScene		= nullptr;
	PxRigidStatic*	floor		= nullptr;
	PxShape*		floorShape	= nullptr;
	RenderItem*		floorRI		= nullptr;

public:
	ParticleSystem(PxPhysics* gPhysics, PxScene* gScene, const Vector3& g = { 0.0f, -9.8f, 0.0f});
	~ParticleSystem();

	void update(double t);
	void onParticleDeath(Particle* p);

#pragma region LISTAS
	void addGeneratorName(string name);
	void addParticleGenerator(ParticleGenerator* pg);
	void addForceGenerator(ForceGenerator* fg);
	void removeForceGenerator(ForceGenerator* fg);
	ParticleGenerator* getParticleGenerator(const string& name);
#pragma endregion

#pragma region ORIGIN
	void addOrigin();
#pragma endregion

#pragma region FIGURAS
	void addCircle(Vector3 center = Vector3(0.0f, 0.0f, 0.0f));
	void addSphere(Vector3 center = Vector3(0.0f, 0.0f, 0.0f));
#pragma endregion

#pragma region PARTICULAS
	void addParticle(ParticleType Type, PxTransform Transform, Vector3 Dir = Vector3(0.0f, 0.0f, 1.0f));
	void addParticle(PxTransform Transform, Vector3 Dir = Vector3(0.0f, 1.0f, 0.0f), float Mass = 1.0f, float Velc = 10.0f,
		Vector3 Acc = Vector3(0.0f, 0.0f, 0.0f), float Damping = 0.99f, Vector3 Size = Vector3(1.0f, 1.0f, 1.0f),
		float Time = 1.0f, Vector4 Color = Vector4(1.0f, 1.0f, 1.0f, 1.0f), int NumDivisions = 0);
	void addParticle(PxPhysics* GPhysics, PxScene* GScene, ParticleType Type, PxTransform Transform, Vector3 Dir = Vector3(0.0f, 0.0f, 1.0f));
	void addParticle(Particle* p);
	void addParticles(list<Particle*> list);
	void addFirework(PxPhysics* GPhysics, PxScene* GScene, ParticleType Type, PxTransform Transform, Vector3 Dir);

	inline void increaseNumParticles() { ++numParticles; }
	inline void decreaseNumParticles() { --numParticles; }
	inline void setNumParticlesToMax() { numParticles = MAX_PARTICLES; }
	inline void updateNumParticles(int n) { numParticles += n; }
	inline void updateNumParticlesText() { num_particles = to_string(numParticles); }
	inline int getNumMaxParticles() const { return MAX_PARTICLES; }
#pragma endregion

#pragma region GENERADORES DE PARTICULAS
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
		if (fireworkGenerator != nullptr) {
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
	}
	inline void activateFireSystem() {
		if (fireGenerator != nullptr)
			fireGenerator->setActive(!fireGenerator->getActive());
	}
	inline void activateWaterfallSystem() {
		if (waterfallGenerator != nullptr)
			waterfallGenerator->setActive(!waterfallGenerator->getActive());
	}
	inline void activateSteamSystem() {
		if (steamGenerator != nullptr)
			steamGenerator->setActive(!steamGenerator->getActive());
	}
	inline void activateSquirtSystem() {
		if (squirtGenerator != nullptr)
			squirtGenerator->setActive(!squirtGenerator->getActive());
	}
	inline void activateWindSystem() {
		if (windGenerator != nullptr)
			windGenerator->setActive(!windGenerator->getActive());
	}
	inline void activateWhirlWindSystem() {
		if (whirlWindGenerator != nullptr)
			whirlWindGenerator->setActive(!whirlWindGenerator->getActive());
	}

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
#pragma endregion
	
#pragma region GENERADORES DE FUERZAS
	inline ParticleForceRegistry getParticleForceRegistry() const { return particleForceRegistry; }
	void addForces(Particle* p);
	
	void generateGravityForce();
	void generateGravityForce2();
	void generateDragForce();
	void generateWindForce();
	void generateWhirlWindsForce();
	void generateExplosionsForce();

	inline void switchGravityForce() {
		if (gravityForceGenerator != nullptr && !gravityForceGenerator->getActive()) {
			cout << "Se activ� el primer generador de gravedad\n";
			gravityForceGenerator->setActive(true);
		}
		else if (gravityForceGenerator != nullptr && gravityForceGenerator->getActive() 
			&& gravityForceGenerator2 != nullptr && !gravityForceGenerator2->getActive()) {
			cout << "Se activ� el segundo generador de gravedad\n";
			gravityForceGenerator2->setActive(true);
		}
		else if (gravityForceGenerator != nullptr && gravityForceGenerator->getActive() 
			&& gravityForceGenerator2 != nullptr && gravityForceGenerator2->getActive()) {
			cout << "Se desactivaron los generadores de gravedad\n";
			gravityForceGenerator->setActive(false);
			gravityForceGenerator2->setActive(false);
		}
	}
	inline void activateDragForce() {
		if (particleDragForceGenerator != nullptr)
			particleDragForceGenerator->setActive(!particleDragForceGenerator->getActive()); 
	}
	inline void activateWindForce() { 
		if (windForceGenerator != nullptr)
			windForceGenerator->setActive(!windForceGenerator->getActive());
	}
	inline void activateWhirlWindsForce() {
		if (whirlWindsForceGenerator != nullptr)
			whirlWindsForceGenerator->setActive(!whirlWindsForceGenerator->getActive());
	}
	inline void activateExplosionsForce() {
		if (explosionsForceGenerator != nullptr)
			explosionsForceGenerator->setActive(!explosionsForceGenerator->getActive());
	}

#pragma region MUELLES
	// MUELLES
	void generateSpringForce();
	inline void activateSpringForce() {
		if (springForceGenerator != nullptr)
			springForceGenerator->setActive(!springForceGenerator->getActive());
	}
	inline void increaseSpringForceK() {
		if (springForceGenerator != nullptr)
			springForceGenerator->setK(springForceGenerator->getK() + 0.1f);
	}
	inline void decreaseSpringForceK() {
		if (springForceGenerator != nullptr)
			springForceGenerator->setK(springForceGenerator->getK() - 0.1f);
	}
	void generateSpringDemo();
	void generateSpringSlinky();
	// FLOTACION
	void generateBuoyancyForce();
	inline void activateBuoyancyForce() {
		if (buoyancyForceGenerator != nullptr)
			buoyancyForceGenerator->setActive(!buoyancyForceGenerator->getActive());
	}
#pragma endregion
#pragma endregion

#pragma region SOLIDOS RIGIDOS
	void createScene();
#pragma endregion
};
