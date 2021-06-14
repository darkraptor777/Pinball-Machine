#include "ConvexMesh.h"

namespace PhysicsEngine
{
	//constructor
	ConvexMesh::ConvexMesh(const std::vector<PxVec3>& verts, const PxTransform& pose = PxTransform(PxIdentity), PxReal density = 1.f)
		: DynamicActor(pose)
	{
		PxConvexMeshDesc mesh_desc;
		mesh_desc.points.count = (PxU32)verts.size();
		mesh_desc.points.stride = sizeof(PxVec3);
		mesh_desc.points.data = &verts.front();
		mesh_desc.flags = PxConvexFlag::eCOMPUTE_CONVEX;
		mesh_desc.vertexLimit = 256;

		CreateShape(PxConvexMeshGeometry(CookMesh(mesh_desc)), density);
	}

	//mesh cooking (preparation)
	PxConvexMesh* ConvexMesh::CookMesh(const PxConvexMeshDesc& mesh_desc)
	{
		PxDefaultMemoryOutputStream stream;

		if (!GetCooking()->cookConvexMesh(mesh_desc, stream))
			throw new Exception("ConvexMesh::CookMesh, cooking failed.");

		PxDefaultMemoryInputData input(stream.getData(), stream.getSize());

		return GetPhysics()->createConvexMesh(input);
	}
}