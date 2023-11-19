#include "ParticleBungeeForceGenerator.h"

ParticleBungeeForceGenerator::ParticleBungeeForceGenerator(const float k, double Length, Particle* Other, string Name, double Duration, bool Active) :
	SpringForceGenerator(k, Length, Other, Name, Duration, Active) {}

Vector3 ParticleBungeeForceGenerator::calculateForce(Particle* particle) {
	Vector3 f = particle->getPos();
	f -= other->getPos();

	// Length
	float length = f.normalize();
	length = (length - restingLength);
	if (length <= 0.0f) {
		return Vector3();
	}
	else {
		f *= -(length * k);
		return f;
	}
}
