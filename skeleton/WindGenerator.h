#pragma once
#include <string>
#include "ParticleDragGenerator.h"
#include "core.hpp"
using namespace std;
using namespace physx;

class WindGenerator : public ParticleDragGenerator {
protected:
	float p = 1.225;	// densidad del aire en condiciones normales
	float cd;			// coeficiente aerodinamico (dependiente de la forma del objeto)
	Vector3 windVel;	// velocidad del viento
	Vector3 origin;		// origen del campo de viento
	Vector3 size;		// dimensiones del campo de viento

	float getCdByShape(string shapeName);
	
public:
	WindGenerator(Vector3 WindVel, string Name, double Duration);
	WindGenerator(const float k1, const float k2, Vector3 WindVel, string Name, double Duration);
	WindGenerator(Vector3 WindVel, Vector3 Origin, Vector3 Size, string Name, double Duration);
	WindGenerator(const float k1, const float k2, Vector3 WindVel, Vector3 Origin, Vector3 Size, string Name, double Duration);
	~WindGenerator() {
#ifdef _DEBUG 
		cout << "Se ha eliminado un generador de fuerza de viento\n";
#endif
	}
	virtual void print() { cout << "Se ha generado un generador de fuerza de viento\n"; }

	virtual Vector3 calculateForce(Particle* particle);
	virtual void updateForce(Particle* particle);
	virtual void clearForce(Particle* particle);
};

