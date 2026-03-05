#include "ZHNpch.h"
#include "particle_test.h"
#include"Particle.h"
#include"ZHN_Math.h"

Particle_ember::Particle_ember()
{
	particle = ConstructComponent<Particle>();
	particle->AttachTo(root);
	particle->SetCapacity(60);
	particle->SetInterval(0.5f);
	particle->Load("ember_particle");
	particle->SetMaxSpeed(250);
	particle->SetMinSpeed(200);
	particle->SetFadingtime(static_cast<float>(Math::RandReal(1, 2)));
	particle->SetPattern(ParticlePattern::Line);
	particle->SetLine(10, Math::RandInt(20,70));
	particle->SetLayer(10);
	particle->SetLifetime(static_cast<float>(Math::RandReal(2, 3)));
	particle->SetLocalScale({ static_cast<float>(Math::RandReal(1,5)),static_cast<float>(Math::RandReal(1,5)) });
	particle->SetFilter(true, RGB(Math::RandInt(0, 24),Math::RandInt(0, 10), Math::RandInt(0, 20)), Math::RandInt(50, 100));
	particle->EnableFilter();

	//particle_ = ConstructComponent<Particle>();
	//particle_->AttachTo(root);
	//particle_->SetCapacity(10);
	//particle_->SetInterval(0.04f);
	//particle_->Load("rain_bg_");
	//particle_->SetMaxSpeed(4000);
	//particle_->SetMinSpeed(2000);
	//particle_->SetPattern(ParticlePattern::Line);
	//particle_->SetLine(2000, -90);
	////particle_->SetLifetime(0.4f);
	//particle_->SetLayer(5);

}
