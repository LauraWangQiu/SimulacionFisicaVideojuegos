#include "ParticleGenerator.h"

ParticleGenerator::ParticleGenerator(string Name, Vector3 MeanPos, Vector3 MeanVel, double GenerationProbability, int NumParticles, Particle* Model, bool Active) :
	name(Name), meanPos(MeanPos), meanVel(MeanVel), 
	generationProbability(GenerationProbability), 
	numParticles(NumParticles), actualNumParticles(0),
	model(Model), active(Active) {
	generator = default_random_engine(rd());
}

ParticleGenerator::~ParticleGenerator() {
	delete model;
}

void ParticleGenerator::setParticleColor(Particle* p) {
	switch (model->getParticleType()) {
	case FIREWORK:
		p->setColor2(palettes.fireWorkPalette[rand() % palettes.fireWorkPaletteSize]);
		break; 
	case FIREWORK2:
		p->setColor2(palettes.fireWorkPalette2[rand() % palettes.fireWorkPaletteSize2]);
		break;
	case FIREWORK3:
		p->setColor2(palettes.fireWorkPalette3[rand() % palettes.fireWorkPaletteSize3]);
		break;
	case FIRE:
		p->setColor2(palettes.firePalette[rand() % palettes.firePaletteSize]);
		break;
	case WATER:
		p->setColor2(palettes.waterPalette[rand() % palettes.waterPaletteSize]);
		break;
	}
}