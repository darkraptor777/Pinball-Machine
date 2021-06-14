#pragma once
#include "BasicActors.h"
#include "StaticActor.h"

namespace PhysicsEngine
{
	class PinballMachine : public StaticActor
	{
	public:
		PinballMachine(const PxTransform& pose = PxTransform(PxIdentity), PxReal density = 1.f);

	//private:
		//PxShape* base;
	};
}

