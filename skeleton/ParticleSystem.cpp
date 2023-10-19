#include "ParticleSystem.h"

#define FIREWORKGENERATOR
#define TYPEOF_FIREWORK FIREWORK3
#define FIREWORKGENERATOR_ACTIVE true
//#define FIREGENERATOR
#define FIREGENERATOR_ACTIVE true
//#define WATERFALLGENERATOR
#define WATERFALLGENERATOR_ACTIVE true
//#define STEAMGENERATOR
#define STEAMGENERATOR_ACTIVE true

ParticleSystem::ParticleSystem(const Vector3& g) : gravity(g), numMaxParticles(MAX_PARTICLES), numParticles(0), origin(PxTransform(Vector3(0.0f, 0.0f, 0.0f))) {
	#ifdef FIREWORKGENERATOR
		generateFireworkSystem();
	#endif // FIREWORKGENERATOR
	#ifdef FIREGENERATOR
		generateFireSystem();
	#endif // FIREGENERATOR
	#ifdef WATERFALLGENERATOR
		generateWaterfallSystem();
	#endif // WATERFALLGENERATOR
	#ifdef STEAMGENERATOR
		generateSteamSystem();
	#endif // STEAMGENERATOR
}
ParticleSystem::~ParticleSystem() {}

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
	Firework* model = new Firework(TYPEOF_FIREWORK, origin, Vector3(0.0f, 1.0f, 0.0f), false);
	fireworkGenerator = new GaussianParticleGenerator("Fireworks", origin.p, Vector3(0.0f, 0.0f, 0.0f), 0.1, 1,
		model, Vector3(0.0f, 0.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f), 5, FIREWORKGENERATOR_ACTIVE);
	listOfParticleGenerators.push_back(fireworkGenerator);
}

void ParticleSystem::generateFireSystem() {
	Particle* model = new Particle(FIRE, origin, Vector3(0.0f, 1.0f, 0.0f), false);
	fireGenerator = new GaussianParticleGenerator("Fire", origin.p, Vector3(0.0f, 0.0f, 0.0f), 1, 1,
		model, Vector3(2.0f, 2.0f, 2.0f), Vector3(1.0f, 1.0f, 1.0f), 5, FIREGENERATOR_ACTIVE);
	listOfParticleGenerators.push_back(fireGenerator);
}

void ParticleSystem::generateWaterfallSystem() {
	Particle* model = new Particle(WATER, origin, Vector3(0.0f,-1.0f,0.0f), false);
	waterfallGenerator = new UniformParticleGenerator("Waterfall", origin.p, Vector3(0.0f, 0.0f, 0.0f), 0.3, 1,
		model, Vector3(0.0f, 0.0f, 0.0f), Vector3(50.0f, 0.0f, 0.0f), WATERFALLGENERATOR_ACTIVE);
	listOfParticleGenerators.push_back(waterfallGenerator);
}

void ParticleSystem::generateSteamSystem() {
	Particle* model = new Particle(STEAM, origin, Vector3(0.0f, 1.0f, 0.0f), false);
	steamGenerator = new GaussianParticleGenerator("Fog", origin.p, Vector3(0.0f, 0.0f, 0.0f), 1, 100,
		model, Vector3(10.0f, 10.0f, 10.0f), Vector3(0.0f, 0.0f, 0.0f), 5, STEAMGENERATOR_ACTIVE);
	listOfParticleGenerators.push_back(steamGenerator);
}

void ParticleSystem::addFirework(ParticleType Type, PxTransform Transform, Vector3 Dir) {
	Firework* model = new Firework(Type, Transform, Dir, false);
	Firework* p = new Firework(Type, Transform, Dir);
	listOfParticles.push_back(p);
	GaussianParticleGenerator* g = new GaussianParticleGenerator("Fireworks", Transform.p, Dir, 0.1, 1,
		model, Vector3(0.0f, 0.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f), 5);
	p->addGenerator(g);
}