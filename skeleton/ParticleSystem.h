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

	ParticleGenerator*	randomGenerator		= nullptr;
	Particle*			randomModel			= nullptr;

	// GENERADORES DE FUERZAS
	list<ForceGenerator*> listOfForceGenerators;
	ParticleForceRegistry particleForceRegistry;

	GravityForceGenerator*		gravityForceGenerator		= nullptr;
	ParticleDragForceGenerator*	particleDragForceGenerator	= nullptr;
	WindForceGenerator*			windForceGenerator			= nullptr;
	WhirlWindForceGenerator*	whirlWindsForceGenerator	= nullptr;
	ExplosionForceGenerator*	explosionsForceGenerator	= nullptr;
	Vector3 explosionOrigin;
	
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

#pragma region PARTICULAS
	void addParticle(ParticleType Type, PxTransform Transform, Vector3 Dir = Vector3(0.0f, 0.0f, 1.0f));
	void addParticle(PxTransform Transform, Vector3 Dir = Vector3(0.0f, 1.0f, 0.0f), float Mass = 1.0f, float Velc = 10.0f,
		Vector3 Acc = Vector3(0.0f, 0.0f, 0.0f), float Damping = 0.99f, Vector3 Size = Vector3(1.0f, 1.0f, 1.0f),
		float Time = 1.0f, Vector4 Color = Vector4(1.0f, 1.0f, 1.0f, 1.0f), string ShapeName = "Sphere", int NumDivisions = 0, int NumExplodes = 0);
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

	void generateRandomSystem();
	inline Particle* getRandomModel() const { return randomModel; }
	inline void setRandomModel(Particle* p) { delete randomModel; randomModel = p; }

	inline void activateRandomSystem() {
		if (randomGenerator != nullptr)
			randomGenerator->setActive(!randomGenerator->getActive());
	}
#pragma endregion
	
#pragma region GENERADORES DE FUERZAS
	inline ParticleForceRegistry getParticleForceRegistry() const { return particleForceRegistry; }
	void addForces(Particle* p);
	
	void generateGravityForce();
	void generateDragForce();
	void generateWindForce();
	void generateWhirlWindsForce();
	void generateExplosionsForce();

	inline void activateGravityForce() {
		if (gravityForceGenerator != nullptr)
			gravityForceGenerator->setActive(!gravityForceGenerator->getActive());
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
#pragma endregion

#pragma region FIGURAS
	void addCircle(Vector3 center = { 0.0f, 0.0f, 0.0f });
	void addSphere(Vector3 center = { 0.0f, 0.0f, 0.0f });
#pragma endregion

#pragma region SOLIDOS RIGIDOS
	void createScene();
#pragma endregion
};
