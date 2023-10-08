#include "GaussianParticleGenerator.h"

GaussianParticleGenerator::GaussianParticleGenerator(string Name, Vector3 MeanPos, Vector3 MeanVel, double GenerationProbability, int NumParticles, Particle* Model, 
	Vector3 StdDevPos, Vector3 StdDevVel, double StdDevTime, bool Active) : ParticleGenerator(Name, MeanPos, MeanVel, GenerationProbability, NumParticles, Model, Active),
	stdDevPos(StdDevPos), stdDevVel(StdDevVel), stdDevTime(StdDevTime) {
}

GaussianParticleGenerator::~GaussianParticleGenerator() {}

list<Particle*> GaussianParticleGenerator::generateParticles() {

	list<Particle*> mGenerator;

	if (active) {
		for (int i = 0; i < numParticles; i++) {
			auto* p = model->clone();
			
			//p->setPos(model->getPos() + Vector3(normal * stdDevPos.x, normal * stdDevPos.y, normal * stdDevPos.z));
			p->setPos(model->getPos() + Vector3(stdDevPos.x, stdDevPos.y, stdDevPos.z));
			//p->setVel(model->getVel() + Vector3(normal * stdDevVel.x, normal * stdDevVel.y, normal * stdDevVel.z));
			p->setVel(model->getVel() + Vector3(stdDevVel.x, stdDevVel.y, stdDevVel.z));

			int paletteSize;
			switch (model->getParticleType()) {
			case FIREWORK:
				paletteSize = (sizeof(palettes.fireWorkPalette) / sizeof(palettes.fireWorkPalette[0]));
				p->setColor2(palettes.fireWorkPalette[rand() % paletteSize]);
			break;
			case FIRE:
				paletteSize = (sizeof(palettes.firePalette) / sizeof(palettes.firePalette[0]));
				p->setColor2(palettes.firePalette[rand() % paletteSize]);
			break;
			}

			mGenerator.push_back(p);
		}
	}

	return mGenerator;
}