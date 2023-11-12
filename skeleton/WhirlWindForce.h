#pragma once
#include <string>
#include "ParticleDragGenerator.h"
#include "core.hpp"
using namespace std;
using namespace physx;

class WhirlWindForce : public ParticleDragGenerator {
protected:
	Vector3 whirlPos;

public:
	WhirlWindForce(Vector3 WhirlPos, string Name, double Duration, bool Active = false);
	WhirlWindForce(const float k, Vector3 WhirlPos, string Name, double Duration, bool Active = false);

	virtual Vector3 calculateForce(Particle* particle);
};

