#pragma once
#include "Particle.h"
using namespace physx;

class Projectile : public Particle {
public:
	Projectile(currentShotType Type, PxTransform Transform, Vector3 Dir = Vector3(0.0f, 0.0f, 1.0f), float Time = 5.0f, PxReal Size = 1.0f, Vector4 Color = Vector4(255.0, 255.0, 255.0, 255.0));
	~Projectile();
};