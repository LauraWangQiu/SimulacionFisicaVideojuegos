#include "ParticleGenerator.h"

ParticleGenerator::ParticleGenerator(string Name, Vector3 MeanPos, Vector3 MeanVel, double GenerationProbability, int NumParticles, Particle* Model) :
name(Name), meanPos(MeanPos), meanVel(MeanVel), generationProbability(GenerationProbability), numParticles(NumParticles), model(Model) {}

ParticleGenerator::~ParticleGenerator() {}