#include "Guide.h"

namespace PhysicsEngine
{
	std::vector<PxVec3> Guidevert = {
PxVec3(8, -2.5, 22),
PxVec3(8, 2.5, 22),
PxVec3(-5, -2.5, -29),
PxVec3(-5, 2.5, -29),
PxVec3(8, -2.5, -29),
PxVec3(8, 2.5, -29)
	};
	Guide::Guide(PxTransform pose = PxTransform(PxIdentity), PxReal density = 1.f) :
		ConvexMesh(vector<PxVec3>(begin(Guidevert), end(Guidevert)), pose, density)
	{

	}

}