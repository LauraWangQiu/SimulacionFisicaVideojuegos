#include "ParticleSystem.h"

#define GIZMO

// GENERADORES DE PARTICULAS
#define FIREWORK_MODEL_ACTIVE false
#define FIRE_MODEL_ACTIVE false
#define WATERFALL_MODEL_ACTIVE false
#define STEAM_MODEL_ACTIVE false
#define SQUIRT_MODEL_ACTIVE false

// GENERADORES DE FUERZAS
#define GRAVITY_FORCE
#define GRAVITY_FORCE1 Vector3(0.0f, -1.0f, 0.0f)
#define GRAVITY_FORCE2 Vector3(0.0f, 1.0f, 0.0f)
#define GRAVITY_FORCE1_DURATION 0.0f
#define GRAVITY_FORCE2_DURATION 0.0f
#define DRAG_FORCE
#define DRAG_FORCE_DURATION 0.0f

ParticleSystem::ParticleSystem(const Vector3& g) : gravity(g), numMaxParticles(MAX_PARTICLES), originParticle(nullptr), numParticles(0), origin(PxTransform(Vector3(0.0f, 0.0f, 0.0f))) {

	#ifdef GIZMO
		addOrigin();
	#endif // GIZMO

	// GENERADORES DE PARTICULAS
	generateFireworkSystem();
	generateFireSystem();
	generateWaterfallSystem();
	generateSteamSystem();
	generateSquirtSystem();

	// GENERADORES DE FUERZAS
	generateWindSystem();
	generateWhirlWindsSystem();
	generateExplosionsSystem();
}

ParticleSystem::~ParticleSystem() {

#ifdef GIZMO
	delete originParticle; originParticle = nullptr;
#endif

	for (auto it = listOfParticles.begin(); it != listOfParticles.end(); ++it) {
		delete* it; *it = nullptr;
	}
	listOfParticles.clear();
	
	for (auto it = listOfParticleGenerators.begin(); it != listOfParticleGenerators.end(); ++it) {
		delete* it; *it = nullptr;
	}
	listOfParticleGenerators.clear();

	fireworkGenerator = nullptr;
	fireGenerator = nullptr;
	waterfallGenerator = nullptr;
	steamGenerator = nullptr;
	squirtGenerator = nullptr;

	for (auto it = listOfForceGenerators.begin(); it != listOfForceGenerators.end(); ++it) {
		delete* it; *it = nullptr;
	}
	listOfForceGenerators.clear();

	gravityForceGenerator = nullptr;
	gravityForceGenerator2 = nullptr;
	particleDragForceGenerator = nullptr;

	windForceGenerator = nullptr;
	whirlWindsForceGenerator = nullptr;
	explosionsForceGenerator = nullptr;
}

void ParticleSystem::addGeneratorName(string name) {
	listOfGeneratorNames.push_back(name);
}

void ParticleSystem::addParticleGenerator(ParticleGenerator* pg) {
	listOfParticleGenerators.push_back(pg);
}

void ParticleSystem::addForceGenerator(ForceGenerator* fg) {
	listOfForceGenerators.push_back(fg);
}

void ParticleSystem::addOrigin() {
	originParticle = new Particle(BASIC, origin, Vector3(0.0f, 0.0f, 0.0f), true, true);
}

void ParticleSystem::addParticles(list<Particle*> list) {
	for (auto p : list) {
		listOfParticles.push_back(p);
		addForces(p);
	}
	updateNumParticles(list.size());
	updateNumParticlesText();
}

void ParticleSystem::addParticle(ParticleType Type, PxTransform Transform, Vector3 Dir) {
	Particle* p = new Particle(Type, Transform, Dir);
	addParticle(p);
}

void ParticleSystem::addParticle(PxTransform Transform, Vector3 Dir, float Mass, float Velc, Vector3 Acc, float Damping, Vector3 Size,
	float Time, Vector4 Color, int NumDivisions) {
	Particle* p = new Particle(Transform, Dir, Mass, Velc, Acc, Damping, Size, Time, Color, NumDivisions);
	addParticle(p);
}

void ParticleSystem::addParticle(Particle* p) {
	listOfParticles.push_back(p);
	addForces(p);
	increaseNumParticles();
	updateNumParticlesText();
}

void ParticleSystem::update(double t) {
	// Recorro todos los generadores de particulas y genero particulas anadiendolas a listOfParticles
	for (auto pg : listOfParticleGenerators) {
		if ((pg->getNumParticles() + numParticles) < numMaxParticles && pg->getActive()) {
			auto list = pg->generateParticles();
			addParticles(list);
		}
	}

	// Recorro todos los generadores de fuerzas y elimino aquellos que han terminado
	auto fg = listOfForceGenerators.begin();
	while (fg != listOfForceGenerators.end()) {
		if (!(*fg)->updateTime(t)) {
			particleForceRegistry.removeForceGenerator(*fg);
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
			delete* p;
			particleForceRegistry.deleteParticleRegistry(*p);
			p = listOfParticles.erase(p);
			decreaseNumParticles();
			updateNumParticlesText();
		} else ++p;
	}
}

void ParticleSystem::onParticleDeath(Particle* p) {
	switch (p->getParticleType()) {
	case FIREWORK: case FIREWORK2: case FIREWORK3:
		addParticles(static_cast<Firework*>(p)->explode());
	break;
	}
}

ParticleGenerator* ParticleSystem::getParticleGenerator(const string& name) {
	auto it = listOfParticleGenerators.begin();
	while (it != listOfParticleGenerators.end()) {
		if ((*it)->getName() == name) return *it;
	}
}

void ParticleSystem::generateFireworkSystem() {
	if (fireworkGenerator == nullptr) {
		Firework* model = new Firework(FIREWORK, origin, Vector3(0.0f, 1.0f, 0.0f), FIREWORK_MODEL_ACTIVE);
		fireworkGenerator = new GaussianParticleGenerator("Fireworks", origin.p, Vector3(0.0f, 0.0f, 0.0f), 0.1, 1,
			model, Vector3(0.0f, 0.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f));
		addParticleGenerator(fireworkGenerator);
		addGeneratorName(fireworkGenerator->getName());
	}
}

void ParticleSystem::generateFireSystem() {
	if (fireGenerator == nullptr) {
		Particle* model = new Particle(FIRE, origin, Vector3(0.0f, 1.0f, 0.0f), FIRE_MODEL_ACTIVE);
		fireGenerator = new GaussianParticleGenerator("Fire", origin.p, Vector3(0.0f, 0.0f, 0.0f), 1, 1,
			model, Vector3(2.0f, 2.0f, 2.0f), Vector3(1.0f, 1.0f, 1.0f));
		addParticleGenerator(fireGenerator);
		addGeneratorName(fireGenerator->getName());
	}
}

void ParticleSystem::generateWaterfallSystem() {
	if (waterfallGenerator == nullptr) {
		Particle* model = new Particle(WATER, origin, Vector3(0.0f, -1.0f, 0.0f), WATERFALL_MODEL_ACTIVE);
		waterfallGenerator = new UniformParticleGenerator("Waterfall", origin.p, Vector3(0.0f, 0.0f, 0.0f), 0.3, 1,
			model, Vector3(0.0f, 0.0f, 0.0f), Vector3(50.0f, 0.0f, 0.0f));
		addParticleGenerator(waterfallGenerator);
		addGeneratorName(waterfallGenerator->getName());
	}
}

void ParticleSystem::generateSteamSystem() {
	if (steamGenerator == nullptr) {
		Particle* model = new Particle(STEAM, origin, Vector3(0.0f, 1.0f, 0.0f), STEAM_MODEL_ACTIVE);
		steamGenerator = new GaussianParticleGenerator("Steam", origin.p, Vector3(0.0f, 0.0f, 0.0f), 1, 100,
			model, Vector3(10.0f, 10.0f, 10.0f), Vector3(0.0f, 0.0f, 0.0f));
		addParticleGenerator(steamGenerator);
		addGeneratorName(steamGenerator->getName());
	}
}

void ParticleSystem::generateSquirtSystem() {
	if (squirtGenerator == nullptr) {
		Particle* model = new Particle(WATER, origin, Vector3(0.3f, SQUIRT_INITIAL_VEL, 0.0f), SQUIRT_MODEL_ACTIVE);
		squirtGenerator = new GaussianParticleGenerator("Squirt", origin.p, Vector3(0.0f, 0.0f, 0.0f), 1, 50,
			model, Vector3(1.0f, 1.0f, 1.0f), Vector3(1.0f, 1.0f, 1.0f));
		addParticleGenerator(squirtGenerator);
		addGeneratorName(squirtGenerator->getName());
	}
}

void ParticleSystem::addFirework(ParticleType Type, PxTransform Transform, Vector3 Dir) {
	Firework* model = new Firework(Type, Transform, Dir, FIREWORK_MODEL_ACTIVE);
	Firework* p = new Firework(Type, Transform, Dir);
	addParticle(p);
	GaussianParticleGenerator* g = new GaussianParticleGenerator("Firework", Transform.p, Dir, 0.1, 1,
		model, Vector3(0.0f, 0.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f));
	p->addGenerator(g);
	addGeneratorName(g->getName());
}

void ParticleSystem::addForces(Particle* p) {
	// Se le añaden todas las fuerzas existentes a las particulas deseadas
	for (auto fg : listOfForceGenerators) {
		/*switch (p->getParticleType()) {
		case BASIC: break;
		case FIREWORK: case FIREWORK2: case FIREWORK3: break;
		case FIRE: break;
		case WATER:break;
		case STEAM: break;
		case CANNON_BALL: break;
		case TANK_BALL: break;
		case GUN_BULLET: break;
		case LASER: break;
		default: break;
		}*/
		particleForceRegistry.addRegistry(fg, p);
	}
}

void ParticleSystem::addGravityForce() {
#ifdef GRAVITY_FORCE
	if (gravityForceGenerator == nullptr) {
		gravityForceGenerator = new GravityForceGenerator(GRAVITY_FORCE1, "GravityForce1", GRAVITY_FORCE1_DURATION);
		addForceGenerator(gravityForceGenerator);
		addGeneratorName(gravityForceGenerator->getName());
	}
	else if (gravityForceGenerator2 == nullptr) {
		gravityForceGenerator2 = new GravityForceGenerator(GRAVITY_FORCE2, "GravityForce2", GRAVITY_FORCE2_DURATION);
		addForceGenerator(gravityForceGenerator2);
		addGeneratorName(gravityForceGenerator2->getName());
	}
#endif // GRAVITY_FORCE
}

void ParticleSystem::removeGravityForce() {
	if (gravityForceGenerator != nullptr) {
		particleForceRegistry.removeForceGenerator(gravityForceGenerator);
		auto it = listOfForceGenerators.begin();
		while (it != listOfForceGenerators.end()) {
			if (*it == gravityForceGenerator) {
				listOfForceGenerators.erase(it);
				break;
			}
			++it;
		}
		gravityForceGenerator = nullptr;
	}

	if (gravityForceGenerator2 != nullptr) {
		particleForceRegistry.removeForceGenerator(gravityForceGenerator2);
		auto it = listOfForceGenerators.begin();
		while (it != listOfForceGenerators.end()) {
			if (*it == gravityForceGenerator2) {
				listOfForceGenerators.erase(it);
				break;
			}
			++it;
		}
		gravityForceGenerator2 = nullptr;
	}
}

void ParticleSystem::addDragForce() {
#ifdef DRAG_FORCE
	if (particleDragForceGenerator == nullptr) {
		particleDragForceGenerator = new ParticleDragGenerator("DragForce", DRAG_FORCE_DURATION);
		addForceGenerator(particleDragForceGenerator);
		addGeneratorName(particleDragForceGenerator->getName());
	}
#endif // DRAG_FORCE
}

void ParticleSystem::removeDragForce() {
	if (particleDragForceGenerator != nullptr) {
		particleForceRegistry.removeForceGenerator(particleDragForceGenerator);
		auto it = listOfForceGenerators.begin();
		while (it != listOfForceGenerators.end()) {
			if (*it == particleDragForceGenerator) {
				listOfForceGenerators.erase(it);
				break;
			}
			++it;
		}
		particleDragForceGenerator = nullptr;
	}
}

void ParticleSystem::generateWindSystem() {
	if (windForceGenerator == nullptr) {

	}
}

void ParticleSystem::generateWhirlWindsSystem() {
	if (whirlWindsForceGenerator == nullptr) {

	}
}

void ParticleSystem::generateExplosionsSystem() {
	if (explosionsForceGenerator == nullptr) {

	}
}