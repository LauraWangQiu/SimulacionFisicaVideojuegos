#include "WindGenerator.h"

WindGenerator::WindGenerator(string Name, Vector3 WindVel, double Duration, bool Active) :
	ParticleDragGenerator(0.5f, 0.0f, Name, Duration, Active), windVel(WindVel) {}

WindGenerator::WindGenerator(const float k1, const float k2, Vector3 WindVel, string Name, double Duration, bool Active) :
	ParticleDragGenerator(k1, k2, Name, Duration, Active), windVel(WindVel) {}

void WindGenerator::updateForce(Particle* particle) {
	if (active) {
		
		if (fabs(particle->getInverseMass()) < 1e-10)
			return;

		// p = k1 * |w - v|(w - v) + k2 * |w - v|(w - v)
		Vector3 v = particle->getVel();
		Vector3 diff = windVel - v;
		Vector3 dragF = k1 * diff.normalize() * diff + k2 * diff.normalize() * diff;

#ifdef _DEBUG
		//cout << "Wind: " << dragF.x << "\t" << dragF.y << "\t" << dragF.z << "\n";
#endif
		particle->addForce(dragF);
	}
}

void WindGenerator::clearForce(Particle* particle) {
	if (fabs(particle->getInverseMass()) < 1e-10)
		return;

	Vector3 v = particle->getVel();
	float drag_coef = v.normalize();
	Vector3 dragF;
	drag_coef = k1 * drag_coef + k2 * drag_coef * drag_coef;
	dragF = -v * drag_coef;

	particle->addForce(-dragF);
}