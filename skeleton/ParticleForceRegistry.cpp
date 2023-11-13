#include "ParticleForceRegistry.h"

void ParticleForceRegistry::updateForces() {
	for (auto it = begin(); it != end(); ++it) {
		it->first->updateForce(it->second);
	}
}

void ParticleForceRegistry::addRegistry(ForceGenerator* fg, Particle* p) {
	if (fg != nullptr && p != nullptr) {
		// Comprobar si es un par no existente en el registro,
		// si no existe, insertar e incrementar el numero de referencias
		auto range = equal_range(fg);

		bool exists = false;
		auto it = range.first;
		while (it != range.second) {
			if (it->second == p) {
				exists = true;
				break;
			} 
			++it;
		}

		// Si no existe, insertar y aumentar refCount
		if (!exists) {
			insert(FRPair(fg, p));
			p->increaseRefCount();
		}
	}
}

void ParticleForceRegistry::removeRegistry(ForceGenerator* fg, Particle* p) {
	if (fg != nullptr && p != nullptr) {
		for (auto it = begin(); it != end(); ++it) {
			if (it->first == fg && it->second == p) {
				erase(it);
				break;
			}
		}
	}
}

void ParticleForceRegistry::deleteParticleRegistry(Particle* p) {
	for (auto it = begin(); it != end();) {
		if (it->second == p && p->getDelete()) {
			it = erase(it);
			p->release();
		} else ++it;
	}
}

void ParticleForceRegistry::removeForceGenerator(ForceGenerator* fg) {
	auto it = begin();
	while (it != end()) {
		if (it->first == fg) {
			it->first->clearForce(it->second);
			it->second->decreaseRefCount();
			it = erase(it);
		}
		else ++it;
	}
}