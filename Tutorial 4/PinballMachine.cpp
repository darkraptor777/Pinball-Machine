#include "PinballMachine.h"
namespace PhysicsEngine
{
	PinballMachine::PinballMachine(const PxTransform& pose, PxReal density) : StaticActor(pose)
	{
		//base = new BoxStatic(PxTransform(PxVec3(10.0f, 0.0f, 0.0f), PxQuat(0.113446f)), PxVec3(2.f, 2.f, 2.f));

		float baseLength = 119.0f;			 //130-11
		float baseWidth = 56.0f;
		float baseHeight = 94.0f;
		float baseThickness = 24.0f;
		float inclination = 0.113446f; //6.5 degrees in radians

		float wallThickness = 1.0f;
		float wallLength = 97.0f; //130-11-20-1-1
		float wallWidth = 54.0f; //56 -1 -1
		float wallHeight = 12.0f; //estimate

		float legRadius = 1.5f;
		//adjustments to account for base inclination
		float legHeightAdjust_Front = -8.0f;
		float legHeightAdjust_Back = 8.0f;
		float legPositionAdjust = 3.0f;

		float backboardHeight = 80.0f;
		float backboardThickness = 9.0f;

		float inside = 0.001f; //prevents texture glitch
		//base
		CreateShape(PxBoxGeometry(PxVec3(baseWidth/2, baseThickness/2, baseLength/2)), density);
		GetShape(0)->setLocalPose(PxTransform(PxVec3(0.0f, baseHeight, 0.0f) , PxQuat(1.0f, 0.0f, 0.0f, -inclination)));

		//legs
		CreateShape(PxBoxGeometry(PxVec3(legRadius, baseHeight /2 + legHeightAdjust_Front, legRadius)), density);										
		CreateShape(PxBoxGeometry(PxVec3(legRadius, baseHeight /2 + legHeightAdjust_Front, legRadius)), density);
		CreateShape(PxBoxGeometry(PxVec3(legRadius, baseHeight /2 + legHeightAdjust_Back, legRadius)), density);
		CreateShape(PxBoxGeometry(PxVec3(legRadius, baseHeight /2 + legHeightAdjust_Back, legRadius)), density);
		GetShape(1)->setLocalPose(PxTransform(PxVec3(baseWidth / 2 - legRadius - inside, baseHeight / 2 + legHeightAdjust_Front, baseLength / 2 - legRadius - legPositionAdjust)));
		GetShape(2)->setLocalPose(PxTransform(PxVec3(-baseWidth / 2 + legRadius + inside, baseHeight / 2 + legHeightAdjust_Front, baseLength / 2 - legRadius - legPositionAdjust)));
		GetShape(3)->setLocalPose(PxTransform(PxVec3(baseWidth / 2 - legRadius - inside, baseHeight / 2 + legHeightAdjust_Back, -baseLength / 2 + legRadius + legPositionAdjust)));
		GetShape(4)->setLocalPose(PxTransform(PxVec3(-baseWidth / 2 + legRadius + inside, baseHeight / 2 + legHeightAdjust_Back, -baseLength / 2 + legRadius + legPositionAdjust)));

		CreateShape(PxBoxGeometry(PxVec3(wallWidth / 2 + wallThickness, wallHeight / 2, wallThickness)), density);
		CreateShape(PxBoxGeometry(PxVec3(wallWidth / 2 + wallThickness, wallHeight / 2, wallThickness)), density);
		CreateShape(PxBoxGeometry(PxVec3(wallThickness, wallHeight / 2, wallLength/2 + wallThickness)), density);
		CreateShape(PxBoxGeometry(PxVec3(wallThickness, wallHeight / 2, wallLength/2 + wallThickness)), density);
		GetShape(5)->setLocalPose(PxTransform(PxVec3(0.0f, baseHeight +(baseThickness/2)+(wallHeight/2)-11.15f, wallLength/2), PxQuat(1.0f, 0.0f, 0.0f, -inclination)));
		GetShape(6)->setLocalPose(PxTransform(PxVec3(0.0f, baseHeight +(baseThickness/2)+ (wallHeight / 2) +11.2f, -wallLength/2), PxQuat(1.0f, 0.0f, 0.0f, -inclination)));
		GetShape(7)->setLocalPose(PxTransform(PxVec3(wallWidth/2-inside, baseHeight + (baseThickness / 2) + (wallHeight / 2), 0.0f), PxQuat(1.0f, 0.0f, 0.0f, -inclination)));
		GetShape(8)->setLocalPose(PxTransform(PxVec3(-wallWidth/2+inside, baseHeight + (baseThickness / 2) + (wallHeight / 2), 0.0f), PxQuat(1.0f, 0.0f, 0.0f, -inclination)));

		CreateShape(PxBoxGeometry(PxVec3(wallWidth / 2 + wallThickness, wallHeight / 2, 14.0f/2)), density);
		GetShape(9)->setLocalPose(PxTransform(PxVec3(0.0f, baseHeight + (baseThickness / 2) + (wallHeight / 2) - 12.75f, wallLength / 2 + 7.0f), PxQuat(1.0f, 0.0f, 0.0f, -inclination)));

		CreateShape(PxBoxGeometry(PxVec3(wallWidth / 2 + wallThickness, backboardHeight / 2, backboardThickness/2)), density);
		GetShape(10)->setLocalPose(PxTransform(PxVec3(0.0f, baseHeight + (baseThickness / 2) + (backboardHeight / 2), -(wallLength / 2) -(backboardThickness/2))));
	
	}
}