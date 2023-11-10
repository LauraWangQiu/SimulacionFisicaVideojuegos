#include "ParticleForceRegistry.h"

void ParticleForceRegistry::updateForces() {
	for (auto it = begin(); it != end(); ++it) {
		it->first->updateForce(it->second);
	}
}

void ParticleForceRegistry::addRegistry(ForceGenerator* fg, Particle* p) {
	if (fg != nullptr && p != nullptr) {
		insert(pair<ForceGenerator*, Particle*>(fg, p));
	}
}

void ParticleForceRegistry::deleteParticleRegistry(Particle* p) {
	auto it = begin();
	while (it != end()) {
		if (it->second == p) {
			erase(it);
			break;
		}
		++it;
	}
}

void ParticleForceRegistry::removeForceGenerator(ForceGenerator* fg) {
	auto it = begin();
	while (it != end()) {
		if (it->first == fg) {
			it->first->clearForce(it->second);
			it = erase(it);
		}
		else ++it;
	}
}