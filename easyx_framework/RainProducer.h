#pragma once
#include"CoreMinimal.h"
#include"Object.h"

class RainProducer :public Object
{

public:
	RainProducer();

private:
	class Particle* particle;
	class Particle* particle_;

};