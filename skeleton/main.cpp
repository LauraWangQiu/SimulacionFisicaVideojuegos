#include <ctype.h>
#include <PxPhysicsAPI.h>
#include <vector>
#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"
#include <iostream>
// ==================
#include "ParticleSystem.h"

std::string display_text = "Simulacion Fisica para Videojuegos - Laura Wang Qiu";


using namespace physx;

PxDefaultAllocator		gAllocator;
PxDefaultErrorCallback	gErrorCallback;

PxFoundation*			gFoundation = NULL;
PxPhysics*				gPhysics	= NULL;


PxMaterial*				gMaterial	= NULL;

PxPvd*                  gPvd        = NULL;

PxDefaultCpuDispatcher*	gDispatcher = NULL;
PxScene*				gScene      = NULL;
ContactReportCallback gContactReportCallback;

// ============
std::unique_ptr<ParticleSystem> particleSys;


// Initialize physics engine
void initPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gAllocator, gErrorCallback);

	gPvd = PxCreatePvd(*gFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	gPvd->connect(*transport,PxPvdInstrumentationFlag::eALL);

	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(),true,gPvd);

	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);

	// For Solid Rigids +++++++++++++++++++++++++++++++++++++
	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = contactReportFilterShader;
	sceneDesc.simulationEventCallback = &gContactReportCallback;
	gScene = gPhysics->createScene(sceneDesc);

	// ============
	particleSys = std::make_unique<ParticleSystem>();
}


// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds
void stepPhysics(bool interactive, double t)
{
	PX_UNUSED(interactive);

	gScene->simulate(t);
	gScene->fetchResults(true);

	// ============
	particleSys->update(t);
}

// Function to clean data
// Add custom code to the begining of the function
void cleanupPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	// Rigid Body ++++++++++++++++++++++++++++++++++++++++++
	gScene->release();
	gDispatcher->release();
	// -----------------------------------------------------
	gPhysics->release();	
	PxPvdTransport* transport = gPvd->getTransport();
	gPvd->release();
	transport->release();
	
	gFoundation->release();
}

// Function called when a key is pressed
void keyPress(unsigned char key, const PxTransform& camera)
{
	PX_UNUSED(camera);

	switch(toupper(key)) {
	// Cambiar vista de la camara
	case 'F': GetCamera()->setView(PxVec3(0.0f, 0.0f, 0.0f), PxVec3(0.0f, 0.0f, 1.0f)); break;
	case 'L': GetCamera()->setView(PxVec3(-100.0f, 0.0f, 0.0f), PxVec3(1.0f, 0.0f, 0.0f)); break;

	case '0': particleSys->addParticle(BASIC, GetCamera()->getTransform(), GetCamera()->getDir()); break;
	//case '0': particleSys->addParticle(BASIC, PxTransform(0.0f, 0.0f, 0.0f)); break;
	case '1': particleSys->addProjectile(CANNON_BALL, GetCamera()->getTransform(), GetCamera()->getDir()); break;
	//case '1': particleSys->addProjectile(CANNON_BALL, PxTransform(0.0f, 0.0f, 0.0f)); break;
	case '2': particleSys->addProjectile(TANK_BALL, GetCamera()->getTransform(), GetCamera()->getDir()); break;
	//case '2': particleSys->addProjectile(TANK_BALL, PxTransform(0.0f, 0.0f, 0.0f)); break;
	case '3': particleSys->addProjectile(GUN_BULLET, GetCamera()->getTransform(), GetCamera()->getDir()); break;
	//case '3': particleSys->addProjectile(GUN_BULLET, PxTransform(0.0f, 0.0f, 0.0f)); break;
	case '4': particleSys->addProjectile(LASER, GetCamera()->getTransform(), GetCamera()->getDir()); break;
	//case '4': particleSys->addProjectile(LASER, PxTransform(0.0f, 0.0f, 0.0f)); break;
	//case ' ': particleSys->generateFireworkSystem(); break;
	default: break;
	}
}

void onCollision(physx::PxActor* actor1, physx::PxActor* actor2)
{
	PX_UNUSED(actor1);
	PX_UNUSED(actor2);
}


int main(int, const char*const*)
{
#ifndef OFFLINE_EXECUTION 
	extern void renderLoop();
	renderLoop();
#else
	static const PxU32 frameCount = 100;
	initPhysics(false);
	for(PxU32 i=0; i<frameCount; i++)
		stepPhysics(false);
	cleanupPhysics(false);
#endif

	return 0;
}