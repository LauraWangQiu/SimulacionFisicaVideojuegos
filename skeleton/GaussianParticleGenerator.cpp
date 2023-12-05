#include "GaussianParticleGenerator.h"

GaussianParticleGenerator::GaussianParticleGenerator(string Name, Vector3 MeanPos, Vector3 MeanVel, double GenerationProbability, int NumParticles, Particle* Model, 
	Vector3 StdDevPos, Vector3 StdDevVel, bool Active) : ParticleGenerator(Name, MeanPos, MeanVel, GenerationProbability, NumParticles, Model, Active),
	stdDevPos(StdDevPos), stdDevVel(StdDevVel) {}

GaussianParticleGenerator::~GaussianParticleGenerator() {}

list<Particle*> GaussianParticleGenerator::generateParticles() {

	list<Particle*> mGenerator;

	if (active) {
		int num = min(numParticles, MAX_PARTICLES - actualNumParticles);

		for (int i = 0; i < num; ++i) {
			if (generateRandomValue() < generationProbability) {

				randomize();

				auto* p = model->clone();

				Vector3 pos, vel;
				if (!model->isRigid()) { pos = model->getPos(); vel = model->getVel(); } 
				else { pos = meanPos; vel = meanVel; }

				p->setPos(pos + Vector3(n(generator) * stdDevPos.x, n(generator) * stdDevPos.y, n(generator) * stdDevPos.z));
				p->setVel(vel + Vector3(n(generator) * stdDevVel.x, n(generator) * stdDevVel.y, n(generator) * stdDevVel.z));

				setParticleColor(p);

				mGenerator.push_back(p);

				switch (model->getParticleType()) {
				case FIREWORK: case FIREWORK2:
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
		int num = min(model->getNumDivisions(), MAX_PARTICLES - actualNumParticles);

		for (int i = 0; i < num; ++i) {
			auto* p = model->clone();
			p->setNumExplodes(deadP->getNumExplodes() - 1);

			Vector3 pos, dir;
			double angle = 2.0 * M_PI * (i + 1) / num;

			switch (model->getParticleType()) {
			case FIREWORK:
				pos = deadP->getPos() + Vector3(n2(generator) * stdDevPos.x, n2(generator) * stdDevPos.y, n2(generator) * stdDevPos.z);
				dir = Vector3(n2(generator), n2(generator), n2(generator));
				if (p->getNumExplodes() > 0) static_cast<Firework*>(p)->addGenerator(this);
				break;
			case FIREWORK2:
				pos = deadP->getPos();
				dir.x = std::cos(angle);
				dir.y = std::sin(angle);
				dir.z = 0.0f;
				if (p->getNumExplodes() > 0) static_cast<Firework*>(p)->addGenerator(this);
				break;
			}

			setParticleColor(p);
			p->reTransform(pos, dir);
			mGenerator.push_back(p);
		}
	}
	
	return mGenerator;
}