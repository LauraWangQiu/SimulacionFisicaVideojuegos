#include "Particle.h"

vector<particleInfo> ParticlesInfo = {
	{ // BASIC
		1.0f,
		10.0f,
		Vector3(0.0f, 0.0f, 0.0f),
		0.99f,
		Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		1.0f,
		0,
		1.0f
	},
	{ // FIRE
		1.0f,
		10.0f,
		Vector3(0.0f, -2.0f, 0.0f),
		0.99f,
		Vector4(1.0f, 0.0f, 0.0f, 1.0f),
		1.0f,
		20,
		1.0f
	},
	{ // FIREWORK
		1.0f,
		100.0f,
		Vector3(0.0f, -50.0f, 0.0f),
		0.99f,
		Vector4(1.0f, 0.0f, 0.0f, 1.0f),
		1.0f,
		10,
		1.0f
	},
	{ // WATER
		1.0f,
		100.0f,
		Vector3(0.0f, -50.0f, 0.0f),
		0.99f,
		Vector4(0.5f, 0.9f, 1.0f, 1.0f),
		1.0f,
		3,
		1.0f
	},
	{ // FOG
		1.0f,
		100.0f,
		Vector3(0.0f, 0.0f, 0.0f),
		0.99f,
		Vector4(1.0f, 1.0f, 1.0f, 0.2f),
		5.0f,
		0,
		5.0f
	},
	{ // CANNON_BALL
		1.0f,
		10.0f,
		Vector3(0.0f, -6.0f, 0.0f),
		0.9f,
		Vector4(0.2f, 0.2f, 0.2f, 1.0f),
		1.0f,
		0,
		1.0f
	},
	{ // TANK_BALL
		200.0f,
		40.0f,
		Vector3(0.0f, -20.0f, 0.0f),
		0.99f,
		Vector4(0.0f, 0.0f, 0.0f, 1.0f),
		1.0f,
		0,
		1.0f
	},
	{ // GUN_BULLET
		2.0f,
		35.0f,
		Vector3(0.0f, -1.0f, 0.0f),
		0.99f,
		Vector4(1.0f, 0.8f, 0.2f, 1.0f),
		1.0f,
		0,
		1.0f
	},
	{ // LASER
		0.1f,
		100.0f,
		Vector3(0.0f, 0.0f, 0.0f),
		0.99f,
		Vector4(0.0f, 0.0f, 1.0f, 1.0f),
		1.0f,
		0,
		1.0f
	}
};

Particle::Particle(ParticleType Type, PxTransform Transform, Vector3 Dir, float Time, PxReal Size, Vector4 Color)
	: particleType(Type), transform(Transform), dir(Dir), time(Time), size(Size), color(Color), numDivisions(0),
		toExplode(false) {

	switch (particleType) {
	case BASIC:
		shape = CreateShape(PxSphereGeometry(size));
		break;
	case FIRE:
		shape = CreateShape(PxSphereGeometry(size));
		break;
	case FIREWORK:
		shape = CreateShape(PxSphereGeometry(size));
		break;
	case WATER:
		shape = CreateShape(PxSphereGeometry(size));
		break;
	case STEAM:
		shape = CreateShape(PxBoxGeometry(size, size, size));
		break;

	case CANNON_BALL:
		shape = CreateShape(PxSphereGeometry(size));
		break;
	case TANK_BALL:
		shape = CreateShape(PxSphereGeometry(size));
		break;
	case GUN_BULLET:
		shape = CreateShape(PxBoxGeometry(size, size, size * 3 / 2));
		break;
	case LASER:
		shape = CreateShape(PxBoxGeometry(size, size, size));
		break;
	default: break;
	}

	setMass(ParticlesInfo[particleType].mass);
	setVel(dir * ParticlesInfo[particleType].velc);
	setAcc(ParticlesInfo[particleType].acc);
	setDamping(ParticlesInfo[particleType].damp);
	setColor(ParticlesInfo[particleType].col);
	setTime(ParticlesInfo[particleType].time);
	setNumDivisions(ParticlesInfo[particleType].numDiv);
	setSize(ParticlesInfo[particleType].size);
	// setGeometry(ParticlesInfo[particleType].geo);
	
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