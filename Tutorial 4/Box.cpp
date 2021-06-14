#include "Box.h"

namespace PhysicsEngine
{
	Box::Box(const PxTransform& pose, PxVec3 dimensions, PxReal density)
		: DynamicActor(pose)
	{
		CreateShape(PxBoxGeometry(dimensions), density);
	}
}