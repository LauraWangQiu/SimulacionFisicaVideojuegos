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

extern float inGameTime;
extern float gameTime;
extern int numParticlesEliminated;
extern int gameMode;

class ParticleSystem {
protected:
	list<string>	listOfGeneratorNames;
	list<Particle*> listOfParticles;
	int				numParticles;
	Particle*		originParticle			= nullptr;
	Vector3			gravity;
	PxTransform		origin;
	Camera*			camera					= nullptr;
	bool			stopped					= true;
	bool			end						= false;
	bool			end2					= false;
	bool			end3					= false;

	// GENERADORES DE PARTICULAS
	list<ParticleGenerator*> listOfParticleGenerators;

	ParticleGenerator*	randomGenerator		= nullptr;
	Particle*			randomModel			= nullptr;

	ParticleGenerator* propellantGenerator1	= nullptr;
	ParticleGenerator* propellantGenerator2	= nullptr;
	Particle* propellantModel1				= nullptr;
	Particle* propellantModel2				= nullptr;

	ParticleGenerator*	fireworkGenerator	= nullptr;
	Particle* fireworkModel					= nullptr;

	// GENERADORES DE FUERZAS
	list<ForceGenerator*> listOfForceGenerators;
	ParticleForceRegistry particleForceRegistry;
	
	GravityForceGenerator*		propulsionForceGenerator	= nullptr;
	WhirlWindForceGenerator*	whirlWindsForceGenerator	= nullptr;
	Vector3 center											= SPHERE_END2_POSITION;
	BuoyancyForceGenerator*		buoyancyForceGenerator		= nullptr;
	Particle*					liquidModel					= nullptr;

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

	float cameraAzimuth		= CAMERA_INITIAL_AZIMUTH;
	float cameraElevation	= CAMERA_INITIAL_ELEVATION;
	float cameraRadius		= CAMERA_INITIAL_RADIUS;

public:
	ParticleSystem(PxPhysics* gPhysics, PxScene* gScene, Camera* camera, const Vector3& g = { 0.0f, -9.8f, 0.0f});
	~ParticleSystem();

	void update(double t);
	void onParticleDeath(Particle* p);

#pragma region LISTAS
	void addGeneratorName(string name);
	void clearListOfParticles();
	void addParticleGenerator(ParticleGenerator* pg);
	void addForceGenerator(ForceGenerator* fg);
	void removeForceGenerator(ForceGenerator* fg);
	ParticleGenerator* getParticleGenerator(const string& name);
#pragma endregion

#pragma region ORIGIN
	void addOrigin();
#pragma endregion

#pragma region PARTICULAS
	Particle* addParticle(ParticleType Type,
		PxTransform Transform, Vector3 Dir = Vector3(0.0f, 0.0f, 1.0f),
		bool Visible = true, bool Active = false);
	Particle* addParticle(PxTransform Transform, Vector3 Dir = Vector3(0.0f, 1.0f, 0.0f), 
		float Mass = 1.0f, float Velc = 10.0f,
		Vector3 Acc = VECTOR_ZERO,
		float Damping = 0.99f, Vector3 Size = Vector3(1.0f, 1.0f, 1.0f),
		float Time = 1.0f, Vector4 Color = Vector4(1.0f, 1.0f, 1.0f, 1.0f), string ShapeName = "Sphere", 
		int NumDivisions = 0, int NumExplodes = 0,
		float Density = 1000.0f, Vector3 MassInertiaTensor = Vector3(0.0, 1.0f, 0.0f),
		bool Visible = true, bool Active = false);
	Particle* addParticle(PxPhysics* GPhysics, PxScene* GScene,
		ParticleType Type, PxTransform Transform, Vector3 Dir = Vector3(0.0f, 0.0f, 1.0f), 
		bool Visible = true, bool Active = false);
	void addParticle(Particle* p);
	void addParticles(list<Particle*> list);
	Firework* addFirework(PxPhysics* GPhysics, PxScene* GScene,
		ParticleType Type, PxTransform Transform, Vector3 Dir = Vector3(0.0, 1.0f, 0.0f),
		bool Visible = true, bool Active = false);

	inline void increaseNumParticles() { ++numParticles; }
	inline void decreaseNumParticles() { --numParticles; }
	inline void setNumParticlesToMax() { numParticles = MAX_PARTICLES; }
	inline void updateNumParticles(int n) { numParticles += n; }
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

	void generateFireworkSystem();
	inline Particle* getFireworkModel() const { return fireworkModel; }
	inline void setFireworkModel(Particle* p) { delete fireworkModel; fireworkModel = p; }

	inline void activateFireworkSystem() {
		if (fireworkGenerator != nullptr)
			fireworkGenerator->setActive(!fireworkGenerator->getActive());
	}
#pragma endregion
	
#pragma region GENERADORES DE FUERZAS
	inline ParticleForceRegistry getParticleForceRegistry() const { return particleForceRegistry; }
	void addForces(Particle* p);

	void generatePropulsionForce();
	inline void activatePropulsionForce() {
		if (propulsionForceGenerator != nullptr)
			propulsionForceGenerator->setActive(!propulsionForceGenerator->getActive());
	}

	void generateWhirlWindsForce();
	inline void activateWhirlWindsForce() {
		if (whirlWindsForceGenerator != nullptr)
			whirlWindsForceGenerator->setActive(!whirlWindsForceGenerator->getActive());
	}

	void generateBuoyancyForce();
	inline void activateBuoyancyForce() {
		if (buoyancyForceGenerator != nullptr)
			buoyancyForceGenerator->setActive(!buoyancyForceGenerator->getActive());
	}
#pragma endregion

#pragma region JUEGO
	void createScene();
	void stopMotion(bool m);
	void addSphere(ParticleType type, bool random = false);
	void manageMode();
	void switchMode();

	// Nave
	void createSpacecraft();
	void deleteSpacecraft();
	void createPropellants(Vector3 SpacecraftPos);
	inline void setActivePropellants(bool a) {
		propellantGenerator1->setActive(a);
		propellantGenerator2->setActive(a);
	}
	// Movimiento de la nave
	void left();
	void right();
	void forward();
	void backward();
	void addPropulsion();
	void stopPropulsion();
	void shoot();
	// Personalización de la nave
	void leftColor();
	void rightColor();

	// Seguimiento de la nave
	void objectFollowSpacecraft(Particle* p);
	void generatorFollowSpacecraft(ParticleGenerator* pg);
	// Camara
	void cameraRotate();
	void cameraFollow();
#pragma endregion
};
