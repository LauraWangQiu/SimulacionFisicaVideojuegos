#pragma once
#include "SpringForceGenerator.h"
#include "core.hpp"

class ParticleBungeeForceGenerator : public SpringForceGenerator {
protected:

public:
	ParticleBungeeForceGenerator(const float k, double Length, Particle* Other, string Name, double Duration, bool Active = false);

	virtual Vector3 calculateForce(Particle* particle) override;
};

