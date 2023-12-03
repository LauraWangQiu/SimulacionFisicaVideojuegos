#include "ParticleSystem.h"

#define ORIGIN
//#define ADD_CIRCLES
#define ADD_SPHERES

// GENERADORES DE PARTICULAS
#define FIREWORK_MODEL_VISIBLE false
#define FIRE_MODEL_VISIBLE false
#define WATERFALL_MODEL_VISIBLE false
#define STEAM_MODEL_VISIBLE false
#define SQUIRT_MODEL_VISIBLE false
#define WIND_MODEL_VISIBLE false
#define WHIRL_WIND_MODEL_VISIBLE false
#define RANDOM_MODEL_VISIBLE false

// SOLIDOS RIGIDOS
#define CREATE_RIGID_BODIES_SCENE
#define ADD_CIRCLE_RIGIDBODY true
#define ADD_SPHERE_RIGIDBODY true
#define FIREWORK_GEN_RIGIDBODY true
#define FIRE_GEN_RIGIDBODY true
#define WATERFALL_GEN_RIGIDBODY true
#define STEAM_GEN_RIGIDBODY true
#define SQUIRT_GEN_RIGIDBODY true
#define WIND_GEN_RIGIDBODY true
#define WHIRL_WIND_GEN_RIGIDBODY true
#define RANDOM_GEN_RIGIDBODY true

// GENERADORES DE FUERZAS
#define GRAVITY_FORCE1 Vector3(0.0f, -9.8f /*-1.5f*/, 0.0f)
#define GRAVITY_FORCE2 Vector3(0.0f, 12.0f, 0.0f)
#define GRAVITY_FORCE1_DURATION 0.0f
#define GRAVITY_FORCE2_DURATION 0.0f
#define DRAG_FORCE_DURATION 0.0f
#define WIND_FORCE_DURATION 0.0f
#define WIND_NO_K2
//#define WIND_SPECIFIC_SPACE
#define WIND_SIDE
#define WHIRL_WIND_FORCE_DURATION 0.0f
#define EXPLOSION_FORCE_DURATION 0.0f
#define CHANGE_CIRCLE_CENTER
// MUELLES
//#define GENERATE_SPRINGS_DEMO
#define SPRING_FORCE_STATIC_DURATION 0.0f
#define SPRING_FORCE_DYNAMIC_DURATION 0.0f
#define SPRING_FORCE_SLINKY_DURATION 0.0f
#define SLINKY_PARTICLE_MASS 30.0f
#define SLINKY_PARTICLE_K 100.0f
#define SLINKY_PARTICLE_LENGTH 1.0f
// FLOTACIÓN
#define BUOYANCY_FORCE_DURATION 0.0f

ParticleSystem::ParticleSystem(PxPhysics* gPhysics, PxScene* gScene, const Vector3& g) : gravity(g), 
	numParticles(0),
	origin(PxTransform(Vector3(0.0f, 0.0f, 0.0f))),
	gPhysics(gPhysics), gScene(gScene) {

#ifdef ORIGIN
	addOrigin();
#endif // ORIGIN

	// GENERADORES DE PARTICULAS
	generateRandomSystem();

	// GENERADORES DE FUERZAS
	generateGravityForce();
	generateDragForce();
	generateWindForce();
	generateWhirlWindsForce();
	generateExplosionsForce();

#ifdef CREATE_RIGID_BODIES_SCENE
	createScene();
#endif
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

	gravityForceGenerator		= nullptr;
	particleDragForceGenerator	= nullptr;

	windForceGenerator			= nullptr;
	whirlWindsForceGenerator	= nullptr;
	explosionsForceGenerator	= nullptr;
}

void ParticleSystem::update(double t) {
#ifdef ADD_CIRCLES
	static double elapsedTime = 0;
	elapsedTime += t;
	if (elapsedTime >= CIRCLE_INTERVAL) {
		addCircle();
		elapsedTime = 0;
	}
#endif
#ifdef ADD_SPHERES
	static double elapsedTime2 = 0;
	elapsedTime2 += t;
	if (elapsedTime2 >= SPHERE_INTERVAL) {
		addSphere();
		elapsedTime2 = 0;
	}
#endif

	// Recorro todos los generadores de particulas y genero particulas anadiendolas a listOfParticles
	for (auto pg : listOfParticleGenerators) {
		pg->updateActualNumParticles(numParticles);
		if (pg->getActive()) {
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

void ParticleSystem::addParticle(ParticleType Type, PxTransform Transform, Vector3 Dir) {
	Particle* p = new Particle(Type, Transform, Dir);
	addParticle(p);
}

void ParticleSystem::addParticle(PxTransform Transform, Vector3 Dir, float Mass, float Velc, Vector3 Acc, float Damping, Vector3 Size,
	float Time, Vector4 Color, string ShapeName, int NumDivisions, int NumExplodes) {
	Particle* p = new Particle(Transform, Dir, Mass, Velc, Acc, Damping, Size, Time, Color, ShapeName, NumDivisions, NumExplodes);

	p->setRandom();
	addParticle(p);
}

void ParticleSystem::addParticle(PxPhysics* GPhysics, PxScene* GScene, ParticleType Type, PxTransform Transform, Vector3 Dir) {
	Particle* p = new Particle(GPhysics, GScene, Type, Transform, Dir);
	addParticle(p);
}

void ParticleSystem::addParticle(Particle* p) {
	listOfParticles.push_back(p);
	increaseNumParticles();
	updateNumParticlesText();
}

void ParticleSystem::addFirework(PxPhysics* gPhysics, PxScene* gScene, ParticleType Type, PxTransform Transform, Vector3 Dir) {
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

	if (gravityForceGenerator != nullptr && gravityForceGenerator->getActive()) {
		particleForceRegistry.addRegistry(gravityForceGenerator, p);
	}
	if (particleDragForceGenerator != nullptr && particleDragForceGenerator->getActive()) {
		particleForceRegistry.addRegistry(particleDragForceGenerator, p);
	}
	if (windForceGenerator != nullptr && windForceGenerator->getActive()) {
		particleForceRegistry.addRegistry(windForceGenerator, p);
	}
	if (whirlWindsForceGenerator != nullptr && whirlWindsForceGenerator->getActive()) {
		particleForceRegistry.addRegistry(whirlWindsForceGenerator, p);
	}
	if (explosionsForceGenerator != nullptr && explosionsForceGenerator->getActive()) {
		particleForceRegistry.addRegistry(explosionsForceGenerator, p);
	}
}

void ParticleSystem::generateGravityForce() {
	gravityForceGenerator = new GravityForceGenerator(GRAVITY_FORCE1, 
		"GravityForce", GRAVITY_FORCE1_DURATION);
	addForceGenerator(gravityForceGenerator);
}

void ParticleSystem::generateDragForce() {
	particleDragForceGenerator = new ParticleDragForceGenerator(0.2f, 0.0f, "DragForce", DRAG_FORCE_DURATION);
	addForceGenerator(particleDragForceGenerator);
}

void ParticleSystem::generateWindForce() {
	windForceGenerator = new WindForceGenerator(
#ifdef WIND_NO_K2
		5.0f, 0.0f,
#else
		0.0f, 0.05f,
#endif
#ifdef WIND_SIDE
		Vector3(-10.0f, 0.0f, 0.0f),
#else
		Vector3(0.0f, -10.0f, 0.0f),
#endif
#ifdef WIND_SPECIFIC_SPACE
		Vector3(0.0f, -50.0f, 0.0f), Vector3(50.0f, 50.0f, 50.0f),
#endif
		"WindForce", WIND_FORCE_DURATION);
	addForceGenerator(windForceGenerator);
}

void ParticleSystem::generateWhirlWindsForce() {
	whirlWindsForceGenerator = new WhirlWindForceGenerator(7.0f, Vector3(0.0f, 40.0f, 0.0f), 
		"WhirlWindForce", WHIRL_WIND_FORCE_DURATION);
	addForceGenerator(whirlWindsForceGenerator);
}

void ParticleSystem::generateExplosionsForce() {
	explosionOrigin = Vector3(0.0f, 50.0f, 0.0f);
	explosionsForceGenerator = new ExplosionForceGenerator(explosionOrigin, 1.0, 50000.0f, 1000.0f,
		"ExplosionForce", EXPLOSION_FORCE_DURATION);
	addForceGenerator(explosionsForceGenerator);
}
#pragma endregion

#pragma region FIGURAS
void ParticleSystem::addCircle(Vector3 center) {
#ifdef CHANGE_CIRCLE_CENTER
	center = explosionOrigin;
#endif
	int num = min(NUM_PARTICLES_CIRCLE, MAX_PARTICLES - numParticles);
	for (int i = 0; i < num; ++i) {
		float angle = 2.0f * M_PI * i / num;
		float x = center.x + RADIUS_CIRCLE * cos(angle);
		float y = center.y + RADIUS_CIRCLE * sin(angle);
		float z = center.z;
		Vector3 particlePosition(x, y, z);
		Vector3 particleDirection = (center - particlePosition);
		particleDirection.normalize();
		if (!ADD_SPHERE_RIGIDBODY) addParticle(new Particle(BASIC, PxTransform(particlePosition), particleDirection));
		else addParticle(new Particle(gPhysics, gScene, BASIC, PxTransform(particlePosition), particleDirection));
	}
}

void ParticleSystem::addSphere(Vector3 center) {
#ifdef CHANGE_CIRCLE_CENTER
	center = explosionOrigin;
#endif
	int num = min(NUM_PARTICLES_SPHERE, MAX_PARTICLES - numParticles);
	for (int i = 0; i < num; ++i) {
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
		if (!ADD_SPHERE_RIGIDBODY) addParticle(new Particle(BASIC, PxTransform(particlePosition), particleDirection));
		else addParticle(new Particle(gPhysics, gScene, BASIC, PxTransform(particlePosition), particleDirection));
	}
}
#pragma endregion

#pragma region SOLIDOS RIGIDOS
void ParticleSystem::createScene() {
	// Generar suelo
	floor = gPhysics->createRigidStatic(PxTransform(Vector3(0.0f, -20.0f, 0.0f)));
	floorShape = CreateShape(PxBoxGeometry(100, 0.1, 100));
	floor->attachShape(*floorShape);
	gScene->addActor(*floor);
	// Pintar suelo
	floorRI = new RenderItem(floorShape, floor, { 0.8, 0.8, 0.8, 1 });
}
#pragma endregion