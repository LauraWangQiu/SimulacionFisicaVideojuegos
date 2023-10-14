#pragma once
#include "RenderUtils.hpp"
using namespace physx;

enum ParticleType { BASIC, FIRE, FIREWORK, WATER,
	CANNON_BALL, TANK_BALL, GUN_BULLET, LASER
};

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

	Vector4 waterPalette[3] = {
		Vector4(0.1f, 0.3f, 0.8f, 1.0f),
		Vector4(0.1f, 0.3f, 0.9f, 1.0f),
		Vector4(0.1f, 0.3f, 0.5f, 1.0f),
	};
	int waterPaletteSize = sizeof(waterPalette) / sizeof(waterPalette[0]);
};

class Particle {
public:
	Particle(ParticleType Type, PxTransform Transform, Vector3 Dir = Vector3(0.0f, 1.0f, 0.0f), float Time = 1.0f, PxReal Size = 1.0f, Vector4 Color = Vector4(255.0, 255.0, 255.0, 255.0));
	virtual ~Particle();

	virtual bool integrate(double t);
	virtual Particle* clone() const;

protected: 
	ParticleType particleType;
	PxTransform transform;
	Vector3 dir, vel, acc;
	PxReal size;
	Vector4 color;
	float mass, damping, time;
	PxShape* shape;
	RenderItem* renderItem;

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
	inline Vector3 getDir() const { return dir; }
	inline Vector3 getVel() const { return vel; }
	inline Vector3 getAcc() const { return acc; }
	inline float getDamping() const { return damping; }
	inline PxReal getSize() const { return size; }
	inline Vector4 getColor() const { return color; }
	inline float getTime() const { return time; }

	// Setters
	inline void setPos(Vector3 Pos) { transform.p = Pos; }
	inline void setPos(float X, float Y, float Z) { transform.p = Vector3(X, Y, Z); }
	inline void setPosX(float X) { transform.p.x = X; }
	inline void setPosY(float Y) { transform.p.y = Y; }
	inline void setPosZ(float Z) { transform.p.z = Z; }
	inline void setMass(float Mass) { mass = Mass; }
	inline void setDir(Vector3 Dir) { dir = Dir; }
	inline void setDir(float X, float Y, float Z) { dir = Vector3(X, Y, Z); }
	inline void setVel(Vector3 Vel) { vel = Vel; }
	inline void setVel(float X, float Y, float Z) { vel = Vector3(X, Y, Z); }
	inline void setAcc(Vector3 Acc) { acc = Acc; }
	inline void setAcc(float X, float Y, float Z) { acc = Vector3(X, Y, Z); }
	inline void setDamping(float Damp) { damping = Damp; }
	inline void setSize(PxReal Size) { size = Size; }
	inline void setColor(Vector4 Color) { color = Color; }
	inline void setColor(float R, float G, float B, float A = 1.0f) { color = Vector4(R, G, B, A); }
	inline void setColor2(Vector4 Color) { renderItem->color = Color; }
	inline void setColor2(float R, float G, float B, float A = 255.0f) { renderItem->color = Vector4(R, G, B, A); }
	inline void setTime(float Time) { time = Time; }
	inline void increaseTime(float Time) { time += Time; }
};
