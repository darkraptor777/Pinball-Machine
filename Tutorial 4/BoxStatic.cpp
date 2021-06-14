#include "BoxStatic.h"

namespace PhysicsEngine
{
	BoxStatic::BoxStatic(const PxTransform& pose, PxVec3 dimensions, PxReal density)
		: StaticActor(pose)
	{
		CreateShape(PxBoxGeometry(dimensions), density);
	}
}