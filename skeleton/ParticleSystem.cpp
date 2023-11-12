#include "ParticleSystem.h"

#define GIZMO

// GENERADORES DE PARTICULAS
#define FIREWORK_MODEL_VISIBLE false
#define FIRE_MODEL_VISIBLE false
#define WATERFALL_MODEL_VISIBLE false
#define STEAM_MODEL_VISIBLE false
#define SQUIRT_MODEL_VISIBLE false
#define WIND_MODEL_VISIBLE false
#define WHIRL_WIND_MODEL_VISIBLE false

// GENERADORES DE FUERZAS
#define GRAVITY_FORCE1 Vector3(0.0f, -9.8f, 0.0f)
#define GRAVITY_FORCE2 Vector3(0.0f, 9.8f, 0.0f)
#define GRAVITY_FORCE1_DURATION 0.0f
#define GRAVITY_FORCE2_DURATION 0.0f
#define DRAG_FORCE_DURATION 0.0f
#define WIND_FORCE_DURATION 0.0f
#define WIND_NO_K2
#define WIND_SPECIFIC_SPACE
#define WIND_SIDE
#define WHIRL_WIND_FORCE_DURATION 0.0f
#define EXPLOSION_FORCE_DURATION 0.0f

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
	generateWindSystem();
	generateWhirlWindSystem();
}

ParticleSystem::~ParticleSystem() {

	listOfParticles.clear();
	listOfParticleGenerators.clear();

	originParticle = nullptr;

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
		addParticles(static_cast<Firework*>(p)->explode());
	break;
	}
}

// GENERADORES DE PARTICULAS
ParticleGenerator* ParticleSystem::getParticleGenerator(const string& name) {
	auto it = listOfParticleGenerators.begin();
	while (it != listOfParticleGenerators.end()) {
		if ((*it)->getName() == name) return *it;
	}
}

void ParticleSystem::generateFireworkSystem() {
	if (fireworkGenerator == nullptr) {
		Firework* model = new Firework(FIREWORK, origin, Vector3(0.0f, 1.0f, 0.0f), FIREWORK_MODEL_VISIBLE);
		fireworkGenerator = new GaussianParticleGenerator("Fireworks", origin.p, Vector3(0.0f, 0.0f, 0.0f), 0.1, 1,
			model, Vector3(0.0f, 0.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f));
		addParticleGenerator(fireworkGenerator);
		addGeneratorName(fireworkGenerator->getName());
	}
}

void ParticleSystem::generateFireSystem() {
	if (fireGenerator == nullptr) {
		Particle* model = new Particle(FIRE, origin, Vector3(0.0f, 1.0f, 0.0f), FIRE_MODEL_VISIBLE);
		fireGenerator = new GaussianParticleGenerator("Fire", origin.p, Vector3(0.0f, 0.0f, 0.0f), 1, 1,
			model, Vector3(2.0f, 2.0f, 2.0f), Vector3(1.0f, 1.0f, 1.0f));
		addParticleGenerator(fireGenerator);
		addGeneratorName(fireGenerator->getName());
	}
}

void ParticleSystem::generateWaterfallSystem() {
	if (waterfallGenerator == nullptr) {
		Particle* model = new Particle(WATER, origin, Vector3(0.0f, -1.0f, 0.0f), WATERFALL_MODEL_VISIBLE);
		waterfallGenerator = new UniformParticleGenerator("Waterfall", origin.p, Vector3(0.0f, 0.0f, 0.0f), 0.3, 1,
			model, Vector3(0.0f, 0.0f, 0.0f), Vector3(50.0f, 0.0f, 0.0f));
		addParticleGenerator(waterfallGenerator);
		addGeneratorName(waterfallGenerator->getName());
	}
}

void ParticleSystem::generateSteamSystem() {
	if (steamGenerator == nullptr) {
		Particle* model = new Particle(STEAM, origin, Vector3(0.0f, 1.0f, 0.0f), STEAM_MODEL_VISIBLE);
		steamGenerator = new GaussianParticleGenerator("Steam", origin.p, Vector3(0.0f, 0.0f, 0.0f), 1, 100,
			model, Vector3(10.0f, 10.0f, 10.0f), Vector3(0.0f, 0.0f, 0.0f));
		addParticleGenerator(steamGenerator);
		addGeneratorName(steamGenerator->getName());
	}
}

void ParticleSystem::generateSquirtSystem() {
	if (squirtGenerator == nullptr) {
		Particle* model = new Particle(WATER, origin, Vector3(0.3f, SQUIRT_INITIAL_VEL, 0.3f), SQUIRT_MODEL_VISIBLE);
		squirtGenerator = new GaussianParticleGenerator("Squirt", origin.p, Vector3(0.0f, 0.0f, 0.0f), 1, 50,
			model, Vector3(1.0f, 1.0f, 1.0f), Vector3(1.0f, 1.0f, 1.0f));
		addParticleGenerator(squirtGenerator);
		addGeneratorName(squirtGenerator->getName());
	}
}

void ParticleSystem::generateWindSystem() {
	if (windGenerator == nullptr) {
		Particle* model = new Particle(WIND, origin, Vector3(0.0f, -1.0f, 0.0f), WIND_MODEL_VISIBLE);
		windGenerator = new UniformParticleGenerator("Wind", origin.p, Vector3(0.0f, 0.0f, 0.0f), 0.5, 1,
			model, Vector3(0.0f, 0.0f, 0.0f), Vector3(50.0f, 0.0f, 50.0f));
		addParticleGenerator(windGenerator);
		addGeneratorName(windGenerator->getName());
	}
}

void ParticleSystem::generateWhirlWindSystem() {
	if (whirlWindGenerator == nullptr) {
		Particle* model = new Particle(WIND, origin, Vector3(0.0f, 1.0f, 0.0f), WHIRL_WIND_MODEL_VISIBLE);
		whirlWindGenerator = new GaussianParticleGenerator("WhirlWind", origin.p, Vector3(0.0f, 0.0f, 0.0f), 1, 1,
			model, Vector3(1.0f, 1.0f, 1.0f), Vector3(1.0f, 1.0f, 1.0f));
		addParticleGenerator(whirlWindGenerator);
		addGeneratorName(whirlWindGenerator->getName());
	}
}

void ParticleSystem::addFirework(ParticleType Type, PxTransform Transform, Vector3 Dir) {
	Firework* model = new Firework(Type, Transform, Dir, FIREWORK_MODEL_VISIBLE);
	Firework* p = new Firework(Type, Transform, Dir);
	addParticle(p);
	GaussianParticleGenerator* g = new GaussianParticleGenerator("Firework", Transform.p, Dir, 0.1, 1,
		model, Vector3(0.0f, 0.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f));
	p->addGenerator(g);
	addGeneratorName(g->getName());
}

// GENERADORES DE FUERZAS
void ParticleSystem::addForces(Particle* p) {

	// Se le añaden todas las fuerzas existentes a las particulas deseadas
	for (auto fg : listOfForceGenerators) {
		particleForceRegistry.addRegistry(fg, p);

		/*if (static_cast<WindGenerator*>(fg) != nullptr) {
			switch (p->getParticleType()) {
			case ...: particleForceRegistry.addRegistry(fg, p); break;
			}
		}
		else if (static_cast<GravityForceGenerator*>(fg) != nullptr) {
			switch (p->getParticleType()) {
			case ...: particleForceRegistry.addRegistry(fg, p); break;
			}
		}
		else if (static_cast<ParticleDragGenerator*>(fg) != nullptr) {
			switch (p->getParticleType()) {
			case ...: particleForceRegistry.addRegistry(fg, p); break;
			}
		}*/
	}
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

void ParticleSystem::generateGravityForce() {
	gravityForceGenerator = new GravityForceGenerator(GRAVITY_FORCE1, "GravityForce1", GRAVITY_FORCE1_DURATION);
	addForceGenerator(gravityForceGenerator);
	addGeneratorName(gravityForceGenerator->getName());
}

void ParticleSystem::generateGravityForce2() {
	gravityForceGenerator2 = new GravityForceGenerator(GRAVITY_FORCE2, "GravityForce2", GRAVITY_FORCE2_DURATION);
	addForceGenerator(gravityForceGenerator2);
	addGeneratorName(gravityForceGenerator2->getName());
}

void ParticleSystem::generateDragForce() {
	particleDragForceGenerator = new ParticleDragGenerator(0.2f, 0.0f, "DragForce", DRAG_FORCE_DURATION);
	addForceGenerator(particleDragForceGenerator);
	addGeneratorName(particleDragForceGenerator->getName());
}

void ParticleSystem::generateWindForce() {
	windForceGenerator = new WindGenerator(
#ifdef WIND_NO_K2
		5.0f, 0.0f,
#else
		0.0f, 5.0f,
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
	addGeneratorName(windForceGenerator->getName());
}

void ParticleSystem::generateWhirlWindsForce() {
	whirlWindsForceGenerator = new WhirlWindForce(7.0f, Vector3(0.0f, 40.0f, 0.0f), 
		"WhirlWind", WHIRL_WIND_FORCE_DURATION);
	addForceGenerator(whirlWindsForceGenerator);
	addGeneratorName(whirlWindsForceGenerator->getName());
}

void ParticleSystem::generateExplosionsForce() {

}

void ParticleSystem::switchGravityForce() {
	if (gravityForceGenerator == nullptr) {
		generateGravityForce();
	}
	else if (gravityForceGenerator2 == nullptr) {
		generateGravityForce2();
	}
	else {
		removeForceGenerator(gravityForceGenerator);
		removeForceGenerator(gravityForceGenerator2);
		gravityForceGenerator = nullptr;
		gravityForceGenerator2 = nullptr;
	}
}

void ParticleSystem::activateDragForce() {
	if (particleDragForceGenerator == nullptr) {
		generateDragForce();
	} else {
		removeForceGenerator(particleDragForceGenerator);
		particleDragForceGenerator = nullptr;
	}
}

void ParticleSystem::activateWindForce() {
	if (windForceGenerator == nullptr) {
		generateWindForce();
	} else {
		removeForceGenerator(windForceGenerator);
		windForceGenerator = nullptr;
	}
}

void ParticleSystem::activateWhirlWindsForce() {
	if (whirlWindsForceGenerator == nullptr) {
		generateWhirlWindsForce();
	} else {
		removeForceGenerator(whirlWindsForceGenerator);
		whirlWindsForceGenerator = nullptr;
	}
}

void ParticleSystem::activateExplosionsForce() {
	if (explosionsForceGenerator == nullptr) {
		generateExplosionsForce();
	} else {
		removeForceGenerator(explosionsForceGenerator);
		explosionsForceGenerator = nullptr;
	}
}