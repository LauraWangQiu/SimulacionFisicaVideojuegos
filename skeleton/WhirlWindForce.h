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
	WhirlWindForce(Vector3 WhirlPos, string Name, double Duration);
	WhirlWindForce(const float k, Vector3 WhirlPos, string Name, double Duration);
	~WhirlWindForce() {
#ifdef _DEBUG 
		cout << "Se ha eliminado un generador de fuerza de torbellinos\n";
#endif
	}

	virtual void print() { cout << "Se ha generado un generador de fuerza de torbellinos\n"; }

	virtual Vector3 calculateForce(Particle* particle);
};

