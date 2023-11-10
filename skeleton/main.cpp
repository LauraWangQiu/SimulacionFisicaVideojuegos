#include <ctype.h>
#include <PxPhysicsAPI.h>
#include <vector>
#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"
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
	particleSys = std::make_unique<ParticleSystem>(sceneDesc.gravity);
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

	case 'Z': particleSys->switchFireworkSystem(); break;
	case 'X': particleSys->activateFireSystem();  break;
	case 'C': particleSys->activateWaterfallSystem(); break;
	case 'V': particleSys->activateSteamSystem(); break;
	case 'B': particleSys->activateSquirtSystem(); break;
	case 'N': particleSys->increaseSquirtVel(); break;
	case 'M': particleSys->decreaseSquirtVel(); break;

	//case '0': particleSys->addParticle(BASIC, camera, GetCamera()->getDir()); break;
	case '0': particleSys->addParticle(BASIC, PxTransform(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f)); break;
	//case '1': particleSys->addParticle(CANNON_BALL, camera, GetCamera()->getDir()); break;
	case '1': particleSys->addParticle(CANNON_BALL, PxTransform(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f)); break;
	//case '2': particleSys->addParticle(TANK_BALL, camera, GetCamera()->getDir()); break;
	case '2': particleSys->addParticle(TANK_BALL, PxTransform(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f)); break;
	//case '3': particleSys->addParticle(GUN_BULLET, camera, GetCamera()->getDir()); break;
	case '3': particleSys->addParticle(GUN_BULLET, PxTransform(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f)); break;
	//case '4': particleSys->addParticle(LASER, camera, GetCamera()->getDir()); break;
	case '4': particleSys->addParticle(LASER, PxTransform(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f)); break;
	//case '5': particleSys->addFirework(FIREWORK, camera, GetCamera()->getDir()); break;
	case '5': particleSys->addFirework(FIREWORK, PxTransform(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f)); break;
	//case '6': particleSys->addFirework(FIREWORK2, camera, GetCamera()->getDir()); break;
	case '6': particleSys->addFirework(FIREWORK2, PxTransform(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f)); break;
	//case '7': particleSys->addFirework(FIREWORK3, camera, GetCamera()->getDir()); break;
	case '7': particleSys->addFirework(FIREWORK3, PxTransform(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f)); break;

	case 'U': particleSys->addGravityForce(); break;
	case 'I': particleSys->removeGravityForce(); break;
	case 'O': particleSys->addDragForce(); break;
	case 'P': particleSys->removeDragForce(); break;

	case ' ': break; // Para casos de prueba
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
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); // Check Memory Leaks

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