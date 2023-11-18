#include "SpringForceGenerator.h"

SpringForceGenerator::SpringForceGenerator(const float k, double Length, Particle* Other, string Name, double Duration, bool Active) : 
	ForceGenerator(Name, Duration, Active), k(k), restingLength(Length), other(Other) {}

Vector3 SpringForceGenerator::calculateForce(Particle* particle) {

	// 𝑓 = -k * 𝑥
	Vector3 relativePos = other->getPos() - particle->getPos();
	Vector3 force;

	const float length = relativePos.normalize();
	const float deltaX = length - restingLength;

	force = relativePos * deltaX * k;

	return force;
}