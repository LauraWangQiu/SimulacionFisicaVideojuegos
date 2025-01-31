#include "Particle.h"

vector<particleInfo> ParticlesInfo = {
	{ // BASIC
		1.0f,
		10.0f,
		Vector3(0.0f, 0.0f, 0.0f),
		0.99f,
		Vector4(1.0f, 0.0f, 0.0f, 1.0f),
		1.0f,
		0,
		0,
		Vector3(0.5f, 0.5f, 0.5f),
		"Sphere",
		0.7f,
		Vector3(0.0f, 1.0f, 0.0f)
	},
	{ // FIREWORK
		0.0f,
		100.0f,
		Vector3(0.0f, -50.0f, 0.0f),
		0.99f,
		Vector4(1.0f, 0.0f, 0.0f, 1.0f),
		3.0f,
		20,
		1,
		Vector3(1.0f, 1.0f, 1.0f),
		"Sphere",
		1.0f,
		Vector3(0.0f, 1.0f, 0.0f)
	},
	{ // FIREWORK2
		0.0f,
		150.0f,
		Vector3(0.0f, -50.0f, 0.0f),
		0.99f,
		Vector4(1.0f, 0.0f, 0.0f, 1.0f),
		3.0f,
		10,
		1,
		Vector3(1.0f, 1.0f, 1.0f),
		"Sphere",
		1.0f,
		Vector3(0.0f, 1.0f, 0.0f)
	},
	{ // RANDOM
		1.0f,
		100.0f,
		Vector3(0.0f, 0.0f, 0.0f),
		0.99f,
		Vector4(0.0f, 0.0f, 1.0f, 1.0f),
		10.0f,
		0,
		0,
		Vector3(1.0f, 1.0f, 1.0f),
		"Cube",
		1.0f,
		Vector3(0.0f, 1.0f, 0.0f)
	},
	{ // TRASH
		1.0f,
		100.0f,
		Vector3(0.0f, 0.0f, 0.0f),
		0.99f,
		Vector4(0.0f, 0.0f, 1.0f, 1.0f),
		10.0f,
		0,
		0,
		Vector3(1.0f, 1.0f, 1.0f),
		"Cube",
		10000.0f,
		Vector3(0.0f, 1.0f, 0.0f)
	},
	{ // SPACECRAFT
		10000.0f,
		0.0f,
		Vector3(0.0f, 0.0f, 0.0f),
		0.99f,
		Vector4(1.0f, 0.0f, 0.0f, 1.0f),
		0.0f,
		0,
		0,
		Vector3(3.2f, 7.0f, 3.2f),
		"Cube",
		10000.0f,
		Vector3(0.0f, 1.0f, 0.0f)
	},
	{ // PROPELLANT
		100.0f,
		0.0f,
		Vector3(0.0f, 0.0f, 0.0f),
		0.99f,
		Vector4(0.0f, 0.0f, 1.0f, 1.0f),
		0.0f,
		0,
		0,
		Vector3(1.0f, 2.0f, 2.0f),
		"Cube",
		1.0f,
		Vector3(0.0f, -1.0f, 0.0f)
	},
	{ // WINDOW
		10.0f,
		0.0f,
		Vector3(0.0f, 0.0f, 0.0f),
		0.99f,
		Vector4(0.0f, 0.8f, 0.8f, 1.0f),
		0.0f,
		0,
		0,
		Vector3(2.0f, 2.0f, 2.0f),
		"Sphere",
		1.0f,
		Vector3(0.0f, 0.0f, 1.0f)
	},
	{ // WATER_PLANE
		100000.0f,
		0.0f,
		Vector3(0.0f, 0.0f, 0.0f),
		0.99f,
		Vector4(1.0f, 1.0f, 1.0f, 0.4f),
		0.0f,
		0,
		0,
		Vector3(100.0f, 10.0f, 100.0f),
		"Cube",
		1000000.0f,
		Vector3(0.0f, 1.0f, 0.0f)
	}
};

Particle::Particle(ParticleType Type, PxTransform Transform, Vector3 Dir, 
	bool Visible, bool Active) : 
	particleType(Type),
	transform(Transform), dir(Dir), velc(ParticlesInfo[particleType].velc), 
	force(Vector3(0.0f, 0.0f, 0.0f)), initialForce(force),
	density(1000.0f), massInertiaTensor(Vector3(0.0f, 1.0f, 0.0f)),
	visible(Visible), active(Active),
	toDelete(false), refCount(1), toExplode(false) {
	
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
	setDensity(ParticlesInfo[particleType].density);
	setMassInertiaTensor(ParticlesInfo[particleType].massInertiaTensor);

	if (visible) renderItem = new RenderItem(shape, &transform, color);
}

Particle::Particle(PxTransform Transform, Vector3 Dir, 
	float Mass, float Velc, Vector3 Acc, float Damping, Vector3 Size,
	float Time, Vector4 Color, string ShapeName, 
	int NumDivisions, int NumExplodes, 
	float Density, Vector3 MassInertiaTensor, 
	bool Visible, bool Active) :

	particleType(NONE), transform(Transform), dir(Dir), 
	mass(Mass), velc(Velc), acc(Acc), damping(Damping), size(Size), 
	time(Time), color(Color), shapeName(ShapeName),
	numDivisions(NumDivisions), numExplodes(NumExplodes), 
	density(Density), massInertiaTensor(MassInertiaTensor),
	visible(Visible), active(Active),
	toDelete(false), refCount(1), toExplode(false)
{

	if (gPhysics == nullptr || gScene == nullptr) {
		setVel(dir * velc);
		shape = getShape(shapeName, size);

		if (visible) renderItem = new RenderItem(shape, &transform, color);
	}
}

Particle::Particle(PxPhysics* GPhysics, PxScene* GScene, 
	ParticleType Type, PxTransform Transform, Vector3 Dir, 
	bool Visible, bool Active) : 
	particleType(Type), transform(Transform), dir(Dir), velc(ParticlesInfo[particleType].velc),
	force(Vector3(0.0f, 0.0f, 0.0f)), initialForce(force),
	density(1000.0f), massInertiaTensor(Vector3(0.0f, 1.0f, 0.0f)),
	visible(Visible), active(Active), 
	toDelete(false), refCount(1), toExplode(false),
	gPhysics(GPhysics), gScene(GScene)
{

	if (gPhysics != nullptr & gScene != nullptr) {

		rigid = gPhysics->createRigidDynamic(transform);
		rigid->setName(getName(particleType));
		rigid->userData = this;
		rigid->setLinearVelocity(dir * velc);
		rigid->setAngularVelocity(Vector3(0.0f, 0.0f, 0.0f));
		rigid->setMassSpaceInertiaTensor(massInertiaTensor);
		rigid->setLinearDamping(ParticlesInfo[particleType].damp);
		PxShape* shape = getShape(ParticlesInfo[particleType].geometryType, ParticlesInfo[particleType].size);
		rigid->attachShape(*shape);
		PxRigidBodyExt::updateMassAndInertia(*rigid, ParticlesInfo[particleType].density);
		//rigid->setMass(ParticlesInfo[particleType].mass);
		gScene->addActor(*rigid);

		setMass(rigid->getMass());
		setVel(rigid->getLinearVelocity());
		setDamping(rigid->getLinearDamping());
		setSize(ParticlesInfo[particleType].size);
		setTime(ParticlesInfo[particleType].time);
		setColor(ParticlesInfo[particleType].col);
		setNumDivisions(ParticlesInfo[particleType].numDiv);
		setNumExplodes(ParticlesInfo[particleType].numExp);
		setShapeName(ParticlesInfo[particleType].geometryType);
		setShape(shape);
		setDensity(ParticlesInfo[particleType].density);
		setMassInertiaTensor(rigid->getMassSpaceInertiaTensor());

		if (visible) renderItem = new RenderItem(shape, rigid, ParticlesInfo[particleType].col);
	}
}

Particle::Particle(PxPhysics* GPhysics, PxScene* GScene, 
	PxTransform Transform, Vector3 Dir, 
	float Mass, float Velc, Vector3 Acc, 
	float Damping, Vector3 Size,
	float Time, Vector4 Color, string ShapeName, 
	int NumDivisions, int NumExplodes, 
	float Density, Vector3 MassInertiaTensor, 
	bool Visible, bool Active) :

	particleType(NONE), transform(Transform), dir(Dir), 
	mass(Mass), velc(Velc), acc(Acc), 
	damping(Damping), size(Size), 
	time(Time), color(Color), shapeName(ShapeName),
	numDivisions(NumDivisions), numExplodes(NumExplodes), 
	density(Density), massInertiaTensor(MassInertiaTensor),
	visible(Visible), active(Active),
	toDelete(false), refCount(1), toExplode(false),
	gPhysics(GPhysics), gScene(GScene)
{

	if (gPhysics != nullptr && gScene != nullptr) {

		rigid = gPhysics->createRigidDynamic(transform);
		rigid->setName(getName(particleType));
		rigid->userData = this;
		rigid->setLinearVelocity(dir * velc);
		rigid->setAngularVelocity(Vector3(0.0f, 0.0f, 0.0f));
		rigid->setMassSpaceInertiaTensor(MassInertiaTensor);
		rigid->setLinearDamping(damping);
		PxShape* shape = getShape(shapeName, size);
		rigid->attachShape(*shape);
		PxRigidBodyExt::updateMassAndInertia(*rigid, density);
		//rigid->setMass(mass);
		gScene->addActor(*rigid);

		setMass(rigid->getMass());
		setVel(rigid->getLinearVelocity());
		setDamping(rigid->getLinearDamping());
		setShape(shape);

		if (visible) renderItem = new RenderItem(shape, rigid, color);
	}
}

Particle::~Particle() {
	if (gPhysics != nullptr && gScene != nullptr) {
		gScene->removeActor(*rigid);
		rigid->release();
	}

	if (renderItem != nullptr) {
		DeregisterRenderItem(renderItem);
		renderItem = nullptr;
	}
}

bool Particle::integrate(double t) {

	if (gPhysics == nullptr || gScene == nullptr) {
		if (getInverseMass() <= 0.0f) return false;

		// Actualizamos la aceleracion en funcion de la fuerza
		Vector3 totalAcc = acc + force * getInverseMass();

#pragma region EULER
		//// Actualizamos la posicion de la particula
		//transform.p += vel * t;

		//// Actualizamos la posicion de la particula
		//vel += totalAcc * t;
#pragma endregion

#pragma region EULER-SEMIIMPLICITO
		// Actualizamos la velocidad de la particula
		vel += totalAcc * t;

		// Aplicamos el damping
		vel *= powf(damping, t);

		// Actualizamos la posicion de la particula
		transform.p += vel * t;
#pragma endregion
	}
	else {
		rigid->addForce(force);
		//rigid->addTorque(Vector3(0, 100, 0));
	}

	clearForce();

	// Le vamos restando tiempo de vida
	time -= t;

	changeColorWithTime();

	if (active) return true;
	else return time > 0.0f;
}

Particle* Particle::clone() const {
	if (gPhysics == nullptr || gScene == nullptr) {
		switch (particleType) {
		case NONE: return new Particle(transform, dir, mass, velc, acc, damping, size, time, color, shapeName, numDivisions, numExplodes, density); break;
		default: return new Particle(particleType, transform, dir); break;
		}
	}
	else {
		switch (particleType) {
		case RANDOM: return new Particle(gPhysics, gScene, transform, dir, mass, velc, acc, damping, size, time, color, shapeName, numDivisions, numExplodes, density); break;
		default: return new Particle(gPhysics, gScene, particleType, transform, dir); break;
		}
	}
}

void Particle::addForce(Vector3 f) {
	force += f;
}

void Particle::clearForce() {
	force *= 0.0f;
}

void Particle::release() {
	if (--refCount == 0) {
		delete this;
	}
}

void Particle::changeColorWithTime() {
	/*switch (particleType) {
	case :
		break;
	}*/
}