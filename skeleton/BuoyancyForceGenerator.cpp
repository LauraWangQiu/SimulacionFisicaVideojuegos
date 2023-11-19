#include "BuoyancyForceGenerator.h"

BuoyancyForceGenerator::BuoyancyForceGenerator(Particle* LiquidParticle, float h, float v, float d, float g, string Name, double Duration, bool Active) :
	ForceGenerator(Name, Duration, Active), liquidParticle(LiquidParticle), height(h), volume(v), liquidDensity(d), gravity(-g) {}

Vector3 BuoyancyForceGenerator::calculateForce(Particle* particle) {
	float h = particle->getPosY();
	float h0 = liquidParticle->getPosY();

	Vector3 f(0.0f, 0.0f, 0.0f);
	float immersed = 0.0f;
	if (h - h0 > height * 0.5f) {
		immersed = 0.0f;
	} else if (h0 - h > height * 0.5f) {
		// Totally immersed
		immersed = 1.0f;
	} else {
		immersed = (h0 - h) / height + 0.5f;
	}
	f.y = liquidDensity * volume * immersed * gravity;

	return f;
}