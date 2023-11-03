#include "GravityForceGenerator.h"


GravityForceGenerator::GravityForceGenerator(const Vector3& g, string Name, double Time, double Duration, bool Active) : ForceGenerator(Name, Time, Duration, Active), gravity(g) {}

void GravityForceGenerator::updateForce(Particle* particle) {

	if (fabs(particle->getInverseMass()) < 1e-10)
		return;

	particle->addForce(gravity * particle->getMass());
}