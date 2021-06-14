#include "Corner.h"


namespace PhysicsEngine
{
	std::vector<PxVec3> Cornervert = {
PxVec3(26, -2.5, 0),
PxVec3(26, 2.5, 0),
PxVec3(-28, -2.5, -0.4),
PxVec3(-28, 2.5, -0.4),
PxVec3(26, -2.5, -18),
PxVec3(26, 2.5, -18)
	};
	Corner::Corner(PxTransform pose = PxTransform(PxIdentity), PxReal density = 1.f) :
		ConvexMesh(vector<PxVec3>(begin(Cornervert), end(Cornervert)), pose, density)
	{

	}

}