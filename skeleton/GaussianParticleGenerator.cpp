#include "GaussianParticleGenerator.h"

GaussianParticleGenerator::GaussianParticleGenerator(string Name, Vector3 MeanPos, Vector3 MeanVel, double GenerationProbability, int NumParticles, Particle* Model, 
	Vector3 StdDevPos, Vector3 StdDevVel, double StdDevTime, bool Active) : ParticleGenerator(Name, MeanPos, MeanVel, GenerationProbability, NumParticles, Model, Active),
	stdDevPos(StdDevPos), stdDevVel(StdDevVel), stdDevTime(StdDevTime) {}

GaussianParticleGenerator::~GaussianParticleGenerator() {}

list<Particle*> GaussianParticleGenerator::generateParticles() {

	list<Particle*> mGenerator;

	if (active) {
		for (int i = 0; i < numParticles; i++) {
			auto* p = model->clone();

			if (generateRandomValue(2) <= generationProbability) {
				p->setPos(model->getPos() + Vector3(n(generator) * stdDevPos.x, n(generator) * stdDevPos.y, n(generator) * stdDevPos.z));
				p->setVel(model->getVel() + Vector3(n(generator) * stdDevVel.x, n(generator) * stdDevVel.y, n(generator) * stdDevVel.z));

				setParticleColor(p);

				mGenerator.push_back(p);

				switch (model->getParticleType()) {
				case FIRE: case FIREWORK: case WATER:
					static_cast<Firework*>(p)->addGenerator(this);
					break;
				}
			}
		}
	}

	return mGenerator;
}

//list<Particle*> GaussianParticleGenerator::generateParticles(Particle* deadP) {
//	list<Particle*> mGenerator;
//
//	return mGenerator;
//}