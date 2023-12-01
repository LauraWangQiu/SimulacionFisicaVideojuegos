#include "UniformParticleGenerator.h"

UniformParticleGenerator::UniformParticleGenerator(string Name, Vector3 MeanPos, Vector3 MeanVel, double GenerationProbability, int NumParticles, Particle* Model,
	Vector3 VelWidth, Vector3 PosWidth, bool Active) : ParticleGenerator(Name, MeanPos, MeanVel, GenerationProbability, NumParticles, Model, Active),
	velWidth(VelWidth), posWidth(PosWidth) {}

UniformParticleGenerator::~UniformParticleGenerator() {}

list<Particle*> UniformParticleGenerator::generateParticles() {
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

				p->setPos(pos + Vector3(u(generator) * posWidth.x, u(generator) * posWidth.y, u(generator) * posWidth.z));
				p->setVel(vel + Vector3(u(generator) * velWidth.x, u(generator) * velWidth.y, u(generator) * velWidth.z));

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

list<Particle*> UniformParticleGenerator::generateParticles(Particle* deadP) {
	list<Particle*> mGenerator;
	
	if (deadP->getNumExplodes() > 0) {
		int num = min(model->getNumDivisions(), MAX_PARTICLES - actualNumParticles);

		for (int i = 0; i < num; ++i) {
			auto* p = model->clone();
			p->setNumExplodes(deadP->getNumExplodes() - 1);

			Vector3 pos, dir;
			double angle = 2.0 * M_PI * (i + 1) / num;

			int min = 1;
			int max = 5;
			std::uniform_int_distribution<> randomX(min, max);
			std::uniform_int_distribution<> randomZ(min, max);

			switch (model->getParticleType()) {
			case FIREWORK:
				pos = deadP->getPos();
				dir.x = std::cos(angle);
				dir.y = std::sin(angle);
				dir.z = 0.0f;
				if (p->getNumExplodes() > 0) static_cast<Firework*>(p)->addGenerator(this);
				break;
			case FIREWORK2:
				pos = deadP->getPos() + Vector3(u1(generator) * posWidth.x, u1(generator) * posWidth.y, u1(generator) * posWidth.z);
				dir = deadP->getVel() + Vector3(u1(generator) * (velWidth.x + randomX(generator)), u1(generator) * velWidth.y, u1(generator) * (velWidth.z + randomZ(generator)));
				if (p->getNumExplodes() > 0) static_cast<Firework*>(p)->addGenerator(this);
				break;
			case FIREWORK3:
				pos = deadP->getPos() + Vector3(u2(generator) * posWidth.x, u2(generator) * posWidth.y, u2(generator) * posWidth.z);
				dir = deadP->getVel() + Vector3(u2(generator) * (velWidth.x + randomX(generator)), u2(generator) * velWidth.y, u2(generator) * (velWidth.z + randomZ(generator)));
				if (p->getNumExplodes() > 0) static_cast<Firework*>(p)->addGenerator(this);
				break;
			}

			setParticleColor(p);
			p->reTransform(deadP->getPos(), dir);
			mGenerator.push_back(p);
		}
	}

	return mGenerator;
}