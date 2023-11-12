#include "GravityForceGenerator.h"

GravityForceGenerator::GravityForceGenerator(const Vector3& g, string Name, double Duration, bool Active) : ForceGenerator(Name, Duration, Active), gravity(g) {}

Vector3 GravityForceGenerator::calculateForce(Particle* particle) {
	return gravity * particle->getMass();
}