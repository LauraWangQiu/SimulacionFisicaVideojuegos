#pragma once
#include <list>
#include <time.h>
#include <list>
#include "RenderUtils.hpp"
#include "Particle.h"
#include "ParticleGenerator.h"
#include "Firework.h"
using namespace std;
using namespace physx;

class UniformParticleGenerator : public ParticleGenerator {
protected:
	Vector3 velWidth, posWidth;

	uniform_real_distribution<double> u1{ -1.0, 1.0 };
	uniform_real_distribution<double> u2{ -5.0, 5.0 };	

public:
	/// <summary> Generador de part�culas uniformes </summary>
	/// <param name="**Name**"> Nombre del generador </param>
	/// <param name="**MeanPos**"> Origen de las part�culas </param>
	/// <param name="**MeanVel**"> Velocidad de las part�culas </param>
	/// <param name="**GenerationProbability**"> Probabilidad de generar part�cula </param>
	/// <param name="**NumParticles**"> N�mero de part�culas totales </param>
	/// <param name="**Model**"> N�mero de part�culas totales </param>
	/// <param name="**VelWidth**"> Amplitud </param>
	/// <param name="**PosWidth**"> Area base </param>
	/// <param name="**Active** (Opcional)"> Activaci�n/Desactivaci�n del generador </param>
	UniformParticleGenerator(string Name, Vector3 MeanPos, Vector3 MeanVel, double GenerationProbability, int NumParticles, Particle* Model,
		Vector3 VelWidth, Vector3 PosWidth, bool Active = false);
	~UniformParticleGenerator();

	virtual list<Particle*> generateParticles();
	virtual list<Particle*> generateParticles(Particle* deadP);

	// Getters
	inline Vector3 getVelWidth() const { return velWidth; }
	inline Vector3 getPosWidth() const { return posWidth; }
	// Setters
	inline void setVelWidth(Vector3 VelWidth) { velWidth = VelWidth; }
	inline void setPosWidth(Vector3 PosWidth) { posWidth = PosWidth; }
};

