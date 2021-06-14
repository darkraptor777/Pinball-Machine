#include "DistanceJoint.h"

namespace PhysicsEngine
{
	DistanceJoint::DistanceJoint(Actor* actor0, const PxTransform& localFrame0, Actor* actor1, const PxTransform& localFrame1)
	{
		PxRigidActor* px_actor0 = 0;
		if (actor0)
			px_actor0 = (PxRigidActor*)actor0->Get();

		joint = (PxJoint*)PxDistanceJointCreate(*GetPhysics(), px_actor0, localFrame0, (PxRigidActor*)actor1->Get(), localFrame1);
		joint->setConstraintFlag(PxConstraintFlag::eVISUALIZATION, true);
		((PxDistanceJoint*)joint)->setDistanceJointFlag(PxDistanceJointFlag::eSPRING_ENABLED, true);
		Damping(1.f);
		Stiffness(1.f);
	}

	void DistanceJoint::Stiffness(PxReal value)
	{
		((PxDistanceJoint*)joint)->setStiffness(value);
	}

	PxReal DistanceJoint::Stiffness()
	{
		return ((PxDistanceJoint*)joint)->getStiffness();
	}

	void DistanceJoint::Damping(PxReal value)
	{
		((PxDistanceJoint*)joint)->setDamping(value);
	}

	PxReal DistanceJoint::Damping()
	{
		return ((PxDistanceJoint*)joint)->getDamping();
	}
}