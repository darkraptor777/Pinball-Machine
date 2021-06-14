#include "ramp.h"
namespace PhysicsEngine
{
	std::vector<PxVec3> Rampvert = {
PxVec3(22, - 2.537905, - 1.220539),
PxVec3(22, 2.537905, - 1.220539),
PxVec3(- 26, - 2.537905, - 1.220536),
PxVec3(- 26, 2.537905, - 1.220536),
PxVec3(22, - 2.537905, - 7.431785),
PxVec3(22, 2.537905, - 7.431785),
PxVec3(- 26, - 2.537905, - 15),
PxVec3(- 26, 2.537905, - 15)
	};
	Ramp::Ramp(PxTransform pose = PxTransform(PxIdentity), PxReal density = 1.f) :
		ConvexMesh(vector<PxVec3>(begin(Rampvert), end(Rampvert)), pose, density)
	{

	}

}