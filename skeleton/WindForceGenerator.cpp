#include "WindForceGenerator.h"

WindForceGenerator::WindForceGenerator(Vector3 WindVel, string Name, double Duration, bool Active) :
	ParticleDragForceGenerator(Name, Duration, Active), windVel(WindVel), 
	origin(Vector3(0.0f, 0.0f, 0.0f)), size(Vector3(0.0f, 0.0f, 0.0f)) {}

WindForceGenerator::WindForceGenerator(const float k1, const float k2, Vector3 WindVel, string Name, double Duration, bool Active) :
	ParticleDragForceGenerator(k1, k2, Name, Duration, Active), windVel(WindVel), 
	origin(Vector3(0.0f, 0.0f, 0.0f)), size(Vector3(0.0f, 0.0f, 0.0f)) {}

WindForceGenerator::WindForceGenerator(Vector3 WindVel, Vector3 Origin, Vector3 Size, string Name, double Duration, bool Active) :
	ParticleDragForceGenerator(Name, Duration, Active), windVel(WindVel), origin(Origin), size(Size) {}

WindForceGenerator::WindForceGenerator(const float k1, const float k2, Vector3 WindVel, Vector3 Origin, Vector3 Size, string Name, double Duration, bool Active) :
	ParticleDragForceGenerator(k1, k2, Name, Duration, Active), windVel(WindVel), origin(Origin), size(Size) {}

float WindForceGenerator::getCdByShape(string shapeName) {
	if (shapeName == "Sphere") return 0.5f;
	else if (shapeName == "Cube") return 1.0f;
	else return 0.1f;
}

Vector3 WindForceGenerator::calculateForce(Particle* particle) {

	Vector3 v = particle->getVel();
	Vector3 diff = windVel - v;
	Vector3 dragF;

	if (k2 == 0) {
		// F = k1 * (windVel - v) + k2 * ||windVel - v|| * (windVel - v)
		// como k2 = 0, la segunda parte no haria falta
		dragF = k1 * diff + k2 * diff.magnitude() * diff;
	}
	else {
		// Tarea opcional
		// F = aeff * cd * p * v^2, donde k2 = aeff
		cd = getCdByShape(particle->getShapeName());
		dragF = k2 * cd * p * Vector3(v.y * v.y, v.x * v.x, v.z * v.z);
	}

	// Invertir la direcci�n de la fuerza si el viento sopla hacia arriba o abajo
	if (windVel.y != 0.0f) {
		dragF.y *= -1.0f;
	}

	return dragF;
}

void WindForceGenerator::updateForce(Particle* particle) {
	if ((size.x != 0 && size.y != 0 && size.z != 0) &&
		(particle->getPosX() > (origin.x + size.x / 2) || particle->getPosX() < (origin.x - size.x / 2) ||
			particle->getPosY() > (origin.y + size.y / 2) || particle->getPosY() < (origin.y - size.y / 2) ||
			particle->getPosZ() > (origin.z + size.z / 2) || particle->getPosZ() < (origin.z - size.z / 2))) {
		return;
	}

	ParticleDragForceGenerator::updateForce(particle);
}

void WindForceGenerator::clearForce(Particle* particle) {
	if ((size.x != 0 && size.y != 0 && size.z != 0) &&
		(particle->getPosX() > (origin.x + size.x / 2) || particle->getPosX() < (origin.x - size.x / 2) ||
			particle->getPosY() > (origin.y + size.y / 2) || particle->getPosY() < (origin.y - size.y / 2) ||
			particle->getPosZ() > (origin.z + size.z / 2) || particle->getPosZ() < (origin.z - size.z / 2))) {
		return;
	}

	ParticleDragForceGenerator::clearForce(particle);
}