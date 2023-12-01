#pragma once
#include <vector>
#include <string>
#include "RenderUtils.hpp"
#include "Constants.h"
using namespace std;
using namespace physx;

enum ParticleType {
	BASIC, FIREWORK, FIREWORK2, FIREWORK3, FIRE, WATER, STEAM,
	CANNON_BALL, TANK_BALL, GUN_BULLET, LASER,
	WIND, EXPLOSION, SPRING_STATIC, SPRING_BASE, SPRING_DYNAMIC, SLINKY, IMMERSE, WATER_PLANE,
	NONE
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
		Vector4(1.0f, 1.0f, 0.0f, 1.0f),	// Amarillo neon
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
};

class Particle {
public:
	Particle(ParticleType Type, PxTransform Transform, Vector3 Dir = Vector3(0.0f, 1.0f, 0.0f), bool Visible = true, bool Active = false);
	Particle(PxTransform Transform, Vector3 Dir = Vector3(0.0f, 1.0f, 0.0f), float Mass = 1.0f, float Velc = 10.0f, 
		Vector3 Acc = Vector3(0.0f, 0.0f, 0.0f), float Damping = 0.99f, Vector3 Size = Vector3(1.0f, 1.0f, 1.0f),
		float Time = 1.0f, Vector4 Color = Vector4(1.0f, 1.0f, 1.0f, 1.0f), int NumDivisions = 0, int NumExplodes = 0, bool Visible = true, bool Active = false);

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
	PxShape* shape;
	RenderItem* renderItem;
	int numDivisions, numExplodes;
	bool visible, active;
	Vector3 initialForce, force;
	bool toDelete; int refCount;

	void changeColorWithTime();

public:
	// Getters
	inline ParticleType getParticleType() const { return particleType; }
	inline PxTransform getTransform() const { return transform; }
	inline Vector3 getPos() const { return transform.p; }
	inline float getPosX() const { return getPos().x; }
	inline float getPosY() const { return getPos().y; }
	inline float getPosZ() const { return getPos().z; }
	inline float getMass() const { return mass; }
	inline float getInverseMass() const { return 1 / mass; }
	inline float getVelc() const { return velc; }
	inline Vector3 getDir() const { return dir; }
	inline Vector3 getVel() const { return vel; }
	inline Vector3 getAcc() const { return acc; }
	inline float getDamping() const { return damping; }
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
		else return CreateShape(PxSphereGeometry(size.x));
	}
	inline bool getDelete() const { return toDelete; }
	inline int getRefCount() const { return refCount; }

	// Setters
	inline void setParticleType(ParticleType Type) { particleType = Type; }
	inline void setTransform(PxTransform Transform) { transform = Transform; }
	inline void setPos(Vector3 Pos) { transform.p = Pos; }
	inline void setPos(float X, float Y, float Z) { transform.p = Vector3(X, Y, Z); }
	inline void setPosX(float X) { transform.p.x = X; }
	inline void setPosY(float Y) { transform.p.y = Y; }
	inline void setPosZ(float Z) { transform.p.z = Z; }
	inline void setMass(float Mass) { mass = Mass; }
	inline void setVelc(float VelC) { velc = VelC; }
	inline void setDir(Vector3 Dir) { dir = Dir; }
	inline void setDir(float X, float Y, float Z) { dir = Vector3(X, Y, Z); }
	inline void setVel(Vector3 Vel) { vel = Vel; }
	inline void setVel(float X, float Y, float Z) { vel = Vector3(X, Y, Z); }
	inline void setAcc(Vector3 Acc) { acc = Acc; }
	inline void setAcc(float X, float Y, float Z) { acc = Vector3(X, Y, Z); }
	inline void setDamping(float Damp) { damping = Damp; }
	inline void setSize(Vector3 Size) { size = Size; }
	inline void setSize(float X, float Y, float Z) { size = Vector3(X, Y, Z); }
	inline void setColor(Vector4 Color) { color = Color; }
	inline void setColor(float R, float G, float B, float A = 1.0f) { color = Vector4(R, G, B, A); }
	inline void setColor2(Vector4 Color) { renderItem->color = Color; }
	inline void setColor2(float R, float G, float B, float A = 1.0f) { renderItem->color = Vector4(R, G, B, A); }
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
	inline void setShape(PxShape* g) { shape = g; }
	inline void setDelete(bool toDelete) { this->toDelete = toDelete; }
	inline void release() { 
		if (--refCount == 0) {
			delete this;
		}
	}
	inline void increaseRefCount() { ++refCount; }
	inline void decreaseRefCount() { --refCount; }
};