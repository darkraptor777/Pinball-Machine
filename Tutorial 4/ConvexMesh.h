#pragma once
#include "DynamicActor.h"

namespace PhysicsEngine
{

	///The ConvexMesh class
	class ConvexMesh : public DynamicActor
	{
		public:
			//constructor
			ConvexMesh(const std::vector<PxVec3>& verts, const PxTransform& pose, PxReal density);

			//mesh cooking (preparation)
			PxConvexMesh* CookMesh(const PxConvexMeshDesc& mesh_desc);
	};
}