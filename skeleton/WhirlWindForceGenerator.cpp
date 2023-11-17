#include "WhirlWindForceGenerator.h"

WhirlWindForceGenerator::WhirlWindForceGenerator(Vector3 WhirlPos, string Name, double Duration, bool Active) :
ParticleDragForceGenerator(7.0f, 0.0f, Name, Duration, Active), whirlPos(WhirlPos) {}

WhirlWindForceGenerator::WhirlWindForceGenerator(const float k, Vector3 WhirlPos, string Name, double Duration, bool Active) :
ParticleDragForceGenerator(k, 0.0f, Name, Duration, Active), whirlPos(WhirlPos) {}

Vector3 WhirlWindForceGenerator::calculateForce(Particle* particle) {

	Vector3 dragF = Vector3(0.0f, 0.0f, 0.0f);
	Vector3 p = particle->getPos();
	Vector3 f = k1 * 
		Vector3(
			-(p.z - whirlPos.z),
			50 - (p.y - whirlPos.y),
			p.x - whirlPos.x
		);

	Vector3 v = particle->getVel() - f;
	float drag_coef = v.normalize();
	drag_coef = k1 * drag_coef + k2 * drag_coef * drag_coef;
	dragF = -v * drag_coef;

	return dragF;
}