#pragma once
#include "Joint.h"
#include "Actor.h"

namespace PhysicsEngine
{
	//Distance joint with the springs switched on
	class DistanceJoint : public Joint
	{
	public:
		DistanceJoint(Actor* actor0, const PxTransform& localFrame0, Actor* actor1, const PxTransform& localFrame1);

		void Stiffness(PxReal value);

		PxReal Stiffness();

		void Damping(PxReal value);

		PxReal Damping();
	};
}

