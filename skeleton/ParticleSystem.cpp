#include "ParticleSystem.h"

//#define ORIGIN

// GENERADORES DE PARTICULAS
#define FIREWORK_MODEL_VISIBLE false
#define RANDOM_MODEL_VISIBLE false

// GENERADORES DE FUERZAS
#define WHIRL_WIND_FORCE_DURATION 0.0f
#define SPRING_FORCE_STATIC_DURATION 0.0f
#define BUOYANCY_FORCE_DURATION 0.0f

// SOLIDOS RIGIDOS
#define RANDOM_GEN_RIGIDBODY true
#define FIREWORK_GEN_RIGIDBODY false
#define PROPELLER_GEN_RIGIDBODY false
#define ADD_SPHERE_RIGIDBODY true

ParticleSystem::ParticleSystem(PxPhysics* gPhysics, PxScene* gScene, Camera* camera, const Vector3& g) : gravity(g),
	numParticles(0),
	origin(PxTransform(VECTOR_ZERO)),
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

	randomGenerator		 = nullptr;	randomModel			= nullptr;
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
	whirlWindsForceGenerator	= nullptr;
	buoyancyForceGenerator		= nullptr;
}

void ParticleSystem::update(double t) {

	if (gameMode == NORMAL || gameMode == END || gameMode == END2) inGameTime += t;

	manageMode();

	// Recorro todos los generadores de particulas y genero particulas anadiendolas a listOfParticles
	for (auto pg : listOfParticleGenerators) {
		pg->updateActualNumParticles(numParticles);
		if (pg->getActive()) {

			if (pg->getName() == "Propellant1" || pg->getName() == "Propellant2")
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
			onParticleDeath(*p);
			(*p)->setDelete(true);
			(*p)->release();
			particleForceRegistry.deleteParticleRegistry(*p);
			p = listOfParticles.erase(p);
			decreaseNumParticles();
		}
		else {
			if (*p == spacecraft && spacecraft != nullptr && floorRI != nullptr)
				setActivePropellants(true);
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
	case FIREWORK2:
		p->setToExplode(true);
		addParticles(static_cast<Firework*>(p)->explode(numParticles));
		break;
	}
}

#pragma region LISTAS
void ParticleSystem::addGeneratorName(string name) {
	listOfGeneratorNames.push_back(name);
}

void ParticleSystem::clearListOfParticles() {
	for (auto p : listOfParticles) {
		p->setDelete(true);
		p->release();
		particleForceRegistry.deleteParticleRegistry(p);
		decreaseNumParticles();
	} listOfParticles.clear();
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
	originParticle = new Particle(BASIC, origin, VECTOR_ZERO, true, true);
}
#pragma endregion

#pragma region PARTICULAS
void ParticleSystem::addParticles(list<Particle*> list) {
	for (auto p : list) {
		listOfParticles.push_back(p);
	}
	updateNumParticles(list.size());
}

Particle* ParticleSystem::addParticle(ParticleType Type, PxTransform Transform, Vector3 Dir, bool Visible, bool Active) {
	Particle* p = new Particle(Type, Transform, Dir, Visible, Active);
	addParticle(p);
	return p;
}

Particle* ParticleSystem::addParticle(PxTransform Transform, 
	Vector3 Dir, float Mass, float Velc, Vector3 Acc, 
	float Damping, Vector3 Size,
	float Time, Vector4 Color, string ShapeName, 
	int NumDivisions, int NumExplodes, 
	float Density, Vector3 MassInertiaTensor,
	bool Visible, bool Active) {

	Particle* p = new Particle(Transform, Dir, 
		Mass, Velc, Acc, Damping, Size, 
		Time, Color, ShapeName, 
		NumDivisions, NumExplodes, 
		Density, MassInertiaTensor, 
		Visible, Active);

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
		model, VECTOR_ZERO, Vector3(1.0f, 1.0f, 1.0f));
	p->addGenerator(g);

	return p;
}
#pragma endregion

#pragma region GENERADORES DE PARTICULAS
void ParticleSystem::generateRandomSystem() {
	if (randomGenerator == nullptr) {
		if (!RANDOM_GEN_RIGIDBODY) randomModel = new Particle(RANDOM, PxTransform(RANDOM_GENERATOR_ORIGIN), Vector3(0.0f, -1.0f, 0.0f), RANDOM_MODEL_VISIBLE);
		else randomModel = new Particle(gPhysics, gScene, RANDOM, PxTransform(RANDOM_GENERATOR_ORIGIN), Vector3(0.0f, -1.0f, 0.0f), RANDOM_MODEL_VISIBLE);

		randomGenerator = new UniformParticleGenerator("Random", RANDOM_GENERATOR_ORIGIN, VECTOR_ZERO, 1.0, 1,
			randomModel, Vector3(1.0f, 1.0f, 1.0f), Vector3(1.0f, 1.0f, 1.0f));
		addParticleGenerator(randomGenerator);
	}
}

void ParticleSystem::generateFireworkSystem() {
	if (fireworkGenerator == nullptr) {
		if (!FIREWORK_GEN_RIGIDBODY) fireworkModel = new Firework(FIREWORK2, PxTransform(FIREWORK_GENERATOR_ORIGIN), Vector3(0.0f, 1.0f, 0.0f), FIREWORK_MODEL_VISIBLE);
		else fireworkModel = new Firework(gPhysics, gScene, FIREWORK2, PxTransform(FIREWORK_GENERATOR_ORIGIN), Vector3(0.0f, 1.0f, 0.0f), FIREWORK_MODEL_VISIBLE);

		fireworkGenerator = new GaussianParticleGenerator("Fireworks", FIREWORK_GENERATOR_ORIGIN, VECTOR_ZERO, 0.1, 1,
			fireworkModel, VECTOR_ZERO, Vector3(10.0f, 10.0f, 10.0f));
		addParticleGenerator(fireworkGenerator);
	}
}
#pragma endregion

#pragma region GENERADORES DE FUERZAS
void ParticleSystem::addForces(Particle* p) {
	if (propulsionForceGenerator != nullptr && propulsionForceGenerator->getActive()) {
		if (p == spacecraft)
			particleForceRegistry.addRegistry(propulsionForceGenerator, p);
	}

	if (whirlWindsForceGenerator != nullptr && whirlWindsForceGenerator->getActive()) {
		if (p->getParticleType() == RANDOM)
			particleForceRegistry.addRegistry(whirlWindsForceGenerator, p);
	}

	if (buoyancyForceGenerator != nullptr && buoyancyForceGenerator->getActive()) {
		if (p == spacecraft || p->getParticleType() == RANDOM)
			particleForceRegistry.addRegistry(buoyancyForceGenerator, p);
	}
}

void ParticleSystem::generatePropulsionForce() {
	propulsionForceGenerator = new GravityForceGenerator(PROPULSION_FORCE,
		"PropulsionForce", 0.0f);
	addForceGenerator(propulsionForceGenerator);
}

void ParticleSystem::generateWhirlWindsForce() {
	whirlWindsForceGenerator = new WhirlWindForceGenerator(7.0f, center - Vector3(0.0f, 50.0f, 0.0f),
		"WhirlWindForce", WHIRL_WIND_FORCE_DURATION);
	addForceGenerator(whirlWindsForceGenerator);
}

void ParticleSystem::generateBuoyancyForce() {
	if (liquidModel == nullptr) {
		liquidModel = new Particle(WATER_PLANE, PxTransform(WATER_PLANE_POSITION), VECTOR_ZERO, true, true);
		float volume = spacecraft->getSize().x * spacecraft->getSize().y * spacecraft->getSize().z;
		buoyancyForceGenerator = new BuoyancyForceGenerator(liquidModel, 15.0f, volume, 1000.0f, gravity.y, "BuoyancyForce", BUOYANCY_FORCE_DURATION);
		addForceGenerator(buoyancyForceGenerator);
	}
}
#pragma endregion

#pragma region JUEGO
void ParticleSystem::createScene() {

	// SUELO
	floor = gPhysics->createRigidStatic(PxTransform(Vector3(0.0f, -5.0f, 0.0f)));
	floor->setName("FLOOR");
	floorShape = CreateShape(PxBoxGeometry(100, 1, 100));
	floor->attachShape(*floorShape);
	gScene->addActor(*floor);
	floorRI = new RenderItem(floorShape, floor, { 1.0, 1.0, 1.0, 1.0 });

	// NAVE
	createSpacecraft();

	// GENERADORES DE PARTICULAS
	generateRandomSystem();
	randomGenerator->setActive(true);
	addParticles(randomGenerator->generateParticles());

	generateFireworkSystem();
	generateWhirlWindsForce();
	generateBuoyancyForce();

	stopMotion(true);
}

void ParticleSystem::stopMotion(bool m) {
	for (auto p : listOfParticles) {
		if (p->isRigid()) p->getRigid()->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, m);
		if (p->getParticleType() == NONE)
			p->setActive(m);
	}
	randomGenerator->setActive(!m);
}

void ParticleSystem::addSphere(bool random) {

	for (int i = 0; i < NUM_PARTICLES_SPHERE; ++i) {
		float latitud = ((float)rand() / RAND_MAX) * 360.0f;
		float longitud = ((float)rand() / RAND_MAX) * 360.0f;

		float latitudRad = latitud * M_PI / 180.0f;
		float longitudRad = longitud * M_PI / 180.0f;

		float x = RADIUS_SPHERE * std::sin(latitudRad) * std::cos(longitudRad) + center.x;
		float y = RADIUS_SPHERE * std::sin(latitudRad) * std::sin(longitudRad) + center.y;
		float z = RADIUS_SPHERE * std::cos(latitudRad) + center.z;

		Vector3 particlePosition(x, y, z);
		Vector3 particleDirection = (center - particlePosition);
		particleDirection.normalize();

		Particle* p = nullptr;
		if (!ADD_SPHERE_RIGIDBODY) p = addParticle(RANDOM, PxTransform(particlePosition), particleDirection, true, true);
		else p = addParticle(gPhysics, gScene, RANDOM, PxTransform(particlePosition), particleDirection, true, true);
		if (random) p->setRandomColor();
	}
}

void ParticleSystem::manageMode() {

	if (inGameTime >= gameTime) {
		if (inGameTime >= (gameTime + END_TIME)) {
			if (inGameTime >= (gameTime + END_TIME2)) gameMode = END3;
			else gameMode = END2;
		} else gameMode = END;
	}

	stopped = gameMode == PERSONALIZATION || gameMode == END;

	static int c = 0;

	switch (gameMode) {
	case NORMAL:
		cameraAzimuth = CAMERA_INITIAL_AZIMUTH;
		cameraFollow();
		break;
	case PERSONALIZATION:
		cameraAzimuth += 0.001f;
		cameraRotate();
		if (stopped) stopMotion(true);
		break;
	case END:
		if (!end) {
			end = true;
			randomGenerator->setActive(false);
			fireworkGenerator->setActive(true);
			spacecraft->setPos(SPACECRAFT_END_POSITION);
		}
		cameraAzimuth += 0.001f;
		cameraRotate();
		break;
	case END2:
		camera->setView(CAMERA_FINAL_POSITION, CAMERA_VIEW);
		++c;
		if (!end2 && c > 50) {
			end2 = true;
			fireworkGenerator->setActive(false);
			spacecraft->setPos(SPACECRAFT_END2_POSITION);
			addSphere();
			whirlWindsForceGenerator->setActive(true);
			propulsionForceGenerator->setActive(true);
			propulsionForceGenerator->setGravity(PROPULSION_END2_FORCE);
			c = 0;
		}
		break;
	case END3:
		cameraAzimuth = CAMERA_INITIAL_AZIMUTH;
		cameraFollow();
		if (!end) ++c;
		if (!end3 && c > 50) {
			end3 = true;
			spacecraft->setPos(SPACECRAFT_END3_POSITION);
			whirlWindsForceGenerator->setActive(false);
			spacecraft->clearForce();
			spacecraft->getRigid()->setLinearVelocity(VECTOR_ZERO);
			propulsionForceGenerator->setGravity(VECTOR_ZERO);
			propulsionForceGenerator->setActive(false);
			buoyancyForceGenerator->setActive(true);
			auto p = listOfParticles.begin();
			while (p != listOfParticles.end()) {
				if ((*p)->getParticleType() == RANDOM || 
					(*p)->getParticleType() == NONE || 
					(*p)->getParticleType() == FIREWORK ||
					(*p)->getParticleType() == BASIC)
					p = listOfParticles.erase(p);
				else ++p;
			}
			center = SPHERE_END3_POSITION;
			addSphere(true);
			c = 0;
		}
		break;
	default: break;
	}
}

void ParticleSystem::switchMode() {
	if (gameMode == NORMAL) gameMode = PERSONALIZATION;
	else if (gameMode == PERSONALIZATION) gameMode = NORMAL;

	switch (gameMode) {
	case NORMAL:
		cameraFollow();
		stopMotion(false);
		break;
	case PERSONALIZATION:
		cameraRotate();
		stopMotion(true);
		break;
	default: break;
	}
}

#pragma region NAVE
void ParticleSystem::createSpacecraft() {
	
	spacecraft = addParticle(gPhysics, gScene, SPACECRAFT, origin, VECTOR_ZERO, true, true);
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
		VECTOR_ZERO, true, true);

	// Propulsores
	createPropellants(spacecraft->getPos());
	generatePropulsionForce();
}

void ParticleSystem::deleteSpacecraft() {
	spacecraft->setActive(false);
	spacecraft->setTime(0);
	window->setActive(false);
	window->setTime(0);
	propellant1->setActive(false);
	propellant1->setTime(0);
	propellant2->setActive(false);
	propellant2->setTime(0);
	spacecraft = nullptr;
	window = nullptr;
	propellant1 = nullptr;
	propellant2 = nullptr;
	propellantGenerator1->setActive(false);
	propellantGenerator2->setActive(false);
}

void ParticleSystem::createPropellants(Vector3 SpacecraftPos) {
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
			propellantGenerator1 = new GaussianParticleGenerator("Propellant1", SpacecraftPos + PROPELLANT1_GENERATOR_POSITION, VECTOR_ZERO, 1, 3,
				propellantModel1, Vector3(0.1f, 0.1f, 0.1f), Vector3(1.0f, 1.0f, 1.0f));
			addParticleGenerator(propellantGenerator1);
		}
		if (propellantGenerator2 == nullptr) {
			propellantGenerator2 = new GaussianParticleGenerator("Propellant2", SpacecraftPos + PROPELLANT2_GENERATOR_POSITION, VECTOR_ZERO, 1, 3,
				propellantModel2, Vector3(0.1f, 0.1f, 0.1f), Vector3(1.0f, 1.0f, 1.0f));
			addParticleGenerator(propellantGenerator2);
		}
		propellant2 = addParticle(PROPELLANT, PxTransform(SpacecraftPos + PROPELLANT2_POSITION), Vector3(0.0f, -1.0f, 0.0f), true, true);
	}
}

#pragma region MOVIMIENTO
void ParticleSystem::left() {
	if (spacecraft != nullptr && gameMode == NORMAL) spacecraft->setPos(Vector3(spacecraft->getPosX() + SPACECRAFT_MOVEMENT_SPEED, spacecraft->getPosY(), spacecraft->getPosZ()));
}
void ParticleSystem::right() {
	if (spacecraft != nullptr && gameMode == NORMAL) spacecraft->setPos(Vector3(spacecraft->getPosX() - SPACECRAFT_MOVEMENT_SPEED, spacecraft->getPosY(), spacecraft->getPosZ()));
}
void ParticleSystem::forward() {
	if (spacecraft != nullptr && gameMode == NORMAL) spacecraft->setPos(Vector3(spacecraft->getPosX(), spacecraft->getPosY(), spacecraft->getPosZ() + SPACECRAFT_MOVEMENT_SPEED));
}
void ParticleSystem::backward() {
	if (spacecraft != nullptr && gameMode == NORMAL) spacecraft->setPos(Vector3(spacecraft->getPosX(), spacecraft->getPosY(), spacecraft->getPosZ() - SPACECRAFT_MOVEMENT_SPEED));
}
void ParticleSystem::addPropulsion() {
	if (propulsionForceGenerator != nullptr && gameMode == NORMAL) {
		propulsionForceGenerator->setGravity(propulsionForceGenerator->getGravity() + PROPULSION_FORCE);
		propulsionForceGenerator->setActive(true);
	}
}
void ParticleSystem::stopPropulsion() {
	if (propulsionForceGenerator != nullptr && spacecraft != nullptr && gameMode == NORMAL) {
		spacecraft->clearForce();
		propulsionForceGenerator->setGravity(VECTOR_ZERO);
		propulsionForceGenerator->setActive(false);
	}
}
#pragma endregion

void ParticleSystem::shoot() {
	if (spacecraft != nullptr)
		addFirework(gPhysics, gScene, FIREWORK, PxTransform(spacecraft->getPos()));
}

#pragma region PERSONALIZACION
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
#pragma endregion
#pragma endregion

#pragma region SEGUIMIENTO DE LA NAVE
void ParticleSystem::objectFollowSpacecraft(Particle* p) {

	if (spacecraft != nullptr) {
		Vector3 offset;
		if (p == propellant1)
			offset = PROPELLANT1_POSITION;
		else if (p == propellant2)
			offset = PROPELLANT2_POSITION;
		else if (p == window)
			offset = WINDOW_POSITION;

		p->setPos(spacecraft->getPos() + offset);
	}
}
void ParticleSystem::generatorFollowSpacecraft(ParticleGenerator* pg) {

	if (spacecraft != nullptr) {
		Vector3 offset;
		if (pg == propellantGenerator1)
			offset = PROPELLANT1_GENERATOR_POSITION;
		else if (pg == propellantGenerator2)
			offset = PROPELLANT2_GENERATOR_POSITION;

		pg->getModel()->setPos(spacecraft->getPos() + offset);
	}
}
#pragma endregion

#pragma region CAMARA
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
#pragma endregion
#pragma endregion