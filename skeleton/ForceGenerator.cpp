#include "ForceGenerator.h"

ForceGenerator::ForceGenerator(string Name, double Duration, bool Active) : 
	name(Name), time(0.0f), duration(Duration), active(Active) {}

bool ForceGenerator::updateTime(double t) {
	if (active) {
		time += t;
	}
	return time < duration || duration <= 0.0;
}