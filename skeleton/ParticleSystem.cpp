#include "ParticleSystem.h"

#define FIREWORKGENERATOR
//#define FIREGENERATOR
//#define WATERFALLGENERATOR

ParticleSystem::ParticleSystem(const Vector3& g) {
	#ifdef FIREWORKGENERATOR
		generateFireworkSystem();
	#endif // FIREWORKGENERATOR
	#ifdef FIREGENERATOR
		generateFireSystem();
	#endif // FIREGENERATOR
	#ifdef WATERFALLGENERATOR
			generateWaterfallSystem();
	#endif // WATERFALLGENERATOR
}
ParticleSystem::~ParticleSystem() {}

void ParticleSystem::addParticle(ParticleType Type, PxTransform Transform, Vector3 Dir, float Time, PxReal Size, Vector4 Color) {
	listOfParticles.push_back(new Particle(Type, Transform, Dir, Time, Size, Color));
}

void ParticleSystem::update(double t) {
	// Recorro todos los generadores de particulas y genero particulas anadiendolas a listOfParticles
	for (auto pg : listOfParticleGenerators) {
		if (pg->getActive()) {
			auto list = pg->generateParticles();
			for (auto p : list) listOfParticles.push_back(p);
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
		} else ++p;
	}
}

void ParticleSystem::onParticleDeath(Particle* p) {
	
	switch (p->getParticleType()) {
	case FIREWORK:
		// Cuando se elimine el fuego artificial, anadir la explosion
		static_cast<Firework*>(p)->explode();
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
	Firework* model = new Firework();
	fireworkGenerator = new GaussianParticleGenerator("Fireworks", Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, -1.0f, 0.0f), 0.1, 1,
		model, Vector3(0.0f, 0.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f), 5);
	listOfParticleGenerators.push_back(fireworkGenerator);
}

void ParticleSystem::generateFireSystem() {
	Particle* model = new Particle(FIRE, PxTransform(0.0f, 0.0f, 0.0f));
	fireGenerator = new GaussianParticleGenerator("Fire", Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), 1, 1,
		model, Vector3(2.0f, 2.0f, 2.0f), Vector3(1.0f, 1.0f, 1.0f), 5);
	listOfParticleGenerators.push_back(fireGenerator);
}

void ParticleSystem::generateWaterfallSystem() {
	Particle* model = new Particle(WATER, PxTransform(0.0f, 0.0f, 0.0f));
	waterfallGenerator = new GaussianParticleGenerator("Waterfall", Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), 1, 1,
		model, Vector3(10.0f, 0.0f, 2.0f), Vector3(10.0f, 0.0f, 2.0f), 1);
	listOfParticleGenerators.push_back(waterfallGenerator);
}