#pragma once
#include <map>
#include <string>
#include <list>
#include <random>
#include <math.h>
#include "RenderUtils.hpp"
#include "Particle.h"
#include "ForceGenerator.h"
using namespace std;
using namespace physx;

class ParticleForceRegistry : public multimap<ForceGenerator*, Particle*> {
public:
	void updateForces();
	void addRegistry(ForceGenerator* fg, Particle* p);
	void removeRegistry(ForceGenerator* fg, Particle* p);
	void deleteParticleRegistry(Particle* p);
	void removeForceGenerator(ForceGenerator* fg);
};

