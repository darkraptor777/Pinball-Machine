
#include "ConvexMesh.h"
#pragma once
namespace PhysicsEngine
{
	class Paddle : public ConvexMesh
	{
		public:
			Paddle(PxTransform pose, PxReal density);

		//private:


		//pyramid triangles: a list of three vertices for each triangle e.g. the first triangle consists of vertices 1, 4 and 0
		//vertices have to be specified in a counter-clockwise order to assure the correct shading in rendering
		//const static PxU32 trigs[] = { 1, 4, 0, 3, 1, 0, 2, 3, 0, 4, 2, 0, 3, 2, 1, 2, 4, 1 };
	};
}
