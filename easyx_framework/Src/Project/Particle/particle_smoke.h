#pragma once
#include"CoreMinimal.h"
#include"Object.h"

class Particle_smoke :public Object
{

public:
	Particle_smoke();

private:
	class Particle* particle;
	/*class Particle* particle_;*/

};