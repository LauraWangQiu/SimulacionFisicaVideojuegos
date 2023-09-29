#include <iostream>
#include "Particle.h"

Particle::Particle(Vector3 Pos, float Mass, Vector3 Vel, Vector3 Acc, float Time, PxReal Size, Vector4 Color, float Damp) : transform(Pos), mass(Mass), vel(Vel), acc(Acc), time(Time), size(Size), color(Color), damping(Damp) {
	shotType = NONE;
	shape = CreateShape(PxSphereGeometry(size));
	renderItem = new RenderItem(shape, &transform, color);
}

Particle::Particle(currentShotType Type, Vector3 Pos, float Time, PxReal Size, Vector4 Color) : shotType(Type), transform(Pos), time(Time), size(Size), color(Color) {

	switch (shotType) {
		case NONE:
			setMass(1.0f);
			setVel(0.0f, 10.0f, 0.0f);
			setAcc(0.0f, 3.0f, 0.0f);
			setDamping(0.998f);
			shape = CreateShape(PxSphereGeometry(size));
		break;
		case CANNON_BALL:
			setMass(1.0f);
			setVel(0.0f, 0.0f, 10.0f);
			setAcc(0.0f, 6.0f, 0.0f);
			setDamping(0.9f);
			setColor(0.2f, 0.2f, 0.2f);
			shape = CreateShape(PxSphereGeometry(size));
		break;
		case TANK_BALL:
			setMass(200.0f);
			setVel(0.0f, 30.0f, 40.0f);
			setAcc(0.0f, 20.0f, 0.0f);
			setDamping(0.99f);
			setColor(0.0f, 0.0f, 0.0f);
			shape = CreateShape(PxSphereGeometry(size));
		break;
		case GUN_BULLET: 
			setMass(2.0f);
			setVel(0.0f, 0.0f, 35.0f);
			setAcc(0.0f, -1.0f, 0.0f);
			setDamping(0.99f);
			setColor(0.5f, 0.2f, 0.2f);
			shape = CreateShape(PxBoxGeometry(size, size, size));
		break;
		case LASER:
			setMass(0.1f);
			setVel(0.0f, 0.0f, 100.0f);
			setAcc(0.0f, 0.0f, 0.0f);
			setDamping(0.99f);
			setColor(0.0f, 0.0f, 1.0f);
			shape = CreateShape(PxBoxGeometry(size, size, size));
		break;
	}

	renderItem = new RenderItem(shape, &transform, color);
}

Particle::~Particle() {
	DeregisterRenderItem(renderItem);
}

bool Particle::integrate(double t) {

	if (getInverseMass() <= 0.0f) return false;

	// Actualizamos la posicion de la particula
	transform.p += vel * t;

	// Actualizamos la velocidad de la particula
	vel += acc * t;

	// Aplicamos el damping
	vel *= powf(damping, t);

	// Le vamos restando tiempo de vida
	time -= t;

	return time < 0.0f;
}