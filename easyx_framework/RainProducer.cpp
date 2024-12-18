#include"RainProducer.h"
#include"Particle.h"



RainProducer::RainProducer()
{
	particle = ConstructComponent<Particle>();
	particle->AttachTo(root);
	particle->SetCapacity(60);
	particle->SetInterval(0.008f);
	particle->Load("rain_bg");
	particle->SetMaxSpeed(3500);
	particle->SetMinSpeed(2500);
	particle->SetPattern(ParticlePattern::Line);
	particle->SetLine(2250, -90);
	particle->SetLayer(10);
	//particle->SetLifetime(0.5f);
	particle->SetLocalScale({ 0.75f,0.5f });

	particle_ = ConstructComponent<Particle>();
	particle_->AttachTo(root);
	particle_->SetCapacity(10);
	particle_->SetInterval(0.04f);
	particle_->Load("rain_bg_");
	particle_->SetMaxSpeed(4000);
	particle_->SetMinSpeed(2000);
	particle_->SetPattern(ParticlePattern::Line);
	particle_->SetLine(2000, -90);
	//particle_->SetLifetime(0.4f);
	particle_->SetLayer(5);

	//particle = Object::ConstructComponent<Particle>();
	//particle->AttachTo(root);
	//particle->SetInterval(0.008f);
	//particle->Load("mineparticle");
	//particle->SetCapacity(60);
	//particle->SetPattern((ParticlePattern::Center));
	//particle->SetMaxSpeed(4000);
	//particle->SetMinSpeed(2000);
	///*particle->SetSpeed(100.f);*/
	//particle->SetLayer(10);
	//particle->SetLifetime(1.f);
	
}