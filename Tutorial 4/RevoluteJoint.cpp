#include "RevoluteJoint.h"

namespace PhysicsEngine
{
	RevoluteJoint::RevoluteJoint(Actor* actor0, const PxTransform& localFrame0, Actor* actor1, const PxTransform& localFrame1)
	{
		PxRigidActor* px_actor0 = 0;
		if (actor0)
			px_actor0 = (PxRigidActor*)actor0->Get();

		joint = PxRevoluteJointCreate(*GetPhysics(), px_actor0, localFrame0, (PxRigidActor*)actor1->Get(), localFrame1);
		joint->setConstraintFlag(PxConstraintFlag::eVISUALIZATION, true);
	}

	void RevoluteJoint::DriveVelocity(PxReal value)
	{
		//wake up the attached actors
		PxRigidDynamic* actor_0, * actor_1;
		((PxRevoluteJoint*)joint)->getActors((PxRigidActor*&)actor_0, (PxRigidActor*&)actor_1);
		if (actor_0)
		{
			if (actor_0->isSleeping())
				actor_0->wakeUp();
		}
		if (actor_1)
		{
			if (actor_1->isSleeping())
				actor_1->wakeUp();
		}
		((PxRevoluteJoint*)joint)->setDriveVelocity(value);
		((PxRevoluteJoint*)joint)->setRevoluteJointFlag(PxRevoluteJointFlag::eDRIVE_ENABLED, true);
	}

	PxReal RevoluteJoint::DriveVelocity()
	{
		return ((PxRevoluteJoint*)joint)->getDriveVelocity();
	}

	void RevoluteJoint::SetLimits(PxReal lower, PxReal upper)
	{
		((PxRevoluteJoint*)joint)->setLimit(PxJointAngularLimitPair(lower, upper));
		((PxRevoluteJoint*)joint)->setRevoluteJointFlag(PxRevoluteJointFlag::eLIMIT_ENABLED, true);
	}
}