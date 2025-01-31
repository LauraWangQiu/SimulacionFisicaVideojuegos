#pragma once
#include "ForceGenerator.h"
#include "core.hpp"
using namespace std;
using namespace physx;

class ParticleDragForceGenerator : public ForceGenerator {
protected:
	float k1, k2;

public:
	ParticleDragForceGenerator(string Name, double Duration, bool Active = false);
	ParticleDragForceGenerator(const float k1, const float k2, string Name, double Duration, bool Active = false);

	virtual Vector3 calculateForce(Particle* particle);

	// Getters y setters
	inline float getK1() const { return k1; }
	inline float getK2() const { return k2; }
	inline void setK1(float k1) { k1 = k1; }
	inline void setK2(float k2) { k2 = k2; }
	inline pair<float, float> getDrag() const { return pair<float, float>(k1, k2); }
	inline void setDrag(float k1, float k2) { k1 = k1; k2 = k2; }
};

