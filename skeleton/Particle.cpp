#include "Particle.h"

Particle::Particle(currentSimpleParticleType Type, PxTransform Transform, Vector3 Dir, float Time, PxReal Size, Vector4 Color)
	: particleType(Type), transform(Transform), dir(Dir), time(Time), size(Size), color(Color) {

	switch (particleType) {
	case BASIC:
		setMass(1.0f);
		setVel(dir * 10.0f);
		setAcc(0.0f, 0.0f, 0.0f);
		setDamping(0.99f);
		setColor(1.0f, 1.0f, 1.0f);
		shape = CreateShape(PxSphereGeometry(size));
	break;
	default: break;
	}
	
	renderItem = new RenderItem(shape, &transform, color);
}

Particle::Particle(currentShotType Type, PxTransform Transform, Vector3 Dir, float Time, PxReal Size, Vector4 Color)
	: shotType(Type), transform(Transform), dir(Dir), time(Time), size(Size), color(Color) {}

Particle::~Particle() {
	DeregisterRenderItem(renderItem);
}

bool Particle::integrate(double t) {

	if (getInverseMass() <= 0.0f) return false;

	// Actualizamos la velocidad de la particula
	vel += acc * t;

	// Aplicamos el damping
	vel *= powf(damping, t);

	// Actualizamos la posicion de la particula
	transform.p += vel * t;

	// Le vamos restando tiempo de vida
	time -= t;

	return time > 0.0f;
}