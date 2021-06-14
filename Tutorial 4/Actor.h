#pragma once

#include <vector>
#include "PxPhysicsAPI.h"
#include "PhysicsEngine.h"
#include "Exception.h"
#include "Extras\UserData.h"
#include <iostream>
#include <string>

///Abstract Actor class
///Inherit from this class to create your own actors
namespace PhysicsEngine
{
	using namespace physx;
	using namespace std;

	class Actor
	{
		protected:
			PxActor* actor;
			std::vector<PxVec3> colors;
			std::string name;

		public:
			///Constructor
			Actor()
				: actor(0)
			{
			}

			PxActor* Get();

			void Color(PxVec3 new_color, PxU32 shape_index = -1);

			const PxVec3* Color(PxU32 shape_indx = 0);

			void Actor::Name(const string& name);

			string Actor::Name();

			void Material(PxMaterial* new_material, PxU32 shape_index = -1);

			PxShape* GetShape(PxU32 index = 0);

			std::vector<PxShape*> Actor::GetShapes(PxU32 index = -1);

			virtual void CreateShape(const PxGeometry& geometry, PxReal density) {}

			void SetTrigger(bool value, PxU32 index = -1);

			void SetupFiltering(PxU32 filterGroup, PxU32 filterMask, PxU32 shape_index = -1);
	};
}

