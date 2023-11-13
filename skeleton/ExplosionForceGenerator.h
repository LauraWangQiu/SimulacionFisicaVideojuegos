#pragma once
#include "ForceGenerator.h"

class ExplosionForceGenerator : public ForceGenerator {
protected:
	Vector3 expPos;	// origen de la explosi�n
	double R;		// radio de la explosi�n
	float K;		// intensidad de la explosi�n
	float tau;		// constante de tiempo de la explosi�n
	float expVelc = EXPANSION_VELOCITY;	// velocidad de expansi�n (velocidad del sonido en el aire)

public:
	ExplosionForceGenerator(Vector3 ExpPos, double Radio, float ExpInt, float Tau, string Name, double Duration, bool Active = false);

	virtual Vector3 calculateForce(Particle* particle);
};

