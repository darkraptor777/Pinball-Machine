#include "ConvexMesh.h"
#pragma once
namespace PhysicsEngine
{
	class Guide : public ConvexMesh
	{
	public:
		Guide(PxTransform pose, PxReal density);

	};
}
