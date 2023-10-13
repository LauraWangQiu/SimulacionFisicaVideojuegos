#pragma once
#include <list>
#include "RenderUtils.hpp"
#include "Particle.h"
#include "ParticleGenerator.h"
using namespace std;
using namespace physx;


class GaussianParticleGenerator : public ParticleGenerator {
protected:
	Vector3 stdDevPos, stdDevVel;
	double stdDevTime;

	particlePalettes palettes;

public:
	/// <summary> Generador de partículas gaussianas </summary>
	/// <param name="**Name**"> Nombre del generador </param>
	/// <param name="**MeanPos**"> Origen de las partículas </param>
	/// <param name="**GenerationProbability**"> Probabilidad de generar partícula </param>
	/// <param name="**NumParticles**"> Número de partículas totales </param>
	/// <param name="**Model**"> Número de partículas totales </param>
	/// <param name="**StdDevPos**"> Area base </param>
	/// <param name="**StdDevVel**"> Amplitud </param>
	/// <param name="**StdDevTime**"> Tiempo de regeneración </param>
	/// <param name="**Active** (Opcional)"> Activación/Desactivación del generador </param>
	GaussianParticleGenerator(string Name, Vector3 MeanPos, Vector3 MeanVel, double GenerationProbability, int NumParticles, Particle* Model, 
		Vector3 StdDevPos, Vector3 StdDevVel, double StdDevTime, bool Active = true);
	~GaussianParticleGenerator();

	/// <summary> Generador de partículas gaussianas </summary>
	/// <param name="StdDevPos"> Area base </param>
	/// <param name="StdDevVel"> Amplitud </param>
	/// <returns> void </returns>
	virtual list<Particle*> generateParticles();

	// Getters
	Vector3 getStdDevPos() const { return stdDevPos; }
	Vector3 getStdDevVel() const { return stdDevVel; }
	double getStdDevTime() const { return stdDevTime; }
	// Setters
	void setStdDevPos(Vector3 StdDevPos) { stdDevPos = StdDevPos; }
	void setStdDevVel(Vector3 StdDevVel) { stdDevVel = StdDevVel; }
	void setStdDevTime(double StdDevTime) { stdDevTime = StdDevTime; }
};

