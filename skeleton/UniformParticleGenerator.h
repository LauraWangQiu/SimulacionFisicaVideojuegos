#pragma once
#include <list>
#include <time.h>
#include <list>
#include "RenderUtils.hpp"
#include "Particle.h"
#include "ParticleGenerator.h"
using namespace std;
using namespace physx;

class UniformParticleGenerator : public ParticleGenerator {
protected:
	Vector3 velWidth, posWidth;

public:
	/// <summary> Generador de partículas uniformes </summary>
	/// <param name="**Name**"> Nombre del generador </param>
	/// <param name="**MeanPos**"> Origen de las partículas </param>
	/// <param name="**GenerationProbability**"> Probabilidad de generar partícula </param>
	/// <param name="**NumParticles**"> Número de partículas totales </param>
	/// <param name="**Model**"> Número de partículas totales </param>
	/// <param name="**VelWidth**"> Amplitud </param>
	/// <param name="**PosWidth**"> Area base </param>
	/// <param name="**Active** (Opcional)"> Activación/Desactivación del generador </param>
	UniformParticleGenerator(string Name, Vector3 MeanPos, Vector3 MeanVel, double GenerationProbability, int NumParticles, Particle* Model,
		Vector3 VelWidth, Vector3 PosWidth, bool Active = true);
	~UniformParticleGenerator();

	list<Particle*> generateParticles();

	// Getters
	inline Vector3 getVelWidth() const { return velWidth; }
	inline Vector3 getPosWidth() const { return posWidth; }
	// Setters
	inline void setVelWidth(Vector3 VelWidth) { velWidth = VelWidth; }
	inline void setPosWidth(Vector3 PosWidth) { posWidth = PosWidth; }
};

