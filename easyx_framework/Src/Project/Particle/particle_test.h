#pragma once
#include"CoreMinimal.h"
#include"Object.h"

class Particle_ember :public Object
{

public:
	Particle_ember();

private:
	class Particle* particle;
	/*class Particle* particle_;*/

};