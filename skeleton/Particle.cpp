#include "Particle.h"

Particle::Particle(ParticleType Type, PxTransform Transform, Vector3 Dir, float Time, PxReal Size, Vector4 Color)
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
	case FIRE:
		setMass(1.0f);
		setDir(0.0f, 1.0f, 0.0f);
		setVel(dir * 10.0f);
		setAcc(0.0f, 0.0f, 0.0f);
		setDamping(0.99f);
		setColor(1.0f, 0.0f, 0.0f);
		setTime(2.5f);
		shape = CreateShape(PxSphereGeometry(size));
	break;
	case FIREWORK:
		setMass(1.0f);
		setDir(0.0f, 1.0f, 0.0f);
		setVel(dir * 100.0f);
		setAcc(0.0f, 0.0f, 0.0f);
		setDamping(0.99f);
		setColor(1.0f, 0.0f, 0.0f);
		shape = CreateShape(PxSphereGeometry(size));
		break;
	case WATER:
		setMass(1.0f);
		setDir(0.0f, -1.0f, 0.0f);
		setVel(dir * 100.0f);
		setAcc(0.0f, 0.0f, 0.0f);
		setDamping(0.99f);
		setColor(0.5f, 0.9f, 1.0f);
		shape = CreateShape(PxSphereGeometry(size));
		break;

	case CANNON_BALL:
		setMass(1.0f);
		setVel(dir * 10.0f);
		setAcc(0.0f, -6.0f, 0.0f);
		setDamping(0.9f);
		setColor(0.2f, 0.2f, 0.2f);
		shape = CreateShape(PxSphereGeometry(size));
		break;
	case TANK_BALL:
		setMass(200.0f);
		setVel(dir * 40.0f);
		setAcc(0.0f, -20.0f, 0.0f);
		setDamping(0.99f);
		setColor(0.0f, 0.0f, 0.0f);
		shape = CreateShape(PxSphereGeometry(size));
		break;
	case GUN_BULLET:
		setMass(2.0f);
		setVel(dir * 35.0f);
		setAcc(0.0f, -1.0f, 0.0f);
		setDamping(0.99f);
		setColor(1.0f, 0.8f, 0.2f);
		shape = CreateShape(PxBoxGeometry(size, size, size * 3 / 2));
		break;
	case LASER:
		setMass(0.1f);
		setVel(dir * 100.0f);
		setAcc(0.0f, 0.0f, 0.0f);
		setDamping(0.99f);
		setColor(0.0f, 0.0f, 1.0f);
		shape = CreateShape(PxBoxGeometry(size, size, size));
		break;
	default: break;
	}
	
	renderItem = new RenderItem(shape, &transform, color);
}

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

Particle* Particle::clone() const {
	return new Particle(particleType, transform, dir, time, size, color);
}