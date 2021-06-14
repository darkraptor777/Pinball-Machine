#include "Plane.h"

namespace PhysicsEngine
{
	Plane::Plane(PxVec3 normal, PxReal distance)
		: StaticActor(PxTransformFromPlaneEquation(PxPlane(normal, distance)))
	{
		CreateShape(PxPlaneGeometry());
	}
}