#pragma once
#include "RenderUtils.hpp"
using namespace physx;
using namespace std;

enum currentShotType { NONE, CANNON_BALL, TANK_BALL, GUN_BULLET, LASER };

class Particle {
public:
	// Diferentes constructoras
	Particle(Vector3 Pos, float Mass, Vector3 Vel, Vector3 Acc, float Time, PxReal Size = 1.0f, Vector4 Color = Vector4(255.0, 255.0, 255.0, 255.0), float Damp = 0.998);
	Particle(currentShotType Type, Vector3 Pos, float Time = 5.0f, PxReal Size = 1.0f, Vector4 Color = Vector4(255.0, 255.0, 255.0, 255.0));
	~Particle();

	bool integrate(double t);

private: 
	currentShotType shotType;
	physx::PxTransform transform;
	Vector3 vel, acc;
	PxReal size;
	Vector4 color;
	float mass, damping, time;
	PxShape* shape;
	RenderItem* renderItem;

	// Getters
	inline currentShotType getShotType() const { return shotType; }
	inline PxTransform getTransform() const { return transform; }
	inline Vector3 getPos() const { return transform.p; }
	inline float getPosX() const { return getPos().x; }
	inline float getPosY() const { return getPos().y; }
	inline float getPosZ() const { return getPos().z; }
	inline float getMass() const { return mass; }
	inline float getInverseMass() const { return 1 / mass; }
	inline Vector3 getVel() const { return vel; }
	inline Vector3 getAcc() const { return acc; }
	inline float getDamping() const { return damping; }
	inline Vector4 getColor() const { return color; }

	// Setters
	inline void setPos(Vector3 Pos) { transform.p = Pos; }
	inline void setPos(float X, float Y, float Z) { transform.p = Vector3(X, Y, Z); }
	inline void setPosX(float X) { transform.p.x = X; }
	inline void setPosY(float Y) { transform.p.y = Y; }
	inline void setPosZ(float Z) { transform.p.z = Z; }
	inline void setMass(float Mass) { mass = Mass; }
	inline void setVel(Vector3 Vel) { vel = Vel; }
	inline void setVel(float X, float Y, float Z) { vel = Vector3(X, Y, Z); }
	inline void setAcc(Vector3 Acc) { acc = Acc; }
	inline void setAcc(float X, float Y, float Z) { acc = Vector3(X, Y, Z); }
	inline void setDamping(float Damp) { damping = Damp; }
	inline void setColor(Vector4 Color) { color = Color; }
	inline void setColor(float r, float g, float b, float a = 255.0f) { color = Vector4(r, g, b, a); }
};
