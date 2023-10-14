#include "UniformParticleGenerator.h"

UniformParticleGenerator::UniformParticleGenerator(string Name, Vector3 MeanPos, Vector3 MeanVel, double GenerationProbability, int NumParticles, Particle* Model,
	Vector3 VelWidth, Vector3 PosWidth, bool Active) : ParticleGenerator(Name, MeanPos, MeanVel, GenerationProbability, NumParticles, Model, Active),
	velWidth(VelWidth), posWidth(PosWidth) {}

UniformParticleGenerator::~UniformParticleGenerator() {}

list<Particle*> UniformParticleGenerator::generateParticles() {
	list<Particle*> mGenerator;

	if (active) {
		for (int i = 0; i < numParticles; i++) {
			auto* p = model->clone();

			if (generateRandomValue(2) <= generationProbability) {
				p->setPos(model->getPos() + Vector3(u(generator) * posWidth.x, u(generator) * posWidth.y, u(generator) * posWidth.z));
				p->setVel(model->getVel() + Vector3(u(generator) * velWidth.x, u(generator) * velWidth.y, u(generator) * velWidth.z));

				setParticleColor(p);

				mGenerator.push_back(p);
			}
		}
	}

	return mGenerator;
}