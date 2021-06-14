#include "Actor.h"


namespace PhysicsEngine
{
	using namespace physx;
	using namespace std;

	///Actor methods

	PxActor* Actor::Get()
	{
		return actor;
	}

	void Actor::Color(PxVec3 new_color, PxU32 shape_index)
	{
		//change color of all shapes
		if (shape_index == -1)
		{
			for (unsigned int i = 0; i < colors.size(); i++)
				colors[i] = new_color;
		}
		//or only the selected one
		else if (shape_index < colors.size())
		{
			colors[shape_index] = new_color;
		}
	}

	const PxVec3* Actor::Color(PxU32 shape_indx)
	{
		if (shape_indx < colors.size())
			return &colors[shape_indx];
		else
			return 0;
	}

	void Actor::Material(PxMaterial* new_material, PxU32 shape_index)
	{
		std::vector<PxShape*> shape_list = GetShapes(shape_index);
		for (PxU32 i = 0; i < shape_list.size(); i++)
		{
			std::vector<PxMaterial*> materials(shape_list[i]->getNbMaterials());
			for (unsigned int j = 0; j < materials.size(); j++)
				materials[j] = new_material;
			shape_list[i]->setMaterials(materials.data(), (PxU16)materials.size());
		}
	}

	PxShape* Actor::GetShape(PxU32 index)
	{
		std::vector<PxShape*> shapes(((PxRigidActor*)actor)->getNbShapes());
		if (index < ((PxRigidActor*)actor)->getShapes((PxShape**)&shapes.front(), (PxU32)shapes.size()))
			return shapes[index];
		else
			return 0;
	}

	std::vector<PxShape*> Actor::GetShapes(PxU32 index)
	{
		std::vector<PxShape*> shapes(((PxRigidActor*)actor)->getNbShapes());
		((PxRigidActor*)actor)->getShapes((PxShape**)&shapes.front(), (PxU32)shapes.size());
		if (index == -1)
			return shapes;
		else if (index < shapes.size())
		{
			return std::vector<PxShape*>(1, shapes[index]);
		}
		else
			return std::vector<PxShape*>();
	}

	void Actor::Name(const string& new_name)
	{
		name = new_name;
		actor->setName(name.c_str());
	}

	string Actor::Name()
	{
		return name;
	}

	void Actor::SetTrigger(bool value, PxU32 shape_index)
	{
		std::vector<PxShape*> shape_list = GetShapes(shape_index);
		for (PxU32 i = 0; i < shape_list.size(); i++)
		{
			shape_list[i]->setFlag(PxShapeFlag::eSIMULATION_SHAPE, !value);
			shape_list[i]->setFlag(PxShapeFlag::eTRIGGER_SHAPE, value);
		}
	}

	void Actor::SetupFiltering(PxU32 filterGroup, PxU32 filterMask, PxU32 shape_index)
	{
		std::vector<PxShape*> shape_list = GetShapes(shape_index);
		for (PxU32 i = 0; i < shape_list.size(); i++)
			shape_list[i]->setSimulationFilterData(PxFilterData(filterGroup, filterMask, 0, 0));

		// PxFilterData(word0, word1, 0, 0)
		// word0 = own ID
		// word1 = ID mask to filter pairs that trigger a contact callback
	}


}