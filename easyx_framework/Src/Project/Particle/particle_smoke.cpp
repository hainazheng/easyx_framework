#include "ZHNpch.h"
#include "particle_smoke.h"
#include"Particle.h"
#include"ZHN_Math.h"

Particle_smoke::Particle_smoke()
{
	particle = ConstructComponent<Particle>();
	particle->AttachTo(root);
	particle->SetCapacity(60);
	particle->SetInterval(0.5f);
	particle->Load("smoke_particle_");
	particle->SetMaxSpeed(300);
	particle->SetMinSpeed(200);
	particle->SetFadingtime(static_cast<float>(Math::RandReal(1, 2)));
	particle->SetPattern(ParticlePattern::Line);
	particle->SetLine(10, Math::RandInt(100, 160));
	particle->SetLayer(11);
	particle->SetLifetime(static_cast<float>(Math::RandReal(2, 3)));
	particle->SetLocalScale({ static_cast<float>(Math::RandReal(2,4)),static_cast<float>(Math::RandReal(2,4)) });
	/*particle->SetFilter(true, RGB(Math::RandInt(0, 24), Math::RandInt(0, 10), Math::RandInt(0, 20)), Math::RandInt(50, 100));
	particle->EnableFilter();*/
}
