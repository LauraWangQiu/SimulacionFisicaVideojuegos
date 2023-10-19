#include "UniformParticleGenerator.h"

UniformParticleGenerator::UniformParticleGenerator(string Name, Vector3 MeanPos, Vector3 MeanVel, double GenerationProbability, int NumParticles, Particle* Model,
	Vector3 VelWidth, Vector3 PosWidth, bool Active) : ParticleGenerator(Name, MeanPos, MeanVel, GenerationProbability, NumParticles, Model, Active),
	velWidth(VelWidth), posWidth(PosWidth) {}

UniformParticleGenerator::~UniformParticleGenerator() {}

list<Particle*> UniformParticleGenerator::generateParticles() {
	list<Particle*> mGenerator;

	if (active) {
		for (int i = 0; i < numParticles; i++) {
			if (generateRandomValue(2) <= generationProbability) {
				auto* p = model->clone();
				p->setPos(model->getPos() + Vector3(u(generator) * posWidth.x, u(generator) * posWidth.y, u(generator) * posWidth.z));
				p->setVel(model->getVel() + Vector3(u(generator) * velWidth.x, u(generator) * velWidth.y, u(generator) * velWidth.z));

				setParticleColor(p);

				mGenerator.push_back(p);
			}
		}
	}

	return mGenerator;
}

list<Particle*> UniformParticleGenerator::generateParticles(Particle* deadP) {
	list<Particle*> mGenerator;

	for (int i = 0; i < model->getNumDivisions(); ++i) {
		double angle = 2.0 * M_PI * (i + 1) / model->getNumDivisions();
		double dirX, dirY, dirZ;
		switch (model->getParticleType()) {
		case FIREWORK:
			dirX = std::cos(angle);
			dirY = std::sin(angle);
			dirZ = 0.0f;
			break;
		case FIREWORK2:
			dirX = std::cos(angle);
			dirY = std::sin(angle);
			dirZ = std::tan(angle);
			break;
		case FIREWORK3:
			dirX = std::cos(angle);
			dirY = std::sin(angle);
			dirZ = std::cos(angle) * std::sin(angle) * std::tan(angle);
			break;
		}

		auto* p = model->clone();
		setParticleColor(p);
		p->reTransform(deadP->getPos(), Vector3(dirX, dirY, dirZ));

		mGenerator.push_back(p);
	}

	return mGenerator;
}