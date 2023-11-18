#include "Particle.h"

vector<particleInfo> ParticlesInfo = {
	{ // BASIC
		1.0f,
		20.0f,
		Vector3(0.0f, 0.0f, 0.0f),
		0.99f,
		Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		5.0f,
		0,
		0,
		Vector3(1.0f, 1.0f, 1.0f),
		"Sphere"
	},
	{ // FIREWORK
		1.0f,
		100.0f,
		Vector3(0.0f, -50.0f, 0.0f),
		0.99f,
		Vector4(1.0f, 0.0f, 0.0f, 1.0f),
		1.0f,
		10,
		2,
		Vector3(1.0f, 1.0f, 1.0f),
		"Sphere"
	},
	{ // FIREWORK2
		1.0f,
		100.0f,
		Vector3(0.0f, -50.0f, 0.0f),
		0.99f,
		Vector4(1.0f, 0.0f, 0.0f, 1.0f),
		1.0f,
		50,
		1,
		Vector3(1.0f, 1.0f, 1.0f),
		"Sphere"
	},
	{ // FIREWORK3
		1.0f,
		100.0f,
		Vector3(0.0f, -50.0f, 0.0f),
		0.99f,
		Vector4(1.0f, 0.0f, 0.0f, 1.0f),
		1.0f,
		20,
		2,
		Vector3(1.0f, 1.0f, 1.0f),
		"Sphere"
	},
	{ // FIRE
		1.0f,
		10.0f,
		Vector3(0.0f, 0.0f, 0.0f),
		0.99f,
		Vector4(1.0f, 0.0f, 0.0f, 1.0f),
		1.0f,
		0,
		0,
		Vector3(1.0f, 1.0f, 1.0f),
		"Sphere"
	},
	{ // WATER
		1.0f,
		100.0f,
		Vector3(0.0f, -50.0f, 0.0f),
		0.99f,
		Vector4(0.5f, 0.9f, 1.0f, 1.0f),
		10.0f,
		0,
		0,
		Vector3(1.0f, 1.0f, 1.0f),
		"Sphere"
	},
	{ // STEAM
		1.0f,
		100.0f,
		Vector3(0.0f, 0.0f, 0.0f),
		0.99f,
		Vector4(1.0f, 1.0f, 1.0f, 0.2f),
		5.0f,
		0,
		0,
		Vector3(5.0f, 5.0f, 5.0f),
		"Cube"
	},
	{ // CANNON_BALL
		100.0f,
		10.0f,
		Vector3(0.0f, -6.0f, 0.0f),
		0.9f,
		Vector4(0.2f, 0.2f, 0.2f, 1.0f),
		3.0f,
		0,
		0,
		Vector3(1.0f, 1.0f, 1.0f),
		"Sphere"
	},
	{ // TANK_BALL
		200.0f,
		40.0f,
		Vector3(0.0f, -20.0f, 0.0f),
		0.99f,
		Vector4(0.0f, 0.0f, 0.0f, 1.0f),
		3.0f,
		0,
		0,
		Vector3(1.0f, 1.0f, 1.0f),
		"Sphere"
	},
	{ // GUN_BULLET
		2.0f,
		35.0f,
		Vector3(0.0f, -1.0f, 0.0f),
		0.99f,
		Vector4(1.0f, 0.8f, 0.2f, 1.0f),
		3.0f,
		0,
		0,
		Vector3(1.0f, 1.0f, 1.0f),
		"Cube"
	},
	{ // LASER
		0.1f,
		100.0f,
		Vector3(0.0f, 0.0f, 0.0f),
		0.99f,
		Vector4(0.0f, 0.0f, 1.0f, 1.0f),
		3.0f,
		0,
		0,
		Vector3(1.0f, 1.0f, 1.0f),
		"Cube"
	},
	{ // WIND
		0.5f,
		50.0f,
		Vector3(0.0f, 0.0f, 0.0f),
		0.99f,
		Vector4(0.8f, 0.9f, 1.0f, 1.0f),
		10.0f,
		0,
		0,
		Vector3(1.0f, 1.0f, 1.0f),
		"Sphere"
	},
	{ // EXPLOSION
		1.0f,
		33.75f,
		Vector3(0.0f, 0.0f, 0.0f),
		0.99f,
		Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		3.0f,
		0,
		0,
		Vector3(1.0f, 1.0f, 1.0f),
		"Cube"
	},
	{ // SPRING
		20.0f,
		0.0f,
		Vector3(0.0f, 0.0f, 0.0f),
		0.99f,
		Vector4(1.0f, 0.0f, 0.8f, 1.0f),
		0.0f,
		0,
		0,
		Vector3(1.0f, 1.0f, 1.0f),
		"Sphere"
	},
	{ // SPRING_BASE
		1.0f,
		0.0f,
		Vector3(0.0f, 0.0f, 0.0f),
		0.99f,
		Vector4(1.0f, 0.0f, 0.8f, 1.0f),
		0.0f,
		0,
		0,
		Vector3(3.0f, 0.5f, 3.0f),
		"Cube"
	}
};

Particle::Particle(ParticleType Type, PxTransform Transform, Vector3 Dir, bool Visible, bool Active) : particleType(Type), 
transform(Transform), dir(Dir), velc(ParticlesInfo[particleType].velc), 
force(Vector3(0.0f, 0.0f, 0.0f)), initialForce(force),
visible(Visible), active(Active), toDelete(false), refCount(1) {

	setMass(ParticlesInfo[particleType].mass);
	setVel(dir * velc);
	setAcc(ParticlesInfo[particleType].acc);
	setDamping(ParticlesInfo[particleType].damp);
	setSize(ParticlesInfo[particleType].size);
	setTime(ParticlesInfo[particleType].time);
	setColor(ParticlesInfo[particleType].col);
	setNumDivisions(ParticlesInfo[particleType].numDiv);
	setNumExplodes(ParticlesInfo[particleType].numExp);
	setShapeName(ParticlesInfo[particleType].geometryType);
	setShape(getShape(ParticlesInfo[particleType].geometryType, ParticlesInfo[particleType].size));

	if (visible) renderItem = new RenderItem(shape, &transform, color);
}

Particle::Particle(PxTransform Transform, Vector3 Dir, float Mass, float Velc, Vector3 Acc, float Damping, Vector3 Size, 
	float Time, Vector4 Color, int NumDivisions, int NumExplodes, bool Visible, bool Active) :
	particleType(NONE), transform(Transform), dir(Dir), mass(Mass), velc(Velc), acc(Acc), damping(Damping), size(Size), time(Time), color(Color), 
	numDivisions(NumDivisions), numExplodes(NumExplodes), visible(Visible), active(Active),
	toDelete(false), refCount(1) {

	setVel(dir * velc);
	shape = CreateShape(PxSphereGeometry(size.x));

	if (visible) renderItem = new RenderItem(shape, &transform, color);
}

Particle::~Particle() {
	DeregisterRenderItem(renderItem);
	renderItem = nullptr;
}

bool Particle::integrate(double t) {

	if (getInverseMass() <= 0.0f) return false;

	// Actualizamos la aceleracion en funcion de la fuerza
	Vector3 totalAcc = acc + force * getInverseMass();

	// Actualizamos la velocidad de la particula
	vel += totalAcc * t;

	// Aplicamos el damping
	vel *= powf(damping, t);

	// Actualizamos la posicion de la particula
	transform.p += vel * t;

	// Le vamos restando tiempo de vida
	time -= t;

	clearForce();

	changeColorWithTime();

	if (active) return true;
	else return time > 0.0f;
}

Particle* Particle::clone() const {
	switch (particleType) {
	case NONE: return new Particle(transform, dir, mass, velc, acc, damping, size, time, color, numDivisions, numExplodes); break;
	default: return new Particle(particleType, transform, dir); break;
	}
}

void Particle::addForce(Vector3 f) {
	force += f;
}

void Particle::clearForce() {
	force *= 0.0f;
}

void Particle::changeColorWithTime() {
	switch (particleType) {
	case EXPLOSION:
		if (time >= (ParticlesInfo[particleType].time / 2)) {
			setColor2(palettes.explosionPalette[0]);
		}
		else if (time >= (ParticlesInfo[particleType].time / 3)) {
			setColor2(palettes.explosionPalette[1]);
		}
		else if (time >= (ParticlesInfo[particleType].time / 4)) {
			setColor2(palettes.explosionPalette[2]);
		}
		break;
	}
}