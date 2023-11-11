#include "GravityForceGenerator.h"

GravityForceGenerator::GravityForceGenerator(const Vector3& g, string Name, double Duration) : ForceGenerator(Name, Duration), gravity(g) {
#ifdef _DEBUG
	print();
#endif
}

Vector3 GravityForceGenerator::calculateForce(Particle* particle) {
	return gravity * particle->getMass();
}