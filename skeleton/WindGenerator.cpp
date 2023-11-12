#include "WindGenerator.h"

WindGenerator::WindGenerator(Vector3 WindVel, string Name, double Duration) :
	ParticleDragGenerator(Name, Duration), windVel(WindVel), 
	origin(Vector3(0.0f, 0.0f, 0.0f)), size(Vector3(0.0f, 0.0f, 0.0f)) {
#ifdef _DEBUG
	print();
#endif
}

WindGenerator::WindGenerator(const float k1, const float k2, Vector3 WindVel, string Name, double Duration) :
	ParticleDragGenerator(k1, k2, Name, Duration), windVel(WindVel), 
	origin(Vector3(0.0f, 0.0f, 0.0f)), size(Vector3(0.0f, 0.0f, 0.0f)) {
#ifdef _DEBUG
	print();
#endif
}

WindGenerator::WindGenerator(Vector3 WindVel, Vector3 Origin, Vector3 Size, string Name, double Duration) :
	ParticleDragGenerator(Name, Duration), windVel(WindVel), origin(Origin), size(Size) {
#ifdef _DEBUG
	print();
#endif
}

WindGenerator::WindGenerator(const float k1, const float k2, Vector3 WindVel, Vector3 Origin, Vector3 Size, string Name, double Duration) :
	ParticleDragGenerator(k1, k2, Name, Duration), windVel(WindVel), origin(Origin), size(Size) {
#ifdef _DEBUG
	print();
#endif
}

float WindGenerator::getCdByShape(string shapeName) {
	if (shapeName == "Sphere") return 0.5f;
	else if (shapeName == "Cube") return 1.0f;
	else return 0.1f;
}

Vector3 WindGenerator::calculateForce(Particle* particle) {

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

	// Invertir la dirección de la fuerza si el viento sopla hacia arriba o abajo
	if (windVel.y != 0.0f) {
		dragF.y *= -1.0f;
	}

	return dragF;
}

void WindGenerator::updateForce(Particle* particle) {
	if ((size.x != 0 && size.y != 0 && size.z != 0) &&
		(particle->getPosX() > (origin.x + size.x / 2) || particle->getPosX() < (origin.x - size.x / 2) ||
			particle->getPosY() > (origin.y + size.y / 2) || particle->getPosY() < (origin.y - size.y / 2) ||
			particle->getPosZ() > (origin.z + size.z / 2) || particle->getPosZ() < (origin.z - size.z / 2))) {
		return;
	}

	ParticleDragGenerator::updateForce(particle);
}

void WindGenerator::clearForce(Particle* particle) {
	if ((size.x != 0 && size.y != 0 && size.z != 0) &&
		(particle->getPosX() > (origin.x + size.x / 2) || particle->getPosX() < (origin.x - size.x / 2) ||
			particle->getPosY() > (origin.y + size.y / 2) || particle->getPosY() < (origin.y - size.y / 2) ||
			particle->getPosZ() > (origin.z + size.z / 2) || particle->getPosZ() < (origin.z - size.z / 2))) {
		return;
	}

	ParticleDragGenerator::clearForce(particle);
}