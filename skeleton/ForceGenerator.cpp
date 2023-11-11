#include "ForceGenerator.h"

ForceGenerator::ForceGenerator(string Name, double Duration) : 
	name(Name), time(0.0f), duration(Duration) {}

void ForceGenerator::updateForce(Particle* particle) {
	if (fabs(particle->getInverseMass()) < 1e-10)
		return;

	particle->addForce(calculateForce(particle));
}

void ForceGenerator::clearForce(Particle* particle) {
	if (fabs(particle->getInverseMass()) < 1e-10)
		return;

	particle->addForce(-calculateForce(particle));
}

bool ForceGenerator::updateTime(double t) {
	time += t;
	return time < duration || duration <= 0.0;
}