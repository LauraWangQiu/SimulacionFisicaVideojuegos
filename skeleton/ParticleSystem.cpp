#include "ParticleSystem.h"

//#define ORIGIN

// GENERADORES DE PARTICULAS
#define FIREWORK_MODEL_VISIBLE false
#define RANDOM_MODEL_VISIBLE false

// SOLIDOS RIGIDOS
#define RANDOM_GEN_RIGIDBODY true
#define PROPELLER_GEN_RIGIDBODY false

#define PROPULSION_FORCE Vector3(0.0f, 50.0f, 0.0f)

ParticleSystem::ParticleSystem(PxPhysics* gPhysics, PxScene* gScene, Camera* camera, const Vector3& g) : gravity(g),
	numParticles(0),
	origin(PxTransform(Vector3(0.0f, 0.0f, 0.0f))),
	gPhysics(gPhysics), gScene(gScene), camera(camera) {

#ifdef ORIGIN
	addOrigin();
#endif // ORIGIN

	createScene();
}

ParticleSystem::~ParticleSystem() {

	listOfParticles.clear();
	listOfParticleGenerators.clear();

	originParticle = nullptr;

	randomGenerator		= nullptr; randomModel		= nullptr;
	propellantGenerator1 = nullptr; propellantModel1	= nullptr;
	propellantGenerator2 = nullptr; propellantModel2	= nullptr;
	
	spacecraft	= nullptr;
	propellant1 = nullptr;
	propellant2 = nullptr;

	for (auto it = listOfForceGenerators.begin(); it != listOfForceGenerators.end(); ++it) {
		delete* it; *it = nullptr;
	}
	listOfForceGenerators.clear();

	propulsionForceGenerator	= nullptr;
}

void ParticleSystem::update(double t) {

	// Camara
	manageMode();

	// Recorro todos los generadores de particulas y genero particulas anadiendolas a listOfParticles
	for (auto pg : listOfParticleGenerators) {
		pg->updateActualNumParticles(numParticles);
		if (pg->getActive()) {

			if (pg->getName() == "Propeller1" || pg->getName() == "Propeller2")
				generatorFollowSpacecraft(pg);

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
				propellantGenerator1->setActive(false);
				propellantGenerator2->setActive(false);
				delete spacecraft;	spacecraft = nullptr;
				delete propellant1; propellant1 = nullptr;
				delete propellant2; propellant2 = nullptr;
				delete window;		window = nullptr;
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

			if (*p == spacecraft) {
				propellantGenerator1->setActive(spacecraft->getPosY() > FLOOR_HEIGHT);
				propellantGenerator2->setActive(spacecraft->getPosY() > FLOOR_HEIGHT);
			}
			else if (*p == propellant1 || *p == propellant2 || *p == window)
				objectFollowSpacecraft(*p);

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
		if (!RANDOM_GEN_RIGIDBODY) randomModel = new Particle(RANDOM, PxTransform(RANDOM_GENERATOR_ORIGIN), Vector3(0.0f, -1.0f, 0.0f), RANDOM_MODEL_VISIBLE);
		else randomModel = new Particle(gPhysics, gScene, RANDOM, PxTransform(RANDOM_GENERATOR_ORIGIN), Vector3(0.0f, -1.0f, 0.0f), RANDOM_MODEL_VISIBLE);

		randomGenerator = new UniformParticleGenerator("Random", RANDOM_GENERATOR_ORIGIN, Vector3(0.0f, 0.0f, 0.0f), 1.0, 1,
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

	// SUELO
	floor = gPhysics->createRigidStatic(PxTransform(Vector3(0.0f, -5.0f, 0.0f)));
	floorShape = CreateShape(PxBoxGeometry(100, 1, 100));
	floor->attachShape(*floorShape);
	gScene->addActor(*floor);
	floorRI = new RenderItem(floorShape, floor, { 0.0, 1.0, 0.0, 1 });

	// NAVE
	spacecraft = addParticle(gPhysics, gScene, SPACECRAFT, origin, Vector3(0.0f, 0.0f, 0.0f), true, true);
	// Bloquear las rotaciones
	 PxRigidDynamicLockFlags flags = 
		  PxRigidDynamicLockFlag::eLOCK_ANGULAR_X 
		| PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y
		| PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z;

	spacecraft->getRigid()->setRigidDynamicLockFlags(flags);
	spacecraft->setColor2(palettes.spacecraftPalette[colorIndex]);

	// Ventana de la nave
	window = addParticle(WINDOW, 
		PxTransform(Vector3(
			spacecraft->getPosX(), 
			spacecraft->getPosY(), 
			spacecraft->getPosZ() - 3.0f)),
		Vector3(0.0f, 0.0f, 0.0f), true, true);

	// Propulsores
	generatePropellants(spacecraft->getPos());
	generatePropulsionForce();

	// GENERADORES DE PARTICULAS
	generateRandomSystem();
	randomGenerator->setActive(true);
	addParticles(randomGenerator->generateParticles());
}

void ParticleSystem::generatePropellants(Vector3 SpacecraftPos) {
	if (propellantGenerator1 == nullptr || propellantGenerator2 == nullptr) {
		if (!PROPELLER_GEN_RIGIDBODY) {
			propellantModel1 = new Particle(BASIC, PxTransform(SpacecraftPos + PROPELLANT1_GENERATOR_POSITION), Vector3(0.0f, -1.0f, 0.0f), false);
			propellantModel2 = new Particle(BASIC, PxTransform(SpacecraftPos + PROPELLANT2_GENERATOR_POSITION), Vector3(0.0f, -1.0f, 0.0f), false);
		}
		else {
			propellantModel1 = new Particle(gPhysics, gScene, BASIC, PxTransform(SpacecraftPos + PROPELLANT1_GENERATOR_POSITION), Vector3(0.0f, -1.0f, 0.0f), false);
			propellantModel2 = new Particle(gPhysics, gScene, BASIC, PxTransform(SpacecraftPos + PROPELLANT2_GENERATOR_POSITION), Vector3(0.0f, -1.0f, 0.0f), false);
		}
		propellant1 = addParticle(PROPELLANT, PxTransform(SpacecraftPos + PROPELLANT1_POSITION), Vector3(0.0f, -1.0f, 0.0f), true, true);

		if (propellantGenerator1 == nullptr) {
			propellantGenerator1 = new GaussianParticleGenerator("Propeller1", SpacecraftPos + PROPELLANT1_GENERATOR_POSITION, Vector3(0.0f, 0.0f, 0.0f), 1, 3,
				propellantModel1, Vector3(0.1f, 0.1f, 0.1f), Vector3(1.0f, 1.0f, 1.0f));
			addParticleGenerator(propellantGenerator1);
		}
		if (propellantGenerator2 == nullptr) {
			propellantGenerator2 = new GaussianParticleGenerator("Propeller2", SpacecraftPos + PROPELLANT2_GENERATOR_POSITION, Vector3(0.0f, 0.0f, 0.0f), 1, 3,
				propellantModel2, Vector3(0.1f, 0.1f, 0.1f), Vector3(1.0f, 1.0f, 1.0f));
			addParticleGenerator(propellantGenerator2);
		}
		propellant2 = addParticle(PROPELLANT, PxTransform(SpacecraftPos + PROPELLANT2_POSITION), Vector3(0.0f, -1.0f, 0.0f), true, true);
	}
}

void ParticleSystem::generatePropulsionForce() {
	propulsionForceGenerator = new GravityForceGenerator(PROPULSION_FORCE,
		"PropulsionForce", 0.0f);
	addForceGenerator(propulsionForceGenerator);
}

void ParticleSystem::left() {
	if (gameMode == NORMAL) spacecraft->setPos(Vector3(spacecraft->getPosX() + SPACECRAFT_MOVEMENT_SPEED, spacecraft->getPosY(), spacecraft->getPosZ()));
}

void ParticleSystem::right() {
	if (gameMode == NORMAL) spacecraft->setPos(Vector3(spacecraft->getPosX() - SPACECRAFT_MOVEMENT_SPEED, spacecraft->getPosY(), spacecraft->getPosZ()));
}

void ParticleSystem::forward() {
	if (gameMode == NORMAL) spacecraft->setPos(Vector3(spacecraft->getPosX(), spacecraft->getPosY(), spacecraft->getPosZ() + SPACECRAFT_MOVEMENT_SPEED));
}

void ParticleSystem::backward() {
	if (gameMode == NORMAL) spacecraft->setPos(Vector3(spacecraft->getPosX(), spacecraft->getPosY(), spacecraft->getPosZ() - SPACECRAFT_MOVEMENT_SPEED));
}

void ParticleSystem::addPropulsion() {
	if (propulsionForceGenerator != nullptr && gameMode == NORMAL) {
		propulsionForceGenerator->setGravity(propulsionForceGenerator->getGravity() + PROPULSION_FORCE);
		propulsionForceGenerator->setActive(true);
	}
}

void ParticleSystem::stopPropulsion() {
	if (propulsionForceGenerator != nullptr && gameMode == NORMAL) {
		spacecraft->clearForce();
		propulsionForceGenerator->setGravity(Vector3(0.0f, 0.0f, 0.0f));
		propulsionForceGenerator->setActive(false);
	}
}

void ParticleSystem::objectFollowSpacecraft(Particle* p) {
	Vector3 offset;
	if (p == propellant1)
		offset = PROPELLANT1_POSITION;
	else if (p == propellant2)
		offset = PROPELLANT2_POSITION;
	else if (p == window)
		offset = WINDOW_POSITION;

	p->setPos(spacecraft->getPos() + offset);
}

void ParticleSystem::generatorFollowSpacecraft(ParticleGenerator* pg) {
	Vector3 offset;
	if (pg == propellantGenerator1)
		offset = PROPELLANT1_GENERATOR_POSITION;
	else if (pg == propellantGenerator2)
		offset = PROPELLANT2_GENERATOR_POSITION;

	pg->getModel()->setPos(spacecraft->getPos() + offset);
}

void ParticleSystem::manageMode() {
	switch (gameMode) {
	case NORMAL: 
		cameraAzimuth = CAMERA_INITIAL_AZIMUTH; 
		cameraFollow(); 
		break;
	case PERSONALIZATION: 
		cameraAzimuth += 0.001f; 
		cameraRotate();
		break;
	default: break;
	}
}

// Personalizacion cohete
void ParticleSystem::leftColor() {
	if (gameMode == PERSONALIZATION) {
		if (--colorIndex < 0) colorIndex = palettes.spacecraftPaletteSize - 1;
		spacecraft->setColor2(palettes.spacecraftPalette[colorIndex % palettes.spacecraftPaletteSize]);
	}
}

void ParticleSystem::rightColor() {
	if (gameMode == PERSONALIZATION) {
		spacecraft->setColor2(palettes.spacecraftPalette[++colorIndex % palettes.spacecraftPaletteSize]);
	}
}

void ParticleSystem::switchMode(){
	gameMode = (gameMode + 1) % GAME_MODES_SIZE;

	switch (gameMode) {
	case NORMAL:
		menu = false;
		cameraFollow(); 
		stopMotion(false);
		break;
	case PERSONALIZATION:
		menu = true;
		cameraRotate();
		stopMotion(true);
		break;
	default: break;
	}
}

// Camara
void ParticleSystem::cameraRotate() {
	// Calcular la posición de la cámara usando coordenadas esféricas
	float x = cameraRadius * cos(cameraElevation) * cos(cameraAzimuth);
	float y = cameraRadius * sin(cameraElevation);
	float z = cameraRadius * cos(cameraElevation) * sin(cameraAzimuth);
	Vector3 dir = Vector3(x, y, z);
	camera->setView(dir + spacecraft->getPos(), (-dir).getNormalized());
}

void ParticleSystem::cameraFollow() {
	camera->setView(spacecraft->getPos() + CAMERA_POSITION, CAMERA_VIEW);
}

void ParticleSystem::stopMotion(bool m) {
	spacecraft->getRigid()->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, m);
}
#pragma endregion