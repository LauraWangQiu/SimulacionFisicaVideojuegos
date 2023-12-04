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

	ParticleGenerator* propellerGenerator1	= nullptr;
	ParticleGenerator* propellerGenerator2	= nullptr;
	Particle* propellantModel1				= nullptr;
	Particle* propellantModel2				= nullptr;

	// GENERADORES DE FUERZAS
	list<ForceGenerator*> listOfForceGenerators;
	ParticleForceRegistry particleForceRegistry;
	
	// SÓLIDOS RÍGIDOS
	PxPhysics*		gPhysics	= nullptr;
	PxScene*		gScene		= nullptr;
	PxRigidStatic*	floor		= nullptr;
	PxShape*		floorShape	= nullptr;
	RenderItem*		floorRI		= nullptr;

	Particle* spacecraft		= nullptr;
	Particle* propellant1		= nullptr;
	Particle* propellant2		= nullptr;
	Particle* window			= nullptr;
	particlePalettes palettes;
	int colorIndex = 0;

	GravityForceGenerator*		propulsionForceGenerator	= nullptr;

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
	Particle* addParticle(ParticleType Type, PxTransform Transform, Vector3 Dir = Vector3(0.0f, 0.0f, 1.0f), bool Visible = true, bool Active = false);
	Particle* addParticle(PxTransform Transform, Vector3 Dir = Vector3(0.0f, 1.0f, 0.0f), float Mass = 1.0f, float Velc = 10.0f,
		Vector3 Acc = Vector3(0.0f, 0.0f, 0.0f), float Damping = 0.99f, Vector3 Size = Vector3(1.0f, 1.0f, 1.0f),
		float Time = 1.0f, Vector4 Color = Vector4(1.0f, 1.0f, 1.0f, 1.0f), string ShapeName = "Sphere", int NumDivisions = 0, int NumExplodes = 0,
		float Density = 1000.0f, bool Visible = true, bool Active = false);
	Particle* addParticle(PxPhysics* GPhysics, PxScene* GScene, ParticleType Type, PxTransform Transform, Vector3 Dir = Vector3(0.0f, 0.0f, 1.0f), bool Visible = true, bool Active = false);
	void addParticle(Particle* p);
	void addParticles(list<Particle*> list);
	Firework* addFirework(PxPhysics* GPhysics, PxScene* GScene, ParticleType Type, PxTransform Transform, Vector3 Dir, bool Visible = true, bool Active = false);

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
#pragma endregion

#pragma region JUEGO
	void createScene();
	void generatePropellants(Vector3 SpacecraftPos);
	void generatePropulsionForce();
	void left();
	void right();
	void addPropulsion();
	void stopPropulsion();

	void leftColor();
	void rightColor();
#pragma endregion
};
