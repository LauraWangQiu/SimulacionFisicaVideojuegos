#pragma once
#include <iostream>
#include <string>
#include <list>
#include <random>
#include <math.h>
#include "RenderUtils.hpp"
#include "Particle.h"
using namespace std;
using namespace physx;

class ForceGenerator {
protected:
	string name;
	double time, duration;
	bool active;

public:
	ForceGenerator(string Name, double Duration, bool Active = false);

	virtual Vector3 calculateForce(Particle* particle) = 0;
	virtual void updateForce(Particle* particle);
	virtual void clearForce(Particle* particle);
	bool updateTime(double t);

	// Getters y setters
	inline string getName() const { return name; }
	inline void setName(string Name) { name = Name; }
	inline double getTime() const { return time; }
	inline void setTime(double Time) { time = Time; }
	inline double getDuration() const { return duration; }
	inline void setDuration(double Duration) { duration = Duration; }
	inline bool getActive() const { return active; }
	inline void setActive(bool Active) { active = Active; }
};

