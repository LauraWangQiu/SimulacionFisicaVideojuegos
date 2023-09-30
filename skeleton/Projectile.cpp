#include "Projectile.h"

Projectile::Projectile(currentShotType Type, PxTransform Transform, Vector3 Dir, float Time, PxReal Size, Vector4 Color) : Particle(Type, Transform, Dir, Time, Size, Color) {

	switch (shotType) {
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

Projectile::~Projectile() {}