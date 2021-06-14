#pragma once
#include "DynamicActor.h"

namespace PhysicsEngine
{
	class Capsule : public DynamicActor
	{
		public:
			Capsule(const PxTransform& pose = PxTransform(PxIdentity), PxVec2 dimension = PxVec2(1.f, 1.f), PxReal density = 1.f);
	};
}



