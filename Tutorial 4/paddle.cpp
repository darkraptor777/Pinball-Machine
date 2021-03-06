#include "paddle.h"


namespace PhysicsEngine
{
	std::vector<PxVec3> Paddlevert = {
		PxVec3(1.000000, 0.502064, -4.196961),
		PxVec3(1.000000, -0.502064, -4.196961),
		PxVec3(0.984461, 0.237494, 4.977801),
		PxVec3(0.984461, -0.237494, 4.977801),
		PxVec3(-1.000000, 0.502064, -4.196961),
		PxVec3(-1.000000, -0.502064, -4.196961),
		PxVec3(-1.000000, 0.243162, 5.000000),
		PxVec3(-1.000000, -0.243162, 5.000000),
		PxVec3(-1.000000, -1.300000, -3.571429),
		PxVec3(-1.000000, -1.592163, -2.142857),
		PxVec3(-1.000000, -1.300000, -0.714286),
		PxVec3(-1.000000, -0.893330, 0.714286),
		PxVec3(-1.000000, -0.579577, 2.142857),
		PxVec3(-0.968922, -0.373324, 3.582528),
		PxVec3(1.000000, 1.300000, -3.571429),
		PxVec3(1.000000, 1.592163, -2.142857),
		PxVec3(1.000000, 1.300000, -0.714286),
		PxVec3(1.000000, 0.893330, 0.714286),
		PxVec3(1.000000, 0.581203, 2.142857),
		PxVec3(0.984461, 0.373324, 3.582528),
		PxVec3(-0.968922, 0.373324, 3.582528),
		PxVec3(-1.000000, 0.581203, 2.142857),
		PxVec3(-1.000000, 0.893330, 0.714286),
		PxVec3(-1.000000, 1.300000, -0.714286),
		PxVec3(-1.000000, 1.592163, -2.142857),
		PxVec3(-1.000000, 1.300000, -3.571429),
		PxVec3(0.984653, -0.381074, 3.564723),
		PxVec3(0.999808, -0.575079, 2.160662),
		PxVec3(1.000000, -0.893330, 0.714286),
		PxVec3(1.000000, -1.300000, -0.714286),
		PxVec3(1.000000, -1.592163, -2.142857),
		PxVec3(1.000000, -1.300000, -3.571429)
	};
	Paddle::Paddle(PxTransform pose = PxTransform(PxIdentity), PxReal density = 1.f) :
		ConvexMesh(vector<PxVec3>(begin(Paddlevert), end(Paddlevert)), pose, density)
	{

	}
	
}
