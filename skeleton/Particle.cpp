// ACTIVIDAD 1A
#include "Particle.h"

Particle::Particle(Vector3 Pos, Vector3 Vel) : pos(Pos), vel(Vel) {
	// Le especificamos el aspecto de la particula. En este caso va a ser una esfera
	renderItem = new RenderItem(CreateShape(PxSphereGeometry(10.0f)), &pos, Vector4(255.0f, 255.0f, 255.0f, 255.0f));
}

Particle::~Particle() {
	renderItem->release();
}

void Particle::integrate(double t) {

	// Actualizamos la posicion de la particula
	pos.p += vel * t;
}
// ============