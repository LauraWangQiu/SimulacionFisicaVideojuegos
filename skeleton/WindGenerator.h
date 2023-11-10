#pragma once
#include <string>
#include "ParticleDragGenerator.h"
#include "core.hpp"
using namespace std;
using namespace physx;

class WindGenerator : public ParticleDragGenerator {
protected:
	Vector3 windVel;

public:
	WindGenerator(string Name, Vector3 WindVel, double Duration, bool Active = true);
	WindGenerator(const float k1, const float k2, Vector3 WindVel, string Name, double Duration, bool Active = true);

	virtual void updateForce(Particle* particle);
	virtual void clearForce(Particle* particle);
};

