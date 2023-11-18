#pragma once
#include "ForceGenerator.h"
using namespace physx;

class SpringForceGenerator : public ForceGenerator {
protected:
	float k;
	double restingLength;
	Particle* other;

public:
	SpringForceGenerator(const float k, double Length, Particle* Other, string Name, double Duration, bool Active = false);

	virtual Vector3 calculateForce(Particle* particle) override;

	// Getters y setters
	inline float getK() const { return k; }
	inline void setK(const float k) { this->k = k; }
	inline double getLength() const { return restingLength; }
	inline void setLength(double Length) { restingLength = Length; }
	inline Particle* getOther() const { return other; }
	inline void setOther(Particle* Other) { other = Other; }
};