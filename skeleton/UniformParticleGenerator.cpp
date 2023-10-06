#include "UniformParticleGenerator.h"

UniformParticleGenerator::UniformParticleGenerator(string Name, Vector3 MeanPos, Vector3 MeanVel, double GenerationProbability, int NumParticles, Particle* Model,
	Vector3 VelWidth, Vector3 PosWidth) : ParticleGenerator(Name, MeanPos, MeanVel, GenerationProbability, NumParticles, Model), 
	velWidth(VelWidth), posWidth(PosWidth) {}

UniformParticleGenerator::~UniformParticleGenerator() {}

list<Particle*> UniformParticleGenerator::generateParticles() {
	list<Particle*> mGenerator;

	/* Generar particulas uniformes y anadirlas a la lista */

	return mGenerator;
}