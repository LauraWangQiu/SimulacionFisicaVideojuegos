#include "ParticleSystem.h"

//#define ORIGIN

// GENERADORES DE PARTICULAS
#define FIREWORK_MODEL_VISIBLE false
#define RANDOM_MODEL_VISIBLE false

// SOLIDOS RIGIDOS
#define RANDOM_GEN_RIGIDBODY true
#define PROPELLER_GEN_RIGIDBODY false

#define PROPULSION_FORCE Vector3(0.0f, 50.0f, 0.0f)

ParticleSystem::ParticleSystem(PxPhysics* gPhysics, PxScene* gScene, const Vector3& g) : gravity(g), 
	numParticles(0),
	origin(PxTransform(Vector3(0.0f, 0.0f, 0.0f))),
	gPhysics(gPhysics), gScene(gScene) {

#ifdef ORIGIN
	addOrigin();
#endif // ORIGIN

	// GENERADORES DE PARTICULAS
	generateRandomSystem();

	createScene();
}

ParticleSystem::~ParticleSystem() {

	listOfParticles.clear();
	listOfParticleGenerators.clear();

	originParticle		= nullptr;

	randomGenerator		= nullptr; randomModel		= nullptr;

	for (auto it = listOfForceGenerators.begin(); it != listOfForceGenerators.end(); ++it) {
		delete* it; *it = nullptr;
	}
	listOfForceGenerators.clear();

	spacecraft					= nullptr;
	propulsionForceGenerator	= nullptr;
}

void ParticleSystem::update(double t) {

	// Recorro todos los generadores de particulas y genero particulas anadiendolas a listOfParticles
	for (auto pg : listOfParticleGenerators) {
		pg->updateActualNumParticles(numParticles);
		if (pg->getActive()) {

			if (pg->getName() == "Propeller1")
				pg->getModel()->setPos(Vector3(spacecraft->getPosX() - 2.0f, spacecraft->getPosY() - 5.0f, spacecraft->getPosZ()));
			else if (pg->getName() == "Propeller2")
				pg->getModel()->setPos(Vector3(spacecraft->getPosX() + 2.0f, spacecraft->getPosY() - 5.0f, spacecraft->getPosZ()));

			auto list = pg->generateParticles();
			addParticles(list);
		}
	}

	// Recorro todos los generadores de fuerzas y elimino aquellos que han terminado
	auto fg = listOfForceGenerators.begin();
	while (fg != listOfForceGenerators.end()) {
		if (!(*fg)->updateTime(t)) {
			particleForceRegistry.removeForceGenerator(*fg);
			listOfGeneratorNames.remove((*fg)->getName());
			delete* fg;
			fg = listOfForceGenerators.erase(fg);
		}
		else ++fg;
	}

	// Actualiza las fuerzas que actuan sobre las particulas
	particleForceRegistry.updateForces();

	// Actualizo las particulas y elimino aquellas no vivas
	auto p = listOfParticles.begin();
	while (p != listOfParticles.end()) {
		// Si se termina su tiempo de vida, se elimina
		if (!(*p)->integrate(t)) {

			if (*p == spacecraft) {
				propellerGenerator1->setActive(false);
				propellerGenerator2->setActive(false);
				spacecraft = nullptr;
			}
			onParticleDeath(*p);
			(*p)->setDelete(true);
			(*p)->release();
			particleForceRegistry.deleteParticleRegistry(*p);
			p = listOfParticles.erase(p);
			decreaseNumParticles();
			updateNumParticlesText();
		}
		else {
			addForces(*p);
			++p;
		}
	}
}

void ParticleSystem::onParticleDeath(Particle* p) {
	switch (p->getParticleType()) {
	case FIREWORK:
		addParticles(static_cast<Firework*>(p)->explode(numParticles));
		break;
	}
}

#pragma region LISTAS
void ParticleSystem::addGeneratorName(string name) {
	listOfGeneratorNames.push_back(name);
}

void ParticleSystem::addParticleGenerator(ParticleGenerator* pg) {
	listOfParticleGenerators.push_back(pg);
	addGeneratorName(pg->getName());
}

void ParticleSystem::addForceGenerator(ForceGenerator* fg) {
	listOfForceGenerators.push_back(fg);
	addGeneratorName(fg->getName());
}

void ParticleSystem::removeForceGenerator(ForceGenerator* fg) {
	if (fg != nullptr) {
		particleForceRegistry.removeForceGenerator(fg);
		auto it = listOfForceGenerators.begin();
		while (it != listOfForceGenerators.end()) {
			if (*it == fg) {
				listOfGeneratorNames.remove((*it)->getName());
				delete* it;
				listOfForceGenerators.erase(it);
				fg = nullptr;
				break;
			}
			++it;
		}
	}
}

ParticleGenerator* ParticleSystem::getParticleGenerator(const string& name) {
	auto it = listOfParticleGenerators.begin();
	while (it != listOfParticleGenerators.end()) {
		if ((*it)->getName() == name) return *it;
	}
}
#pragma endregion

#pragma region ORIGIN
void ParticleSystem::addOrigin() {
	originParticle = new Particle(BASIC, origin, Vector3(0.0f, 0.0f, 0.0f), true, true);
}
#pragma endregion

#pragma region PARTICULAS
void ParticleSystem::addParticles(list<Particle*> list) {
	for (auto p : list) {
		listOfParticles.push_back(p);
	}
	updateNumParticles(list.size());
	updateNumParticlesText();
}

Particle* ParticleSystem::addParticle(ParticleType Type, PxTransform Transform, Vector3 Dir, bool Visible, bool Active) {
	Particle* p = new Particle(Type, Transform, Dir, Visible, Active);
	addParticle(p);
	return p;
}

Particle* ParticleSystem::addParticle(PxTransform Transform, Vector3 Dir, float Mass, float Velc, Vector3 Acc, float Damping, Vector3 Size,
	float Time, Vector4 Color, string ShapeName, int NumDivisions, int NumExplodes, float Density, bool Visible, bool Active) {
	Particle* p = new Particle(Transform, Dir, Mass, Velc, Acc, Damping, Size, Time, Color, ShapeName, NumDivisions, NumExplodes, Density, Visible, Active);

	p->setRandom();
	addParticle(p);
	return p;
}

Particle* ParticleSystem::addParticle(PxPhysics* GPhysics, PxScene* GScene, ParticleType Type, PxTransform Transform, Vector3 Dir, bool Visible, bool Active) {
	Particle* p = new Particle(GPhysics, GScene, Type, Transform, Dir, Visible, Active);
	addParticle(p);
	return p;
}

void ParticleSystem::addParticle(Particle* p) {
	listOfParticles.push_back(p);
	increaseNumParticles();
	updateNumParticlesText();
}

Firework* ParticleSystem::addFirework(PxPhysics* gPhysics, PxScene* gScene, ParticleType Type, PxTransform Transform, Vector3 Dir, bool Visible, bool Active) {
	Firework* model = nullptr;
	Firework* p = nullptr;

	if (gPhysics == nullptr || gScene == nullptr) {
		model = new Firework(Type, Transform, Dir, FIREWORK_MODEL_VISIBLE);
		p = new Firework(Type, Transform, Dir);
	}
	else {
		model = new Firework(gPhysics, gScene, Type, Transform, Dir, FIREWORK_MODEL_VISIBLE);
		p = new Firework(gPhysics, gScene, Type, Transform, Dir);
	}

	addParticle(p);
	GaussianParticleGenerator* g = new GaussianParticleGenerator("Firework", Transform.p, Dir, 0.1, 1,
		model, Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f, 1.0f, 1.0f));
	p->addGenerator(g);

	return p;
}
#pragma endregion

#pragma region GENERADORES DE PARTICULAS
void ParticleSystem::generateRandomSystem() {
	if (randomGenerator == nullptr) {
		if (!RANDOM_GEN_RIGIDBODY) randomModel = new Particle(RANDOM, origin, Vector3(0.0f, 1.0f, 0.0f), RANDOM_MODEL_VISIBLE);
		else randomModel = new Particle(gPhysics, gScene, RANDOM, origin, Vector3(0.0f, 1.0f, 0.0f), RANDOM_MODEL_VISIBLE);

		randomGenerator = new GaussianParticleGenerator("Random", origin.p, Vector3(0.0f, 0.0f, 0.0f), 0.1, 1,
			randomModel, Vector3(1.0f, 1.0f, 1.0f), Vector3(1.0f, 1.0f, 1.0f));
		addParticleGenerator(randomGenerator);
	}
}
#pragma endregion

#pragma region GENERADORES DE FUERZAS
void ParticleSystem::addForces(Particle* p) {
	if (propulsionForceGenerator != nullptr && propulsionForceGenerator->getActive()) {
		if (p == spacecraft)
			particleForceRegistry.addRegistry(propulsionForceGenerator, p);
	}
}
#pragma endregion

#pragma region JUEGO
void ParticleSystem::createScene() {
	//// Generar suelo
	//floor = gPhysics->createRigidStatic(PxTransform(Vector3(0.0f, -20.0f, 0.0f)));
	//floorShape = CreateShape(PxBoxGeometry(100, 1, 100));
	//floor->attachShape(*floorShape);
	//gScene->addActor(*floor);
	//// Pintar suelo
	//floorRI = new RenderItem(floorShape, floor, { 0.8, 0.8, 0.8, 1 });

	spacecraft = addParticle(gPhysics, gScene, SPACECRAFT, origin, Vector3(0.0f, 0.0f, 0.0f), true, true);
	generatePropellants(spacecraft->getPos());
	generatePropulsionForce();
	
}

void ParticleSystem::generatePropellants(Vector3 SpacecraftPos) {
	if (propellerGenerator1 == nullptr || propellerGenerator2 == nullptr) {
		if (!PROPELLER_GEN_RIGIDBODY) {
			propellerModel1 = new Particle(BASIC, PxTransform(SpacecraftPos + Vector3(-2.0f, -5.0f, 0.0f)), Vector3(0.0f, -1.0f, 0.0f), false);
			propellerModel2 = new Particle(BASIC, PxTransform(SpacecraftPos + Vector3(2.0f, -5.0f, 0.0f)), Vector3(0.0f, -1.0f, 0.0f), false);
		}
		else {
			propellerModel1 = new Particle(gPhysics, gScene, BASIC, PxTransform(SpacecraftPos + Vector3(-2.0f, -5.0f, 0.0f)), Vector3(0.0f, -1.0f, 0.0f), false);
			propellerModel2 = new Particle(gPhysics, gScene, BASIC, PxTransform(SpacecraftPos + Vector3(2.0f, -5.0f, 0.0f)), Vector3(0.0f, -1.0f, 0.0f), false);
		}

		if (propellerGenerator1 == nullptr) {
			propellerGenerator1 = new GaussianParticleGenerator("Propeller1", SpacecraftPos + Vector3(-2.0f, -5.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), 0.1, 1,
				propellerModel1, Vector3(0.1f, 0.1f, 0.1f), Vector3(1.0f, 1.0f, 1.0f), true);
			addParticleGenerator(propellerGenerator1);
		}
		if (propellerGenerator2 == nullptr) {
			propellerGenerator2 = new GaussianParticleGenerator("Propeller2", SpacecraftPos + Vector3(2.0f, -5.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), 0.1, 1,
				propellerModel2, Vector3(0.1f, 0.1f, 0.1f), Vector3(1.0f, 1.0f, 1.0f), true);
			addParticleGenerator(propellerGenerator2);
		}
	}
}

void ParticleSystem::generatePropulsionForce() {
	propulsionForceGenerator = new GravityForceGenerator(PROPULSION_FORCE,
		"PropulsionForce", 0.0f);
	addForceGenerator(propulsionForceGenerator);
}

void ParticleSystem::left() {
	spacecraft->setPos(Vector3(spacecraft->getPosX() + SPACECRAFT_MOVEMENT_SPEED, spacecraft->getPosY(), spacecraft->getPosZ()));
}

void ParticleSystem::right() {
	spacecraft->setPos(Vector3(spacecraft->getPosX() - SPACECRAFT_MOVEMENT_SPEED, spacecraft->getPosY(), spacecraft->getPosZ()));
}

void ParticleSystem::addPropulsion() {
	if (propulsionForceGenerator != nullptr) {
		// HAY QUE APLICARLE LA PROPULSION SEGÚN LA DIRECCIÓN A LA QUE ESTÉ LA NAVE
		propulsionForceGenerator->setGravity(propulsionForceGenerator->getGravity() + PROPULSION_FORCE);
		propulsionForceGenerator->setActive(true);
	}
}

void ParticleSystem::stopPropulsion() {
	if (propulsionForceGenerator != nullptr) {
		spacecraft->clearForce();
		propulsionForceGenerator->setGravity(Vector3(0.0f, 0.0f, 0.0f));
		propulsionForceGenerator->setActive(false);
	}
}
#pragma endregion