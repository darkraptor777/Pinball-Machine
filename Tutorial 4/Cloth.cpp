#include "Cloth.h"


namespace PhysicsEngine
{
	Cloth::Cloth(PxTransform pose, const PxVec2& size, PxU32 width, PxU32 height, bool fix_top)
	{
		//prepare vertices
		PxReal w_step = size.x / width;
		PxReal h_step = size.y / height;

		PxClothParticle* vertices = new PxClothParticle[(width + 1) * (height + 1) * 4];
		PxU32* quads = new PxU32[width * height * 4];

		for (PxU32 j = 0; j < (height + 1); j++)
		{
			for (PxU32 i = 0; i < (width + 1); i++)
			{
				PxU32 offset = i + j * (width + 1);
				vertices[offset].pos = PxVec3(w_step * i, 0.f, h_step * j);
				if (fix_top && (j == 0)) //fix the top row of vertices
					vertices[offset].invWeight = 0.f;
				else
					vertices[offset].invWeight = 1.f;
			}

			for (PxU32 j = 0; j < height; j++)
			{
				for (PxU32 i = 0; i < width; i++)
				{
					PxU32 offset = (i + j * width) * 4;
					quads[offset + 0] = (i + 0) + (j + 0) * (width + 1);
					quads[offset + 1] = (i + 1) + (j + 0) * (width + 1);
					quads[offset + 2] = (i + 1) + (j + 1) * (width + 1);
					quads[offset + 3] = (i + 0) + (j + 1) * (width + 1);
				}
			}
		}

		//init cloth mesh description
		mesh_desc.points.data = vertices;
		mesh_desc.points.count = (width + 1) * (height + 1);
		mesh_desc.points.stride = sizeof(PxClothParticle);

		mesh_desc.invMasses.data = &vertices->invWeight;
		mesh_desc.invMasses.count = (width + 1) * (height + 1);
		mesh_desc.invMasses.stride = sizeof(PxClothParticle);

		mesh_desc.quads.data = quads;
		mesh_desc.quads.count = width * height;
		mesh_desc.quads.stride = sizeof(PxU32) * 4;

		//create cloth fabric (cooking)
		PxClothFabric* fabric = PxClothFabricCreate(*GetPhysics(), mesh_desc, PxVec3(0, -1, 0));

		//create cloth
		actor = (PxActor*)GetPhysics()->createCloth(pose, *fabric, vertices, PxClothFlags());
		//collisions with the scene objects
		((PxCloth*)actor)->setClothFlag(PxClothFlag::eSCENE_COLLISION, true);

		colors.push_back(default_color);
		actor->userData = new UserData(&colors.back(), &mesh_desc);
	}

	Cloth::~Cloth()
	{
		delete (UserData*)actor->userData;
	}
}