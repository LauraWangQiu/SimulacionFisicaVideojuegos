#include "GaussianParticleGenerator.h"

GaussianParticleGenerator::GaussianParticleGenerator(string Name, Vector3 MeanPos, Vector3 MeanVel, double GenerationProbability, int NumParticles, Particle* Model, 
	Vector3 StdDevPos, Vector3 StdDevVel, bool Active) : ParticleGenerator(Name, MeanPos, MeanVel, GenerationProbability, NumParticles, Model, Active),
	stdDevPos(StdDevPos), stdDevVel(StdDevVel) {}

GaussianParticleGenerator::~GaussianParticleGenerator() {}

list<Particle*> GaussianParticleGenerator::generateParticles() {

	list<Particle*> mGenerator;

	if (active) {
		for (int i = 0; i < numParticles; i++) {
			if (generateRandomValue() <= generationProbability) {
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

	if (deadP->getNumExplodes() > 0) {
		for (int i = 0; i < model->getNumDivisions(); ++i) {
			auto* p = model->clone();

			Vector3 pos, dir;
			double angle = 2.0 * M_PI * (i + 1) / model->getNumDivisions();

			switch (model->getParticleType()) {
			case FIREWORK:
				pos = deadP->getPos();
				dir.x = std::cos(angle);
				dir.y = std::sin(angle);
				dir.z = 0.0f;
				static_cast<Firework*>(p)->addGenerator(this);
				break;
			case FIREWORK2:
				pos = deadP->getPos() + Vector3(n1(generator) * stdDevPos.x, n1(generator) * stdDevPos.y, n1(generator) * stdDevPos.z);
				dir = deadP->getVel() + Vector3(n1(generator) * stdDevVel.x, n1(generator) * stdDevVel.y, n1(generator) * stdDevVel.z);
				static_cast<Firework*>(p)->addGenerator(this);
				break;
			case FIREWORK3:
				pos = deadP->getPos() + Vector3(n2(generator) * stdDevPos.x, n2(generator) * stdDevPos.y, n2(generator) * stdDevPos.z);
				dir = deadP->getVel() + Vector3(n2(generator) * stdDevVel.x, n2(generator) * stdDevVel.y, n2(generator) * stdDevVel.z);
				static_cast<Firework*>(p)->addGenerator(this);
				break;
			}

			setParticleColor(p);
			p->reTransform(pos, dir);
			p->setNumExplodes(deadP->getNumExplodes() - 1);
			mGenerator.push_back(p);
		}
	}
	
	return mGenerator;
}