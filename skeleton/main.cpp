#include <ctype.h>
#include <PxPhysicsAPI.h>
#include <vector>
#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"
// ==================
#include "Constants.h"
#include "ParticleSystem.h"

std::string title = "Space up!";
std::string controls1 = "Q-E -> select spacecraft color";
std::string controls2 = "ENTER to PLAY";
std::string controls3 = "WASD to move";
std::string controls4 = "ESPACE to fly";
std::string controls5 = "Right click to shoot";
std::string end = "Congratulations!";
std::string the_end = "THE END";
std::string special_thanks = "Thanks for making the space a better place :)";
float gameTime = GAME_TIME;
float inGameTime = 0.0f;
int numParticlesEliminated = 0;
int gameMode = PERSONALIZATION;

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
	/*gMaterial = gPhysics->createMaterial(0.0f, 0.0f, 0.0f);
	gMaterial = gPhysics->createMaterial(1.0f, 0.0f, 0.0f);
	gMaterial = gPhysics->createMaterial(0.0f, 1.0f, 0.0f);
	gMaterial = gPhysics->createMaterial(0.0f, 0.0f, 1.0f);*/

	// For Solid Rigids +++++++++++++++++++++++++++++++++++++
	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = contactReportFilterShader;
	sceneDesc.simulationEventCallback = &gContactReportCallback;
	gScene = gPhysics->createScene(sceneDesc);

	// ============
	particleSys = std::make_unique<ParticleSystem>(gPhysics, gScene, GetCamera(), sceneDesc.gravity);
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
	// GENERADORES DE PARTICULAS
	case '1': particleSys->activateRandomSystem(); break;
	case '2': particleSys->activateFireworkSystem(); break;
	// GENERADORES DE FUERZAS
	case '3': particleSys->activateWhirlWindsForce(); break;

	// MOVIMIENTO DEL COHETE
	case 'A': particleSys->left(); break;
	case 'D': particleSys->right(); break;
	case 'W': particleSys->forward(); break;
	case 'S': particleSys->backward(); break;
	case ' ': particleSys->addPropulsion(); break;
	// PERSONALIZACION DEL COHETE
	case 'Q': particleSys->leftColor(); break;
	case 'E': particleSys->rightColor(); break;
	case '\r': particleSys->switchMode(); break;

	default: break;
	}
}

void keyUp(unsigned char key, const PxTransform& camera)
{
	PX_UNUSED(camera);

	switch (toupper(key)) {
	case ' ': particleSys->stopPropulsion(); break;
	default: break;
	}
}

void keyPressSpecial(int key, const PxTransform& camera) {

	PX_UNUSED(camera);
	
	/*switch (key) {
	case GLUT_ACTIVE_CTRL:  break;
	}*/
}

void mouseInput(int button) {

	switch (button) {
	// Clic izquierdo
	case 0: break;
	// Clic central
	case 1: break;
	// Clic derecho
	case 2: particleSys->shoot(); break;
	default: break;
	}
}

void onCollision(physx::PxActor* actor1, physx::PxActor* actor2)
{
	PX_UNUSED(actor1);
	PX_UNUSED(actor2);

	if ((actor1->getName() == "SPACECRAFT" && actor2->getName() == "FLOOR") ||
		(actor1->getName() == "FLOOR" && actor2->getName() == "SPACECRAFT")) {
		particleSys->setActivePropellants(false);
	}

	if (gameMode == NORMAL || gameMode == PERSONALIZATION) {
		// Colision Nave - Objeto Random
		if ((actor1->getName() == "SPACECRAFT" && actor2->getName() == "NONE") ||
			(actor1->getName() == "NONE" && actor2->getName() == "SPACECRAFT")) {

			Particle* p1 = static_cast<Particle*>(actor1->userData);
			Particle* p2 = static_cast<Particle*>(actor2->userData);

			// Si son del mismo color, se elimina el Objeto Random
			if (p1->getColor() == p2->getColor()) {
				(actor1->getName() == "NONE") ? p1->setTime(0) :
					p2->setTime(0);
				++numParticlesEliminated;
			}
		}

		// Colision Fuego Artificial - Objeto Random
		if (((actor1->getName() == "FIREWORK" || actor1->getName() == "FIREWORK2") && actor2->getName() == "NONE") ||
			(actor1->getName() == "NONE" && (actor2->getName() == "FIREWORK" || actor2->getName() == "FIREWORK2"))) {

			Particle* p1 = static_cast<Particle*>(actor1->userData);
			Particle* p2 = static_cast<Particle*>(actor2->userData);

			// Desaparecen de la escena y en el caso del fuego artificial, explota
			p1->setToExplode(true);
			p2->setToExplode(true);
			p1->setTime(0);
			p2->setTime(0);
			++numParticlesEliminated;
		}
	}
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