#include "ParticleSystem.h"

#define GIZMO

#define FIREWORK_MODEL_ACTIVE false
#define FIREWORK_GAUSSIAN_GEN
#define FIRE_MODEL_ACTIVE false
#define WATERFALL_MODEL_ACTIVE false
#define STEAM_MODEL_ACTIVE false
#define SQUIRT_MODEL_ACTIVE false

ParticleSystem::ParticleSystem(const Vector3& g) : gravity(g), numMaxParticles(MAX_PARTICLES), originParticle(nullptr), numParticles(0), origin(PxTransform(Vector3(0.0f, 0.0f, 0.0f))) {

	#ifdef GIZMO
		addOrigin();
	#endif // GIZMO

	generateFireworkSystem();
	generateFireSystem();
	generateWaterfallSystem();
	generateSteamSystem();
	generateSquirtSystem();
}

ParticleSystem::~ParticleSystem() {

	while (!listOfParticles.empty()) listOfParticles.pop_back();

	while (!listOfParticleGenerators.empty()) listOfParticleGenerators.pop_back();
}

void ParticleSystem::addParticles(list<Particle*> list) {
	for (auto p : list) {
		listOfParticles.push_back(p);
	}
	numParticles += list.size();
}

void ParticleSystem::addParticle(ParticleType Type, PxTransform Transform, Vector3 Dir) {
	listOfParticles.push_back(new Particle(Type, Transform, Dir));
}

void ParticleSystem::addParticle(PxTransform Transform, Vector3 Dir, float Mass, float Velc, Vector3 Acc, float Damping, Vector3 Size,
	float Time, Vector4 Color, int NumDivisions) {
	listOfParticles.push_back(new Particle(Transform, Dir, Mass, Velc, Acc, Damping, Size, Time, Color, NumDivisions));
}

void ParticleSystem::update(double t) {
	// Recorro todos los generadores de particulas y genero particulas anadiendolas a listOfParticles
	for (auto pg : listOfParticleGenerators) {
		if ((pg->getNumParticles() + numParticles) < numMaxParticles && pg->getActive()) {
			auto list = pg->generateParticles();
			addParticles(list);
		}
	}

	// Actualizo las particulas y elimino aquellas no vivas
	auto p = listOfParticles.begin();
	while (p != listOfParticles.end()) {
		// Si se termina su tiempo de vida, se elimina
		if (!(*p)->integrate(t)) {
			onParticleDeath(*p);
			delete* p;
			p = listOfParticles.erase(p);
			decreaseNumParticles();
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
	Firework* model = new Firework(FIREWORK, origin, Vector3(0.0f, 1.0f, 0.0f), FIREWORK_MODEL_ACTIVE);
#ifdef FIREWORK_GAUSSIAN_GEN
	fireworkGenerator = new GaussianParticleGenerator("Fireworks", origin.p, Vector3(0.0f, 0.0f, 0.0f), 0.1, 1,
		model, Vector3(0.0f, 0.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f));
#else
	fireworkGenerator = new UniformParticleGenerator("Fireworks", origin.p, Vector3(0.0f, 0.0f, 0.0f), 0.3, 1,
		model, Vector3(0.0f, 0.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f));
#endif
	listOfParticleGenerators.push_back(fireworkGenerator);
}

void ParticleSystem::generateFireSystem() {
	Particle* model = new Particle(FIRE, origin, Vector3(0.0f, 1.0f, 0.0f), FIRE_MODEL_ACTIVE);
	fireGenerator = new GaussianParticleGenerator("Fire", origin.p, Vector3(0.0f, 0.0f, 0.0f), 1, 1,
		model, Vector3(2.0f, 2.0f, 2.0f), Vector3(1.0f, 1.0f, 1.0f));
	listOfParticleGenerators.push_back(fireGenerator);
}

void ParticleSystem::generateWaterfallSystem() {
	Particle* model = new Particle(WATER, origin, Vector3(0.0f,-1.0f,0.0f), WATERFALL_MODEL_ACTIVE);
	waterfallGenerator = new UniformParticleGenerator("Waterfall", origin.p, Vector3(0.0f, 0.0f, 0.0f), 0.3, 1,
		model, Vector3(0.0f, 0.0f, 0.0f), Vector3(50.0f, 0.0f, 0.0f));
	listOfParticleGenerators.push_back(waterfallGenerator);
}

void ParticleSystem::generateSteamSystem() {
	Particle* model = new Particle(STEAM, origin, Vector3(0.0f, 1.0f, 0.0f), STEAM_MODEL_ACTIVE);
	steamGenerator = new GaussianParticleGenerator("Steam", origin.p, Vector3(0.0f, 0.0f, 0.0f), 1, 100,
		model, Vector3(10.0f, 10.0f, 10.0f), Vector3(0.0f, 0.0f, 0.0f));
	listOfParticleGenerators.push_back(steamGenerator);
}

void ParticleSystem::generateSquirtSystem() {
	Particle* model = new Particle(WATER, origin, Vector3(0.3f, SQUIRT_INITIAL_VEL, 0.0f), SQUIRT_MODEL_ACTIVE);
	squirtGenerator = new GaussianParticleGenerator("Squirt", origin.p, Vector3(0.0f, 0.0f, 0.0f), 1, 50,
		model, Vector3(1.0f, 1.0f, 1.0f), Vector3(1.0f, 1.0f, 1.0f));
	listOfParticleGenerators.push_back(squirtGenerator);
}

void ParticleSystem::addFirework(ParticleType Type, PxTransform Transform, Vector3 Dir) {
	Firework* model = new Firework(Type, Transform, Dir, FIREWORK_MODEL_ACTIVE);
	Firework* p = new Firework(Type, Transform, Dir);
	listOfParticles.push_back(p);
	GaussianParticleGenerator* g = new GaussianParticleGenerator("Fireworks", Transform.p, Dir, 0.1, 1,
		model, Vector3(0.0f, 0.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f));
	p->addGenerator(g);
}

void ParticleSystem::addOrigin() {
	originParticle = new Particle(BASIC, origin, Vector3(0.0f, 0.0f, 0.0f), true, true);
}