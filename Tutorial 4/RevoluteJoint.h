#pragma once
#include "Joint.h"
#include "Actor.h"

namespace PhysicsEngine
{

	///Revolute Joint
	class RevoluteJoint : public Joint
	{
		public:
			RevoluteJoint(Actor* actor0, const PxTransform& localFrame0, Actor* actor1, const PxTransform& localFrame1);

			void DriveVelocity(PxReal value);

			PxReal DriveVelocity();

			void SetLimits(PxReal lower, PxReal upper);
	};
}