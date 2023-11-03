#include "ParticleForceRegistry.h"

void ParticleForceRegistry::updateForces() {
	for (auto it = begin(); it != end(); ++it) {
		it->first->updateForce(it->second);
	}
}

void ParticleForceRegistry::addRegistry(ForceGenerator* fg, Particle* p) {
	for (auto it = begin(); it != end(); ++it) {
		if (fg != nullptr && p != nullptr) {
			insert(pair<ForceGenerator*, Particle*>(fg, p));
		}
	}
}

void ParticleForceRegistry::deleteParticleRegistry(Particle* p) {
	
}