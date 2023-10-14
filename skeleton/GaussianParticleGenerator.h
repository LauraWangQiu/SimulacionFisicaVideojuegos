#pragma once
#include <list>
#include "RenderUtils.hpp"
#include "Particle.h"
#include "ParticleGenerator.h"
#include "Firework.h"
using namespace std;
using namespace physx;


class GaussianParticleGenerator : public ParticleGenerator {
protected:
	Vector3 stdDevPos, stdDevVel;
	double stdDevTime;

public:
	/// <summary> Generador de part�culas gaussianas </summary>
	/// <param name="**Name**"> Nombre del generador </param>
	/// <param name="**MeanPos**"> Origen de las part�culas </param>
	/// <param name="**GenerationProbability**"> Probabilidad de generar part�cula </param>
	/// <param name="**NumParticles**"> N�mero de part�culas totales </param>
	/// <param name="**Model**"> N�mero de part�culas totales </param>
	/// <param name="**StdDevPos**"> Area base </param>
	/// <param name="**StdDevVel**"> Amplitud </param>
	/// <param name="**StdDevTime**"> Tiempo de regeneraci�n </param>
	/// <param name="**Active** (Opcional)"> Activaci�n/Desactivaci�n del generador </param>
	GaussianParticleGenerator(string Name, Vector3 MeanPos, Vector3 MeanVel, double GenerationProbability, int NumParticles, Particle* Model, 
		Vector3 StdDevPos, Vector3 StdDevVel, double StdDevTime, bool Active = true);
	~GaussianParticleGenerator();

	/// <summary> Generador de part�culas gaussianas </summary>
	/// <param name="StdDevPos"> Area base </param>
	/// <param name="StdDevVel"> Amplitud </param>
	/// <returns> void </returns>
	virtual list<Particle*> generateParticles();

	// Getters
	inline Vector3 getStdDevPos() const { return stdDevPos; }
	inline Vector3 getStdDevVel() const { return stdDevVel; }
	inline double getStdDevTime() const { return stdDevTime; }
	// Setters
	inline void setStdDevPos(Vector3 StdDevPos) { stdDevPos = StdDevPos; }
	inline void setStdDevVel(Vector3 StdDevVel) { stdDevVel = StdDevVel; }
	inline void setStdDevTime(double StdDevTime) { stdDevTime = StdDevTime; }
};

