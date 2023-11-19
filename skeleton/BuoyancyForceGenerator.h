#pragma once
#include "ForceGenerator.h"
#include "core.hpp"

class BuoyancyForceGenerator : public ForceGenerator {
protected:
	float height, volume, liquidDensity, gravity;
	Particle* liquidParticle;

public:
	BuoyancyForceGenerator(Particle* liquidParticle, float h, float v, float d, float g, string Name, double Duration, bool Active = false);

	virtual Vector3 calculateForce(Particle* particle) override;
};

