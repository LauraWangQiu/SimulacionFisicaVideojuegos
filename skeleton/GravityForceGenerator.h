#pragma once
#include "ForceGenerator.h"
#include "core.hpp"
using namespace std;
using namespace physx;

class GravityForceGenerator : public ForceGenerator {
protected: 
	Vector3 gravity;

public:
	GravityForceGenerator(const Vector3& g, string Name, double Duration, bool Active = true);

	virtual void updateForce(Particle* particle);
	virtual void clearForce(Particle* particle);

	// Getters y setters
	inline Vector3 getGravity() const { return gravity; }
	inline void setGravity(Vector3 g) { gravity = g; }
};

