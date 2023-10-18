#include "ParticleGenerator.h"

ParticleGenerator::ParticleGenerator(string Name, Vector3 MeanPos, Vector3 MeanVel, double GenerationProbability, int NumParticles, Particle* Model, bool Active) :
	name(Name), meanPos(MeanPos), meanVel(MeanVel), generationProbability(GenerationProbability), numParticles(NumParticles), model(Model), active(Active) {
	generator = default_random_engine(rd());
}
ParticleGenerator::~ParticleGenerator() {}

list<Particle*> ParticleGenerator::generateParticles(Particle* deadP) {
	list<Particle*> mGenerator;

	switch (model->getParticleType()) {
	//case FIREWORK:
	default:
		for (int i = 0; i < model->getNumDivisions(); ++i) {
			// Calcula el ángulo
			double angle = 2.0 * M_PI * (i + 1) / model->getNumDivisions();

			// Calcula las componentes x, y y z de la dirección basadas en el ángulo.
			double dirX = std::cos(angle);
			double dirY = std::sin(angle);
			double dirZ = 0.0;

			auto* p = model->clone();
			setParticleColor(p);
			p->reTransform(deadP->getPos(), Vector3(dirX, dirY, dirZ));

			mGenerator.push_back(p);
		}
		break;
	}

	return mGenerator;
}

void ParticleGenerator::setParticleColor(Particle* p) {
	switch (p->getParticleType()) {
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
}