#pragma once

#include "Actor.h"
namespace PhysicsEngine
{
	using namespace physx;
	using namespace std;

	class StaticActor : public Actor
	{
	public:
		StaticActor(const PxTransform& pose);

		~StaticActor();

		void CreateShape(const PxGeometry& geometry, PxReal density = 0.f);
	};
}

