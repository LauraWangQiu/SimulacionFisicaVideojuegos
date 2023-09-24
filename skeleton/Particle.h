// ACTIVIDAD 1A
#pragma once
#include "RenderUtils.hpp"
using namespace physx;
using namespace std;

class Particle
{
public:
	Particle(Vector3 Pos, Vector3 Vel);
	~Particle();

	void integrate(double t);

private: 
	Vector3 vel;
	physx::PxTransform pos; // A render item le pasaremos la direccion de este pose, para que se actuzalice automaticamente
	RenderItem* renderItem;
};
// ============
