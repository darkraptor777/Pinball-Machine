#include "Scene.h"

namespace PhysicsEngine
{
	using namespace physx;
	using namespace std;

	///Scene methods
	void Scene::Init()
	{
		//scene
		PxSceneDesc sceneDesc(GetPhysics()->getTolerancesScale());

		if (!sceneDesc.cpuDispatcher)
		{
			PxDefaultCpuDispatcher* mCpuDispatcher = PxDefaultCpuDispatcherCreate(1);
			sceneDesc.cpuDispatcher = mCpuDispatcher;
		}

		sceneDesc.filterShader = PxDefaultSimulationFilterShader;

		px_scene = GetPhysics()->createScene(sceneDesc);

		if (!px_scene)
			throw new Exception("PhysicsEngine::Scene::Init, Could not initialise the scene.");

		//default gravity
		px_scene->setGravity(PxVec3(0.0f, -9.81f, 0.0f));

		CustomInit();

		pause = false;

		selected_actor = 0;

		SelectNextActor();
	}

	void Scene::Update(PxReal dt)
	{
		if (pause)
			return;

		CustomUpdate();

		px_scene->simulate(dt);
		px_scene->fetchResults(true);
	}

	void Scene::Add(Actor* actor)
	{
		px_scene->addActor(*actor->Get());
	}

	PxScene* Scene::Get()
	{
		return px_scene;
	}

	void Scene::Reset()
	{
		px_scene->release();
		Init();
	}

	void Scene::Pause(bool value)
	{
		pause = value;
	}

	bool Scene::Pause()
	{
		return pause;
	}

	PxRigidDynamic* Scene::GetSelectedActor()
	{
		return selected_actor;
	}

	void Scene::SelectNextActor()
	{
#if PX_PHYSICS_VERSION < 0x304000 // SDK 3.3
		std::vector<PxRigidDynamic*> actors(px_scene->getNbActors(PxActorTypeSelectionFlag::eRIGID_DYNAMIC));
		if (actors.size() && (px_scene->getActors(PxActorTypeSelectionFlag::eRIGID_DYNAMIC, (PxActor**)&actors.front(), (PxU32)actors.size())))
#else
		std::vector<PxRigidDynamic*> actors(px_scene->getNbActors(PxActorTypeFlag::eRIGID_DYNAMIC));
		if (actors.size() && (px_scene->getActors(PxActorTypeFlag::eRIGID_DYNAMIC, (PxActor**)&actors.front(), (PxU32)actors.size())))
#endif
		{
			if (selected_actor)
			{
				for (unsigned int i = 0; i < actors.size(); i++)
					if (selected_actor == actors[i])
					{
						HighlightOff(selected_actor);
						//select the next actor
						selected_actor = actors[(i + 1) % actors.size()];
						break;
					}
			}
			else
			{
				selected_actor = actors[0];
			}
			HighlightOn(selected_actor);
		}
		else
			selected_actor = 0;
	}

	std::vector<PxActor*> Scene::GetAllActors()
	{
#if PX_PHYSICS_VERSION < 0x304000 // SDK 3.3
		physx::PxActorTypeSelectionFlags selection_flag = PxActorTypeSelectionFlag::eRIGID_DYNAMIC | PxActorTypeSelectionFlag::eRIGID_STATIC |
			PxActorTypeSelectionFlag::eCLOTH;
#else
		physx::PxActorTypeFlags selection_flag = PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC |
			PxActorTypeFlag::eCLOTH;
#endif
		std::vector<PxActor*> actors(px_scene->getNbActors(selection_flag));
		px_scene->getActors(selection_flag, (PxActor**)&actors.front(), (PxU32)actors.size());
		return actors;
	}

	void Scene::HighlightOn(PxRigidDynamic* actor)
	{
		//store the original colour and adjust brightness of the selected actor
		std::vector<PxShape*> shapes(actor->getNbShapes());
		actor->getShapes((PxShape**)&shapes.front(), (PxU32)shapes.size());

		sactor_color_orig.clear();

		for (unsigned int i = 0; i < shapes.size(); i++)
		{
			PxVec3* color = ((UserData*)shapes[i]->userData)->color;
			sactor_color_orig.push_back(*color);
			*color += PxVec3(.2f, .2f, .2f);
		}
	}

	void Scene::HighlightOff(PxRigidDynamic* actor)
	{
		//restore the original color
		std::vector<PxShape*> shapes(actor->getNbShapes());
		actor->getShapes((PxShape**)&shapes.front(), (PxU32)shapes.size());

		for (unsigned int i = 0; i < shapes.size(); i++)
			*((UserData*)shapes[i]->userData)->color = sactor_color_orig[i];
	}

}