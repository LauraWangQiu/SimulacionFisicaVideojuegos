#include "ParticleSystem.h"

#define ORIGIN

// GENERADORES DE PARTICULAS
//#define ADD_CIRCLES
//#define ADD_SPHERES
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
	generateFireworkSystem();
	generateFireSystem();
	generateWaterfallSystem();
	generateSteamSystem();
	generateSquirtSystem();
	generateWindSystem();
	generateWhirlWindSystem();
	generateRandomSystem();

	// GENERADORES DE FUERZAS
	generateGravityForce();
	generateGravityForce2();
	generateDragForce();
	generateWindForce();
	generateWhirlWindsForce();
	generateExplosionsForce();
#ifdef GENERATE_SPRINGS_DEMO
	// MUELLES
	generateSpringForce();
	generateSpringDemo();
	generateSpringSlinky();
	// FLOTACIÓN
	generateBuoyancyForce();
#endif

#ifdef CREATE_RIGID_BODIES_SCENE
	createScene();
#endif
}

ParticleSystem::~ParticleSystem() {

	listOfParticles.clear();
	listOfParticleGenerators.clear();

	originParticle		= nullptr;

	fireworkGenerator	= nullptr;	fireworkModel	= nullptr;
	fireGenerator		= nullptr;	fireModel		= nullptr;
	waterfallGenerator	= nullptr;	waterfallModel	= nullptr;
	steamGenerator		= nullptr;	steamModel		= nullptr;
	squirtGenerator		= nullptr;	squirtModel		= nullptr;
	windGenerator		= nullptr;	windModel		= nullptr;
	whirlWindGenerator	= nullptr;	whirlWindModel	= nullptr;

	for (auto it = listOfForceGenerators.begin(); it != listOfForceGenerators.end(); ++it) {
		delete* it; *it = nullptr;
	}
	listOfForceGenerators.clear();

	gravityForceGenerator		= nullptr;
	gravityForceGenerator2		= nullptr;
	particleDragForceGenerator	= nullptr;

	windForceGenerator			= nullptr;
	whirlWindsForceGenerator	= nullptr;
	explosionsForceGenerator	= nullptr;

	springForceGenerator		= nullptr;	
	springModel					= nullptr;
	springForceGeneratorPair1	= nullptr;
	springForceGeneratorPair2	= nullptr;
	springForceGenerator1		= nullptr;
	springForceGenerator2		= nullptr;
	springForceGenerator3		= nullptr;
	springForceGenerator4		= nullptr;
	springForceGenerator5		= nullptr;
	springForceGenerator6		= nullptr;

	buoyancyForceGenerator		= nullptr; 
	liquidModel					= nullptr;
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
	case FIREWORK: case FIREWORK2: case FIREWORK3:
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

		if (!ADD_SPHERE_RIGIDBODY) addParticle(new Particle(EXPLOSION, PxTransform(particlePosition), particleDirection));
		else addParticle(new Particle(gPhysics, gScene, EXPLOSION, PxTransform(particlePosition), particleDirection));
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

		if (!ADD_SPHERE_RIGIDBODY) addParticle(new Particle(EXPLOSION, PxTransform(particlePosition), particleDirection));
		else addParticle(new Particle(gPhysics, gScene, EXPLOSION, PxTransform(particlePosition), particleDirection));
	}
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
	if (gPhysics == nullptr || gScene == nullptr) model = new Firework(Type, Transform, Dir, FIREWORK_MODEL_VISIBLE);
	else model = new Firework(gPhysics, gScene, Type, Transform, Dir, FIREWORK_MODEL_VISIBLE);

	Firework* p = nullptr;
	if (gPhysics == nullptr || gScene == nullptr) p = new Firework(Type, Transform, Dir);
	else p = new Firework(gPhysics, gScene, Type, Transform, Dir);

	addParticle(p);
	GaussianParticleGenerator* g = new GaussianParticleGenerator("Firework", Transform.p, Dir, 0.1, 1,
		model, Vector3(0.0f, 0.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f));
	p->addGenerator(g);
}
#pragma endregion

#pragma region GENERADORES DE PARTICULAS
void ParticleSystem::generateFireworkSystem() {
	if (fireworkGenerator == nullptr) {
		if (!FIREWORK_GEN_RIGIDBODY) fireworkModel = new Firework(FIREWORK, origin, Vector3(0.0f, 1.0f, 0.0f), FIREWORK_MODEL_VISIBLE);
		else fireworkModel = new Firework(gPhysics, gScene, FIREWORK, origin, Vector3(0.0f, 1.0f, 0.0f), FIREWORK_MODEL_VISIBLE);

		fireworkGenerator = new GaussianParticleGenerator("Fireworks", origin.p, Vector3(0.0f, 0.0f, 0.0f), 0.1, 1,
			fireworkModel, Vector3(0.0f, 0.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f));
		addParticleGenerator(fireworkGenerator);
	}
}

void ParticleSystem::generateFireSystem() {
	if (fireGenerator == nullptr) {
		if (!FIRE_GEN_RIGIDBODY) fireModel = new Particle(FIRE, origin, Vector3(0.0f, 1.0f, 0.0f), FIRE_MODEL_VISIBLE);
		else fireModel = new Particle(gPhysics, gScene, FIRE, origin, Vector3(0.0f, 1.0f, 0.0f), FIRE_MODEL_VISIBLE);

		fireGenerator = new GaussianParticleGenerator("Fire", origin.p, Vector3(0.0f, 0.0f, 0.0f), 1, 1,
			fireModel, Vector3(2.0f, 2.0f, 2.0f), Vector3(1.0f, 1.0f, 1.0f));
		addParticleGenerator(fireGenerator);
	}
}

void ParticleSystem::generateWaterfallSystem() {
	if (waterfallGenerator == nullptr) {
		if (!WATERFALL_GEN_RIGIDBODY) waterfallModel = new Particle(WATER, origin, Vector3(0.0f, -1.0f, 0.0f), WATERFALL_MODEL_VISIBLE);
		else waterfallModel = new Particle(gPhysics, gScene, WATER, origin, Vector3(0.0f, -1.0f, 0.0f), WATERFALL_MODEL_VISIBLE);

		waterfallGenerator = new UniformParticleGenerator("Waterfall", origin.p, Vector3(0.0f, 0.0f, 0.0f), 0.3, 1,
			waterfallModel, Vector3(0.0f, 0.0f, 0.0f), Vector3(50.0f, 0.0f, 0.0f));
		addParticleGenerator(waterfallGenerator);
	}
}

void ParticleSystem::generateSteamSystem() {
	if (steamGenerator == nullptr) {
		if (!STEAM_GEN_RIGIDBODY) steamModel = new Particle(STEAM, origin, Vector3(0.0f, 1.0f, 0.0f), STEAM_MODEL_VISIBLE);
		else steamModel = new Particle(gPhysics, gScene, STEAM, origin, Vector3(0.0f, 1.0f, 0.0f), STEAM_MODEL_VISIBLE);

		steamGenerator = new GaussianParticleGenerator("Steam", origin.p, Vector3(0.0f, 0.0f, 0.0f), 0.1, 1,
			steamModel, Vector3(10.0f, 10.0f, 10.0f), Vector3(0.0f, 0.0f, 0.0f));
		addParticleGenerator(steamGenerator);
	}
}

void ParticleSystem::generateSquirtSystem() {
	if (squirtGenerator == nullptr) {
		if (!SQUIRT_GEN_RIGIDBODY) squirtModel = new Particle(WATER, origin, Vector3(0.3f, SQUIRT_INITIAL_VEL, 0.3f), SQUIRT_MODEL_VISIBLE);
		else squirtModel = new Particle(gPhysics, gScene, WATER, origin, Vector3(0.3f, SQUIRT_INITIAL_VEL, 0.3f), SQUIRT_MODEL_VISIBLE);

		squirtGenerator = new GaussianParticleGenerator("Squirt", origin.p, Vector3(0.0f, 0.0f, 0.0f), 1, 50,
			squirtModel, Vector3(1.0f, 1.0f, 1.0f), Vector3(1.0f, 1.0f, 1.0f));
		addParticleGenerator(squirtGenerator);
	}
}

void ParticleSystem::generateWindSystem() {
	if (windGenerator == nullptr) {
		if (!WIND_GEN_RIGIDBODY) windModel = new Particle(WIND, origin, Vector3(0.0f, -1.0f, 0.0f), WIND_MODEL_VISIBLE);
		else windModel = new Particle(gPhysics, gScene, WIND, origin, Vector3(0.0f, -1.0f, 0.0f), WIND_MODEL_VISIBLE);

		windGenerator = new UniformParticleGenerator("Wind", origin.p, Vector3(0.0f, 0.0f, 0.0f), 0.5, 1,
			windModel, Vector3(0.0f, 0.0f, 0.0f), Vector3(50.0f, 0.0f, 50.0f));
		addParticleGenerator(windGenerator);
	}
}

void ParticleSystem::generateWhirlWindSystem() {
	if (whirlWindGenerator == nullptr) {
		if (!WHIRL_WIND_GEN_RIGIDBODY) whirlWindModel = new Particle(WIND, origin, Vector3(0.0f, 1.0f, 0.0f), WHIRL_WIND_MODEL_VISIBLE);
		else whirlWindModel = new Particle(gPhysics, gScene, WIND, origin, Vector3(0.0f, 1.0f, 0.0f), WHIRL_WIND_MODEL_VISIBLE);

		whirlWindGenerator = new GaussianParticleGenerator("WhirlWind", origin.p, Vector3(0.0f, 0.0f, 0.0f), 1, 1,
			whirlWindModel, Vector3(1.0f, 1.0f, 1.0f), Vector3(1.0f, 1.0f, 1.0f));
		addParticleGenerator(whirlWindGenerator);
	}
}

void ParticleSystem::generateRandomSystem() {
	if (randomGenerator == nullptr) {
		if (!RANDOM_GEN_RIGIDBODY) randomModel = new Particle(RANDOM, origin, Vector3(0.0f, 1.0f, 0.0f), RANDOM_MODEL_VISIBLE);
		else randomModel = new Particle(gPhysics, gScene, RANDOM, origin, Vector3(0.0f, 1.0f, 0.0f), RANDOM_MODEL_VISIBLE);

		randomGenerator = new GaussianParticleGenerator("Random", origin.p, Vector3(0.0f, 0.0f, 0.0f), 0.1, 1,
			randomModel, Vector3(1.0f, 1.0f, 1.0f), Vector3(1.0f, 1.0f, 1.0f), true);
		addParticleGenerator(randomGenerator);
	}
}
#pragma endregion

#pragma region GENERADORES DE FUERZAS
void ParticleSystem::addForces(Particle* p) {

	if (gravityForceGenerator != nullptr && gravityForceGenerator->getActive()) {
		particleForceRegistry.addRegistry(gravityForceGenerator, p);
	}
	if (gravityForceGenerator2 != nullptr && gravityForceGenerator2->getActive()) {
		particleForceRegistry.addRegistry(gravityForceGenerator2, p);
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
	if (springForceGenerator != nullptr && springForceGenerator->getActive()) {
		switch (p->getParticleType()) {
		case SPRING_STATIC: particleForceRegistry.addRegistry(springForceGenerator, p); break;
		}
	}
	if (buoyancyForceGenerator != nullptr && buoyancyForceGenerator->getActive()) {
		switch (p->getParticleType()) {
		case IMMERSE: particleForceRegistry.addRegistry(buoyancyForceGenerator, p); break;
		}
	}
}

void ParticleSystem::generateGravityForce() {
	gravityForceGenerator = new GravityForceGenerator(GRAVITY_FORCE1, 
		"GravityForce1", GRAVITY_FORCE1_DURATION);
	addForceGenerator(gravityForceGenerator);
}

void ParticleSystem::generateGravityForce2() {
	gravityForceGenerator2 = new GravityForceGenerator(GRAVITY_FORCE2, 
		"GravityForce2", GRAVITY_FORCE2_DURATION);
	addForceGenerator(gravityForceGenerator2);
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

#pragma region MUELLES
void ParticleSystem::generateSpringForce() {
	if (springModel == nullptr) {
		springModel = new Particle(SPRING_BASE, PxTransform(Vector3(-30.0f, 20.0f, 0.0f)), Vector3(0.0f, 0.0f, 0.0f));
		Particle* other = new Particle(SPRING_STATIC, PxTransform(Vector3(-30.0f, 20.0f, 0.0f)), Vector3(0.0f, 0.0f, 0.0f), true, true);
		other->setMass(1.0f);
		addParticle(other);
		springForceGenerator = new SpringForceGenerator(1.0f, 1.0f, springModel, "SpringForce", SPRING_FORCE_STATIC_DURATION);
		addForceGenerator(springForceGenerator);
	}
}

void ParticleSystem::generateSpringDemo() {
	Particle* p1 = new Particle(SPRING_DYNAMIC, PxTransform(Vector3(10.0f, -20.0f, 0.0f)), Vector3(0.0f, 0.0f, 0.0f), true, true);
	Particle* p2 = new Particle(SPRING_DYNAMIC, PxTransform(Vector3(-10.0f, -20.0f, 0.0f)), Vector3(0.0f, 0.0f, 0.0f), true, true);
	p1->setMass(1.0f);
	p2->setMass(2.0f);
	springForceGeneratorPair1 = new SpringForceGenerator(1, 10, p2, "SpringForcePair1", SPRING_FORCE_DYNAMIC_DURATION);
	springForceGeneratorPair2 = new SpringForceGenerator(1, 10, p1, "SpringForcePair2", SPRING_FORCE_DYNAMIC_DURATION);
	addForceGenerator(springForceGeneratorPair1);
	addForceGenerator(springForceGeneratorPair2);
	particleForceRegistry.addRegistry(springForceGeneratorPair1, p1);
	particleForceRegistry.addRegistry(springForceGeneratorPair2, p2);
	addParticle(p1);
	addParticle(p2);
}

void ParticleSystem::generateSpringSlinky() {
	Particle* p1 = new Particle(SLINKY, PxTransform(Vector3(25.0f, 15.0f, 0.0f)), Vector3(0.0f, 0.0f, 0.0f), true, true);
	Particle* p2 = new Particle(SLINKY, PxTransform(Vector3(25.0f, 10.0f, 0.0f)), Vector3(0.0f, 0.0f, 0.0f), true, true);
	Particle* p3 = new Particle(SLINKY, PxTransform(Vector3(25.0f, 5.0f, 0.0f)), Vector3(0.0f, 0.0f, 0.0f), true, true);
	Particle* p4 = new Particle(SLINKY, PxTransform(Vector3(25.0f, 0.0f, 0.0f)), Vector3(0.0f, 0.0f, 0.0f), true, true);
	Particle* p5 = new Particle(SLINKY, PxTransform(Vector3(25.0f, -5.0f, 0.0f)), Vector3(0.0f, 0.0f, 0.0f), true, true);
	Particle* p6 = new Particle(SLINKY, PxTransform(Vector3(25.0f, -10.0f, 0.0f)), Vector3(0.0f, 0.0f, 0.0f), true, true);
	p1->setMass(SLINKY_PARTICLE_MASS);
	p2->setMass(SLINKY_PARTICLE_MASS);
	p3->setMass(SLINKY_PARTICLE_MASS);
	p4->setMass(SLINKY_PARTICLE_MASS);
	p5->setMass(SLINKY_PARTICLE_MASS);
	p6->setMass(SLINKY_PARTICLE_MASS);
	p1->setColor2(1.0f, 0.0f, 0.0f);
	p2->setColor2(1.0f, 0.5f, 0.0f);
	p3->setColor2(1.0f, 1.0f, 0.0f);
	p4->setColor2(0.0f, 1.0f, 0.0f);
	p5->setColor2(0.0f, 1.0f, 1.0f);
	p6->setColor2(0.0f, 0.0f, 1.0f);
	springForceGenerator1 = new SpringForceGenerator(SLINKY_PARTICLE_K, SLINKY_PARTICLE_LENGTH, p1, "SpringForce1", SPRING_FORCE_SLINKY_DURATION);
	springForceGenerator2 = new SpringForceGenerator(SLINKY_PARTICLE_K, SLINKY_PARTICLE_LENGTH, p2, "SpringForce2", SPRING_FORCE_SLINKY_DURATION);
	springForceGenerator3 = new SpringForceGenerator(SLINKY_PARTICLE_K, SLINKY_PARTICLE_LENGTH, p3, "SpringForce3", SPRING_FORCE_SLINKY_DURATION);
	springForceGenerator4 = new SpringForceGenerator(SLINKY_PARTICLE_K, SLINKY_PARTICLE_LENGTH, p4, "SpringForce4", SPRING_FORCE_SLINKY_DURATION);
	springForceGenerator5 = new SpringForceGenerator(SLINKY_PARTICLE_K, SLINKY_PARTICLE_LENGTH, p5, "SpringForce5", SPRING_FORCE_SLINKY_DURATION);
	springForceGenerator6 = new SpringForceGenerator(SLINKY_PARTICLE_K, SLINKY_PARTICLE_LENGTH, p6, "SpringForce6", SPRING_FORCE_SLINKY_DURATION);
	addForceGenerator(springForceGenerator1);
	addForceGenerator(springForceGenerator2);
	addForceGenerator(springForceGenerator3);
	addForceGenerator(springForceGenerator4);
	addForceGenerator(springForceGenerator5);
	addForceGenerator(springForceGenerator6);
	particleForceRegistry.addRegistry(springForceGenerator2, p1);
	particleForceRegistry.addRegistry(springForceGenerator1, p2);
	particleForceRegistry.addRegistry(springForceGenerator3, p2);
	particleForceRegistry.addRegistry(springForceGenerator2, p3);
	particleForceRegistry.addRegistry(springForceGenerator4, p3);
	particleForceRegistry.addRegistry(springForceGenerator3, p4);
	particleForceRegistry.addRegistry(springForceGenerator5, p4);
	particleForceRegistry.addRegistry(springForceGenerator4, p5);
	particleForceRegistry.addRegistry(springForceGenerator6, p5);
	particleForceRegistry.addRegistry(springForceGenerator5, p6);
	addParticle(p1);
	addParticle(p2);
	addParticle(p3);
	addParticle(p4);
	addParticle(p5);
	addParticle(p6);
}

void ParticleSystem::generateBuoyancyForce() {
	if (liquidModel == nullptr) {
		liquidModel = new Particle(WATER_PLANE, origin, Vector3(0.0f, 0.0f, 0.0f), true, true);
		Particle* immerse = new Particle(IMMERSE, PxTransform(origin.p + Vector3(0.0f, 20.0f, 0.0f)), Vector3(0.0f, 0.0f, 0.0f), true, true);
		float volume = immerse->getSize().x * immerse->getSize().y * immerse->getSize().z;
		buoyancyForceGenerator = new BuoyancyForceGenerator(liquidModel, liquidModel->getPosX() + 10.0f, volume, 1.0f, gravity.y, "BuoyancyForce", BUOYANCY_FORCE_DURATION);
		addForceGenerator(buoyancyForceGenerator);
		addParticle(immerse);
	}
}
#pragma endregion
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