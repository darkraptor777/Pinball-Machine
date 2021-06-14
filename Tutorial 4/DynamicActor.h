#pragma once

#include "Actor.h"
namespace PhysicsEngine
{
	using namespace physx;
	using namespace std;

	class DynamicActor : public Actor
	{
	public:
		DynamicActor(const PxTransform& pose);

		~DynamicActor();

		void CreateShape(const PxGeometry& geometry, PxReal density);

		void SetKinematic(bool value, PxU32 index = -1);
	};
}
