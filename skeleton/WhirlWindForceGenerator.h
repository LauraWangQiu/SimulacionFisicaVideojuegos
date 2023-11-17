#pragma once
#include <string>
#include "ParticleDragForceGenerator.h"
#include "core.hpp"
using namespace std;
using namespace physx;

class WhirlWindForceGenerator : public ParticleDragForceGenerator {
protected:
	Vector3 whirlPos;

public:
	WhirlWindForceGenerator(Vector3 WhirlPos, string Name, double Duration, bool Active = false);
	WhirlWindForceGenerator(const float k, Vector3 WhirlPos, string Name, double Duration, bool Active = false);

	virtual Vector3 calculateForce(Particle* particle) override;
};

