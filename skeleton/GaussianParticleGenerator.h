#pragma once
#include <list>
#include <cmath>
#include "RenderUtils.hpp"
#include "Particle.h"
#include "ParticleGenerator.h"
#include "Firework.h"
using namespace std;
using namespace physx;

class GaussianParticleGenerator : public ParticleGenerator {
protected:
	Vector3 stdDevPos, stdDevVel;

	normal_distribution<double> n1{ -0.1, 0.1 };
	normal_distribution<double> n2{ 0.2, 0.5 };

public:
	/// <summary> Generador de part�culas gaussianas </summary>
	/// <param name="**Name**"> Nombre del generador </param>
	/// <param name="**MeanPos**"> Origen de las part�culas </param>
	/// <param name="**MeanVel**"> Velocidad de las part�culas </param>
	/// <param name="**GenerationProbability**"> Probabilidad de generar part�cula </param>
	/// <param name="**NumParticles**"> N�mero de part�culas totales </param>
	/// <param name="**Model**"> N�mero de part�culas totales </param>
	/// <param name="**StdDevPos**"> Area base </param>
	/// <param name="**StdDevVel**"> Amplitud </param>
	/// <param name="**Active** (Opcional)"> Activaci�n/Desactivaci�n del generador </param>
	GaussianParticleGenerator(string Name, Vector3 MeanPos, Vector3 MeanVel, double GenerationProbability, int NumParticles, Particle* Model, 
		Vector3 StdDevPos, Vector3 StdDevVel, bool Active = true);
	~GaussianParticleGenerator();

	virtual list<Particle*> generateParticles();
	virtual list<Particle*> generateParticles(Particle* deadP);

	// Getters
	inline Vector3 getStdDevPos() const { return stdDevPos; }
	inline Vector3 getStdDevVel() const { return stdDevVel; }
	// Setters
	inline void setStdDevPos(Vector3 StdDevPos) { stdDevPos = StdDevPos; }
	inline void setStdDevVel(Vector3 StdDevVel) { stdDevVel = StdDevVel; }
};

