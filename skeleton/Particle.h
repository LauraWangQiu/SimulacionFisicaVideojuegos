#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "RenderUtils.hpp"
#include "Constants.h"
using namespace std;
using namespace physx;

enum ParticleType {
	BASIC, FIREWORK, FIREWORK2, RANDOM, TRASH, SPACECRAFT, PROPELLANT, WINDOW, WATER_PLANE, NONE
};

struct particleInfo {
	float mass;
	float velc;
	Vector3 acc;
	float damp;
	Vector4 col;
	float time;
	int numDiv;
	int numExp;
	Vector3 size;
	string geometryType;
	float density;
	Vector3 massInertiaTensor;
};

// Declaración del vector como extern en el archivo de encabezado
extern std::vector<particleInfo> ParticlesInfo;

struct particlePalettes {
	Vector4 firePalette[3] = {
		Vector4(1.0f, 0.0f, 0.0f, 1.0f),	// Rojo
		Vector4(1.0f, 0.65f, 0.0f, 1.0f),	// Naranja
		Vector4(1.0f, 1.0f, 0.0f, 1.0f)		// Amarillo
	};
	int firePaletteSize = sizeof(firePalette) / sizeof(firePalette[0]);

	Vector4 fireWorkPalette[5] = {
		Vector4(1.0f, 0.078f, 0.576f, 1.0f),// Rosa neon
		Vector4(0.22f, 1.0f, 0.08f, 1.0f),	// Verde neon
		Vector4(0.0f, 0.8f, 1.0f, 1.0f),	// Azul neon
		Vector4(1.0f, 0.5f, 0.0f, 1.0f),	// Naranja neon
		Vector4(1.0f, 1.0f, 0.0f, 1.0f)		// Amarillo neon
	};
	int fireWorkPaletteSize = sizeof(fireWorkPalette) / sizeof(fireWorkPalette[0]);

	Vector4 fireWorkPalette2[5] = {
		Vector4(1.0f, 0.0f, 0.0f, 1.0f),
		Vector4(1.0f, 0.65f, 0.0f, 1.0f),
		Vector4(1.0f, 1.0f, 0.0f, 1.0f),
		Vector4(0.9f, 0.4f, 0.0f, 1.0f),
		Vector4(0.9f, 0.8f, 0.0f, 1.0f),
	};
	int fireWorkPaletteSize2 = sizeof(fireWorkPalette2) / sizeof(fireWorkPalette2[0]);

	Vector4 fireWorkPalette3[5] = {
		Vector4(0.9f, 0.0f, 0.4f, 1.0f),
		Vector4(0.9f, 0.0f, 0.5f, 1.0f),
		Vector4(0.9f, 0.0f, 0.6f, 1.0f),
		Vector4(0.9f, 0.0f, 0.7f, 1.0f),
		Vector4(0.9f, 0.0f, 0.8f, 1.0f),
	};
	int fireWorkPaletteSize3 = sizeof(fireWorkPalette3) / sizeof(fireWorkPalette3[0]);

	Vector4 waterPalette[3] = {
		Vector4(0.1f, 0.3f, 0.8f, 1.0f),
		Vector4(0.1f, 0.3f, 0.9f, 1.0f),
		Vector4(0.1f, 0.3f, 0.5f, 1.0f),
	};
	int waterPaletteSize = sizeof(waterPalette) / sizeof(waterPalette[0]);

	Vector4 explosionPalette[4] = {
		Vector4(1.0f, 1.0f, 0.0f, 1.0f),	// Amarillo
		Vector4(1.0f, 0.65f, 0.0f, 1.0f),	// Naranja
		Vector4(0.4f, 0.2f, 0.1f, 1.0f)		// Marron
	};
	int explosionPaletteSize = sizeof(explosionPalette) / sizeof(explosionPalette[0]);

	Vector4 spacecraftPalette[6] = {
		Vector4(1.0f, 0.0f, 0.0f, 1.0f),	// Rojo
		Vector4(1.0f, 0.078f, 0.576f, 1.0f),// Rosa neon
		Vector4(0.22f, 1.0f, 0.08f, 1.0f),	// Verde neon
		Vector4(0.0f, 0.8f, 1.0f, 1.0f),	// Azul neon
		Vector4(1.0f, 0.5f, 0.0f, 1.0f),	// Naranja neon
		Vector4(1.0f, 1.0f, 0.0f, 1.0f)		// Amarillo neon
	};
	int spacecraftPaletteSize = sizeof(spacecraftPalette) / sizeof(spacecraftPalette[0]);
};

class Particle {
public:
	Particle(ParticleType Type, PxTransform Transform, Vector3 Dir = Vector3(0.0f, 1.0f, 0.0f), 
		bool Visible = true, bool Active = false);
	Particle(PxTransform Transform, Vector3 Dir = Vector3(0.0f, 1.0f, 0.0f), 
		float Mass = 1.0f, float Velc = 10.0f, Vector3 Acc = Vector3(0.0f, 0.0f, 0.0f), 
		float Damping = 0.99f, Vector3 Size = Vector3(1.0f, 1.0f, 1.0f),
		float Time = 1.0f, Vector4 Color = Vector4(1.0f, 1.0f, 1.0f, 1.0f), string ShapeName = "Sphere",
		int NumDivisions = 0, int NumExplodes = 0, 
		float Density = 1000.0f, Vector3 MassInertiaTensor = Vector3(0.0f, 1.0f, 0.0f), 
		bool Visible = true, bool Active = false);
	Particle(PxPhysics* GPhysics, PxScene* GScene, 
		ParticleType Type, PxTransform Transform, Vector3 Dir = Vector3(0.0f, 1.0f, 0.0f), 
		bool Visible = true, bool Active = false);
	Particle(PxPhysics* GPhysics, PxScene* GScene, 
		PxTransform Transform, Vector3 Dir = Vector3(0.0f, 1.0f, 0.0f), 
		float Mass = 1.0f, float Velc = 10.0f, Vector3 Acc = Vector3(0.0f, 0.0f, 0.0f), 
		float Damping = 0.99f, Vector3 Size = Vector3(1.0f, 1.0f, 1.0f),
		float Time = 1.0f, Vector4 Color = Vector4(1.0f, 1.0f, 1.0f, 1.0f), string ShapeName = "Sphere",
		int NumDivisions = 0, int NumExplodes = 0, 
		float Density = 1000.0f, Vector3 MassInertiaTensor = Vector3(0.0f, 1.0f, 0.0f), 
		bool Visible = true, bool Active = false);

	virtual ~Particle();

	virtual bool integrate(double t);
	virtual Particle* clone() const;

	void addForce(Vector3 f);
	void clearForce();

protected: 
	ParticleType particleType;
	PxTransform transform;
	Vector3 dir, vel, acc, size;
	Vector4 color;
	particlePalettes palettes;
	float mass, velc, damping, time;
	string shapeName;
	PxShape* shape			= nullptr;
	RenderItem* renderItem	= nullptr;
	int numDivisions, numExplodes;
	bool visible, active;
	Vector3 initialForce, force;
	bool toDelete; int refCount;
	bool toExplode;

	float density;
	Vector3 massInertiaTensor;
	PxPhysics*	gPhysics	= nullptr;
	PxScene*	gScene		= nullptr;
	PxRigidDynamic* rigid	= nullptr;

	void changeColorWithTime();

public:
	// Getters
	inline ParticleType getParticleType() const { return particleType; }
	inline PxTransform getTransform() const {
		if (!isRigid()) return transform;
		else return rigid->getGlobalPose();
	}
	inline Vector3 getPos() const {
		if (!isRigid()) return transform.p;
		else return rigid->getGlobalPose().p;
	}
	inline float getPosX() const { return getPos().x; }
	inline float getPosY() const { return getPos().y; }
	inline float getPosZ() const { return getPos().z; }
	inline float getMass() const { return mass; }
	inline float getInverseMass() const { return 1 / mass; }
	inline float getVelc() const { return velc; }
	inline Vector3 getDir() const { return dir; }
	inline Vector3 getVel() const {
		if (!isRigid()) return vel;
		else return rigid->getLinearVelocity();
	}
	inline Vector3 getAcc() const { return acc; }
	inline float getDamping() const {
		if (!isRigid()) return damping;
		else return rigid->getLinearDamping();
	}
	inline Vector3 getSize() const { return size; }
	inline Vector4 getColor() const { return color; }
	inline float getTime() const { return time; }
	inline int getNumDivisions() const { return numDivisions; }
	inline int getNumExplodes() const { return numExplodes;  }
	inline bool getVisible() const { return visible; }
	inline bool getActive() const { return active; }
	inline Vector3 getInitialForce() const { return initialForce; }
	inline Vector3 getForce() const { return force; }
	inline string getShapeName() const { return shapeName; }
	inline PxShape* getShape(string name, Vector3 size) const {
		if (name == "Sphere") return CreateShape(PxSphereGeometry(size.x));
		else if (name == "Cube") return CreateShape(PxBoxGeometry(size));
		else if (name == "Capsule") return CreateShape(PxCapsuleGeometry(size.x, size.y));
		else if (name == "Plane") return CreateShape(PxPlaneGeometry());
		else return CreateShape(PxSphereGeometry(size.x));
	}
	inline bool getDelete() const { return toDelete; }
	inline bool getToExplode() const { return toExplode; }
	inline int getRefCount() const { return refCount; }
	inline float getDensity() const { return density; }
	inline Vector3 getMassInertiaTensor() const { return massInertiaTensor; }
	inline bool isRigid() const { return rigid != nullptr; }
	inline PxRigidDynamic* getRigid() const { return rigid; }

	// Setters
	inline void setParticleType(ParticleType Type) { particleType = Type; }
	inline void setTransform(PxTransform Transform) { transform = Transform; }
	inline void setPos(Vector3 Pos) {
		if (!isRigid()) transform.p = Pos;
		else rigid->setGlobalPose(PxTransform(Pos));
	}
	inline void setPos(float X, float Y, float Z) {
		if (!isRigid()) transform.p = Vector3(X, Y, Z);
		else rigid->setGlobalPose(PxTransform(Vector3(X, Y, Z)));
	}
	inline void setPosX(float X) { setPos(X, getPosY(), getPosZ()); }
	inline void setPosY(float Y) { setPos(getPosX(), Y, getPosZ()); }
	inline void setPosZ(float Z) { setPos(getPosX(), getPosY(), Z); }
	inline void setMass(float Mass) { mass = Mass; }
	inline void setVelc(float VelC) { velc = VelC; }
	inline void setDir(Vector3 Dir) { dir = Dir; }
	inline void setDir(float X, float Y, float Z) { setDir(Vector3(X, Y, Z)); }
	inline void setVel(Vector3 Vel) {
		if (!isRigid()) vel = Vel;
		else rigid->setLinearVelocity(Vel);
	}
	inline void setVel(float X, float Y, float Z) { setVel(Vector3(X, Y, Z)); }
	inline void setAcc(Vector3 Acc) { acc = Acc; }
	inline void setAcc(float X, float Y, float Z) { setAcc(Vector3(X, Y, Z)); }
	inline void setDamping(float Damp) { damping = Damp; }
	inline void setSize(Vector3 Size) { size = Size; }
	inline void setSize(float X, float Y, float Z) { setSize(Vector3(X, Y, Z)); }
	inline void setRandomSize() {
		setSize(generateRandomValue(1, MAX_SIZE), generateRandomValue(1, MAX_SIZE), generateRandomValue(1, MAX_SIZE));
	}
	inline void setColor(Vector4 Color) { color = Color; }
	inline void setColor(float R, float G, float B, float A = 1.0f) { setColor(Vector4(R, G, B, A)); }
	inline void setColor2(Vector4 Color) { if (renderItem != nullptr) renderItem->color = Color; setColor(Color); }
	inline void setColor2(float R, float G, float B, float A = 1.0f) { setColor2(Vector4(R, G, B, A)); }
	inline void setRandomColor() {
		float R = static_cast<float>(rand()) / RAND_MAX; // Componente Rojo
		float G = static_cast<float>(rand()) / RAND_MAX; // Componente Verde
		float B = static_cast<float>(rand()) / RAND_MAX; // Componente Azul
		setColor(R, G, B);
		setColor2(R, G, B);
	}
	inline void setRandomColorAmong() {
		int r = rand() % palettes.spacecraftPaletteSize;
		setColor(palettes.spacecraftPalette[r]);
		setColor2(palettes.spacecraftPalette[r]);
	}
	inline void setTime(float Time) { time = Time; }
	inline void increaseTime(float Time) { time += Time; }
	inline void setNumDivisions(int Num) { numDivisions = Num; }
	inline void decreaseNumDivisions() { --numDivisions; }
	inline void increaseNumDivisions() { ++numDivisions; }
	inline void setNumExplodes(int Num) { numExplodes = Num; }
	inline void decreaseNumExplodes() { --numExplodes; }
	inline void increaseNumExplodes() { ++numExplodes; }
	inline void reTransform(Vector3 newPos, Vector3 newDir) {
		setPos(newPos);
		dir = newDir;
		setVel(dir * ParticlesInfo[particleType].velc);
	}
	inline void setVisible(bool Visible) { visible = Visible; }
	inline void setActive(bool Active) { active = Active; }
	inline void setInitialForce(Vector3 InitialForce) { initialForce = InitialForce; }
	inline void setInitialForce(float X, float Y, float Z) { initialForce = Vector3(X, Y, Z); }
	inline void setForce(Vector3 Force) { force = Force; }
	inline void setForce(float X, float Y, float Z) { force = Vector3(X, Y, Z); }
	inline void setShapeName(string name) { shapeName = name; }
	inline void setRandomShapeName() {
		int g = rand() % 3;
		switch (g) {
		case 0: setShapeName("Cube"); break;
		case 1: setShapeName("Sphere"); break;
		case 2: setShapeName("Capsule"); break;
		default: setShapeName("Cube"); break;
		}
	}
	inline void setShape(PxShape* g) { shape = g; }
	inline void setDelete(bool toDelete) { this->toDelete = toDelete; }
	virtual void release();
	inline void increaseRefCount() { ++refCount; }
	inline void decreaseRefCount() { --refCount; }
	inline void setToExplode(bool ToExplode) { toExplode = ToExplode; }
	inline void setDensity(float Density) { density = Density; }
	inline void setRandomDensity() { setDensity(generateRandomValue(1, 1000)); }
	inline void setMassInertiaTensor(Vector3 m) { massInertiaTensor = m; }
	inline void setRandomMassInertiaTensor() {
		int r = generateRandomValue(0, 2);

		switch (r) {
		case 0: setMassInertiaTensor(Vector3(1.0f, 0.0f, 0.0f)); break;
		case 1: setMassInertiaTensor(Vector3(0.0f, 1.0f, 0.0f)); break;
		case 2: setMassInertiaTensor(Vector3(0.0f, 0.0f, 1.0f)); break;
		default: break;
		}
	}

	inline int generateRandomValue(int min, int max) { return min + (rand() % (max - min + 1)); }
	inline double generateRandomValueWithPrecision(int precision = 2) { return (double)(rand() % (int)pow(10, precision)) / pow(10, precision); }

	inline void setRandom() {
		setRandomSize();
		setRandomDensity();
		setRandomShapeName();
		setRandomColorAmong();
		setRandomMassInertiaTensor();
	}

	inline const char* getName(ParticleType Type) {
		switch (Type) {
		case BASIC: return "BASIC";
		case FIREWORK: return "FIREWORK";
		case FIREWORK2: return "FIREWORK";
		case RANDOM: return "RANDOM";
		case SPACECRAFT: return "SPACECRAFT";
		case PROPELLANT: return "PROPELLANT";
		case WINDOW: return "WINDOW";
		case NONE: return "NONE";
		}
	}
};