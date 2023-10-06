#include "GaussianParticleGenerator.h"

GaussianParticleGenerator::GaussianParticleGenerator(string Name, Vector3 MeanPos, Vector3 MeanVel, double GenerationProbability, int NumParticles, Particle* Model, 
	Vector3 StdDevPos, Vector3 StdDevVel, double StdDevTime) : ParticleGenerator(Name, MeanPos, MeanVel, GenerationProbability, NumParticles, Model),
	stdDevPos(StdDevPos), stdDevVel(StdDevVel), stdDevTime(StdDevTime) {}

GaussianParticleGenerator::~GaussianParticleGenerator() {}

list<Particle*> GaussianParticleGenerator::generateParticles() {
	list<Particle*> mGenerator;

    random_device rnd;

    normal_distribution<float> std_dis_posX(meanPos.x, stdDevPos.x);
    normal_distribution<float> std_dis_posY(meanPos.y, stdDevPos.y);
    normal_distribution<float> std_dis_posZ(meanPos.z, stdDevPos.z);

    normal_distribution<float> std_dis_velX(meanVel.x, stdDevVel.x);
    normal_distribution<float> std_dis_velY(meanVel.y, stdDevVel.y);
    normal_distribution<float> std_dis_velZ(meanVel.z, stdDevVel.z);

    uniform_real_distribution<double> std_dis_time(stdDevTime - stdDevTime / 2, stdDevTime + stdDevTime / 2);

    for (size_t i = 0; i < numParticles; ++i)
    {
        Particle* p = model->clone();

        if (p != nullptr)
        {
            p->setPos({ std_dis_posX(rnd), std_dis_posY(rnd), std_dis_posZ(rnd) });
            p->setVel({ std_dis_velX(rnd), std_dis_velY(rnd), std_dis_velZ(rnd) });

            p->setTime(std_dis_time(rnd));

            mGenerator.push_back(p);
        }
    }

	return mGenerator;
}