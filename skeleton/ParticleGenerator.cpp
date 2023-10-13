#include "ParticleGenerator.h"

ParticleGenerator::ParticleGenerator(string Name, Vector3 MeanPos, Vector3 MeanVel, double GenerationProbability, int NumParticles, Particle* Model, bool Active) :
	name(Name), meanPos(MeanPos), meanVel(MeanVel), generationProbability(GenerationProbability), numParticles(NumParticles), model(Model), active(Active) {
	generator = default_random_engine(rd());
}
ParticleGenerator::~ParticleGenerator() {}