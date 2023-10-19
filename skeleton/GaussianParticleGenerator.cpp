#include "GaussianParticleGenerator.h"

GaussianParticleGenerator::GaussianParticleGenerator(string Name, Vector3 MeanPos, Vector3 MeanVel, double GenerationProbability, int NumParticles, Particle* Model, 
	Vector3 StdDevPos, Vector3 StdDevVel, double StdDevTime, bool Active) : ParticleGenerator(Name, MeanPos, MeanVel, GenerationProbability, NumParticles, Model, Active),
	stdDevPos(StdDevPos), stdDevVel(StdDevVel), stdDevTime(StdDevTime) {}

GaussianParticleGenerator::~GaussianParticleGenerator() {}

list<Particle*> GaussianParticleGenerator::generateParticles() {

	list<Particle*> mGenerator;

	if (active) {
		for (int i = 0; i < numParticles; i++) {
			if (generateRandomValue(2) <= generationProbability) {
				auto* p = model->clone();
				p->setPos(model->getPos() + Vector3(n(generator) * stdDevPos.x, n(generator) * stdDevPos.y, n(generator) * stdDevPos.z));
				p->setVel(model->getVel() + Vector3(n(generator) * stdDevVel.x, n(generator) * stdDevVel.y, n(generator) * stdDevVel.z));

				setParticleColor(p);

				mGenerator.push_back(p);

				switch (model->getParticleType()) {
				case FIREWORK: case FIREWORK2: case FIREWORK3:
					static_cast<Firework*>(p)->addGenerator(this);
					break;
				}
			}
		}
	}

	return mGenerator;
}

list<Particle*> GaussianParticleGenerator::generateParticles(Particle* deadP) {
	list<Particle*> mGenerator;

	for (int i = 0; i < model->getNumDivisions(); ++i) {
		
		Vector3 pos, dir;
		double angle = 2.0 * M_PI * (i + 1) / model->getNumDivisions();

		switch (model->getParticleType()) {
		case FIREWORK:
			pos = deadP->getPos();
			dir.x = std::cos(angle);
			dir.y = std::sin(angle);
			dir.z = 0.0f;
			break;
		case FIREWORK2:
			pos = deadP->getPos() + Vector3(r1(generator) * stdDevPos.x, r1(generator) * stdDevPos.y, r1(generator) * stdDevPos.z);
			dir = deadP->getVel() + Vector3(r1(generator) * stdDevVel.x, r1(generator) * stdDevVel.y, r1(generator) * stdDevVel.z);
			break;
		case FIREWORK3:
			pos = deadP->getPos() + Vector3(r2(generator) * stdDevPos.x, r2(generator) * stdDevPos.y, r2(generator) * stdDevPos.z);
			dir = deadP->getVel() + Vector3(r2(generator) * stdDevVel.x, r2(generator) * stdDevVel.y, r2(generator) * stdDevVel.z);
			break;
		}

		auto* p = model->clone();
		setParticleColor(p);
		p->reTransform(pos, dir);

		mGenerator.push_back(p);
	}

	return mGenerator;
}