#include "ForceGenerator.h"

ForceGenerator::ForceGenerator(string Name, double Time, double Duration, bool Active) : 
	name(Name), time(Time), duration(Duration), active(Active) {}

bool ForceGenerator::updateTime(double t) {
	time += t;
	return t < duration || duration < 0.0;
}