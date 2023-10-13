#include "GaussianParticleGenerator.h"
#include <iostream>

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
			
			double randomValue = n(generator);
			//std::cout << randomValue << endl;

			if (randomValue <= generationProbability) {
				p->setPos(model->getPos() + Vector3(n(generator) * stdDevPos.x, n(generator) * stdDevPos.y, n(generator) * stdDevPos.z));
				p->setVel(model->getVel() + Vector3(n(generator) * stdDevVel.x, n(generator) * stdDevVel.y, n(generator) * stdDevVel.z));

				int paletteSize;
				switch (model->getParticleType()) {
				case FIREWORK:
					p->setColor2(palettes.fireWorkPalette[rand() % palettes.fireWorkPaletteSize]);
					break;
				case FIRE:
					p->setColor2(palettes.firePalette[rand() % palettes.firePaletteSize]);
					break;
				case WATER:
					p->setColor2(palettes.waterPalette[rand() % palettes.waterPaletteSize]);
					break;
				}

				mGenerator.push_back(p);
			}
		}
	}

	return mGenerator;
}