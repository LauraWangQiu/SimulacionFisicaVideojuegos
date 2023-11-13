#pragma once
#include "ForceGenerator.h"

class ExplosionForceGenerator : public ForceGenerator {
protected:
	Vector3 expPos;	// origen de la explosión
	double R;		// radio de la explosión
	float K;		// intensidad de la explosión
	float tau;		// constante de tiempo de la explosión
	float expVelc = EXPANSION_VELOCITY;	// velocidad de expansión (velocidad del sonido en el aire)

public:
	ExplosionForceGenerator(Vector3 ExpPos, double Radio, float ExpInt, float Tau, string Name, double Duration, bool Active = false);

	virtual Vector3 calculateForce(Particle* particle);
};

