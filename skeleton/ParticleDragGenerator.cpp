#include "ParticleDragGenerator.h"

ParticleDragGenerator::ParticleDragGenerator(string Name, double Duration, bool Active) : 
	ForceGenerator(Name, Duration, Active), k1(0.5f), k2(0.5f) {}

ParticleDragGenerator::ParticleDragGenerator(const float k1, const float k2, string Name, double Duration, bool Active) : 
	ForceGenerator(Name, Duration, Active), k1(k1), k2(k2) {}

void ParticleDragGenerator::updateForce(Particle* particle) {
	if (active) {
		if (fabs(particle->getInverseMass()) < 1e-10)
			return;

		// 𝑓𝑑𝑟𝑎𝑔 = -𝒗 * (𝑘1 * |𝒗| + 𝑘2 * |𝒗|^2)
		Vector3 v = particle->getVel();
		float drag_coef = v.normalize();
		Vector3 dragF;
		drag_coef = k1 * drag_coef + k2 * drag_coef * drag_coef;
		dragF = -v * drag_coef;

#ifdef _DEBUG
		//cout << dragF.x << "\t" << dragF.y << "\t" << dragF.z << "\n";
#endif
		particle->addForce(dragF);
	}
}

void ParticleDragGenerator::clearForce(Particle* particle) {
	if (fabs(particle->getInverseMass()) < 1e-10)
		return;

	Vector3 v = particle->getVel();
	float drag_coef = v.normalize();
	Vector3 dragF;
	drag_coef = k1 * drag_coef + k2 * drag_coef * drag_coef;
	dragF = -v * drag_coef;

	particle->addForce(-dragF);
}