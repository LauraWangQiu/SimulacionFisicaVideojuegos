#include "WhirlWindForce.h"

WhirlWindForce::WhirlWindForce(Vector3 WhirlPos, string Name, double Duration) : 
ParticleDragGenerator(7.0f, 0.0f, Name, Duration), whirlPos(WhirlPos) {
#ifdef _DEBUG
	print();
#endif
}

WhirlWindForce::WhirlWindForce(const float k, Vector3 WhirlPos, string Name, double Duration) :
ParticleDragGenerator(k, 0.0f, Name, Duration), whirlPos(WhirlPos) {
#ifdef _DEBUG
	print();
#endif
}

Vector3 WhirlWindForce::calculateForce(Particle* particle) {

	Vector3 dragF = Vector3(0.0f, 0.0f, 0.0f);
	Vector3 p = particle->getPos();
	Vector3 f = k1 * Vector3(
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