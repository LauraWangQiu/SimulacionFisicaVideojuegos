#pragma once
#include <string>
#include "ParticleDragForceGenerator.h"
#include "core.hpp"
using namespace std;
using namespace physx;

class WindForceGenerator : public ParticleDragForceGenerator {
protected:
	float p = AIR_DENSITY;	// densidad del aire en condiciones normales
	float cd;				// coeficiente aerodinamico (dependiente de la forma del objeto)
	Vector3 windVel;		// velocidad del viento
	Vector3 origin;			// origen del campo de viento
	Vector3 size;			// dimensiones del campo de viento

	float getCdByShape(string shapeName);
	
public:
	WindForceGenerator(Vector3 WindVel, string Name, double Duration, bool Active = false);
	WindForceGenerator(const float k1, const float k2, Vector3 WindVel, string Name, double Duration, bool Active = false);
	WindForceGenerator(Vector3 WindVel, Vector3 Origin, Vector3 Size, string Name, double Duration, bool Active = false);
	WindForceGenerator(const float k1, const float k2, Vector3 WindVel, Vector3 Origin, Vector3 Size, string Name, double Duration, bool Active = false);

	virtual Vector3 calculateForce(Particle* particle) override;
	virtual void updateForce(Particle* particle) override;
	virtual void clearForce(Particle* particle) override;

	// Getters y setters
	inline Vector3 getWindVel() const { return windVel; }
	inline void setWindVel(Vector3 WindVel) { windVel = WindVel; }
	inline Vector3 getOrigin() const { return origin; }
	inline void setOrigin(Vector3 Origin) { origin = Origin; }
	inline Vector3 getSize() const { return size; }
	inline void setSize(Vector3 Size) { size = Size; }
};

