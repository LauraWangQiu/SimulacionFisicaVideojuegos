#include "ExplosionForceGenerator.h"

ExplosionForceGenerator::ExplosionForceGenerator(Vector3 ExpPos, double Radio, float ExpInt, float Tau, string Name, double Duration, bool Active) :
ForceGenerator(Name, Duration, Active), expPos(ExpPos), R(Radio), K(ExpInt), tau(Tau) {}

Vector3 ExplosionForceGenerator::calculateForce(Particle* particle) {

	Vector3 f = Vector3(0.0f, 0.0f, 0.0f);
	Vector3 pos = particle->getPos();
	Vector3 diff = pos - expPos;
	double r = diff.magnitude(); // distacia a la explosión
	// Tarea opcional
	R = expVelc * time;

	if (r < R) {
		f = (K / pow(r, 2)) * diff * exp(-time / tau);
	}

	return f;
}