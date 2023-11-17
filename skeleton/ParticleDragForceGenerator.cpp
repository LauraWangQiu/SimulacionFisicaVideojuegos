#include "ParticleDragForceGenerator.h"

ParticleDragForceGenerator::ParticleDragForceGenerator(string Name, double Duration, bool Active) : 
	ForceGenerator(Name, Duration, Active), k1(0.5f), k2(0.5f) {}

ParticleDragForceGenerator::ParticleDragForceGenerator(const float k1, const float k2, string Name, double Duration, bool Active) : 
	ForceGenerator(Name, Duration, Active), k1(k1), k2(k2) {}

Vector3 ParticleDragForceGenerator::calculateForce(Particle* particle) {
	// 𝑓𝑑𝑟𝑎𝑔 = -𝒗 * (𝑘1 * |𝒗| + 𝑘2 * |𝒗|^2)
	Vector3 v = particle->getVel();
	float drag_coef = v.normalize();
	Vector3 dragF;
	drag_coef = k1 * drag_coef + k2 * drag_coef * drag_coef;
	dragF = -v * drag_coef;

	return dragF;
}