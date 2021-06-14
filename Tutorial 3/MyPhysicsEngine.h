#pragma once

#include "BasicActors.h"
#include <iostream>
#include <iomanip>

namespace PhysicsEngine
{
	using namespace std;

	//a list of colours: Circus Palette
	static const PxVec3 color_palette[] = {PxVec3(46.f/255.f,9.f/255.f,39.f/255.f),PxVec3(217.f/255.f,0.f/255.f,0.f/255.f),
		PxVec3(255.f/255.f,45.f/255.f,0.f/255.f),PxVec3(255.f/255.f,140.f/255.f,54.f/255.f),PxVec3(4.f/255.f,117.f/255.f,111.f/255.f)};

	static const PxVec3 solar_palette[] = { PxVec3(246.f / 255.f, 139.f / 255.f, 39.f / 255.f), PxVec3(6.f / 255.f, 200.f / 255.f, 239.f / 255.f), PxVec3(86.f / 255.f, 26.f / 255.f, 2.f / 255.f), PxVec3(134.f / 255.f, 160.f / 255.f, 140.f / 255.f) };
	//pyramid vertices
	static PxVec3 pyramid_verts[] = {PxVec3(0,1,0), PxVec3(1,0,0), PxVec3(-1,0,0), PxVec3(0,0,1), PxVec3(0,0,-1)};
	//pyramid triangles: a list of three vertices for each triangle e.g. the first triangle consists of vertices 1, 4 and 0
	//vertices have to be specified in a counter-clockwise order to assure the correct shading in rendering
	static PxU32 pyramid_trigs[] = {1, 4, 0, 3, 1, 0, 2, 3, 0, 4, 2, 0, 3, 2, 1, 2, 4, 1};

	class Pyramid : public ConvexMesh
	{
	public:
		Pyramid(PxTransform pose=PxTransform(PxIdentity), PxReal density=1.f) :
			ConvexMesh(vector<PxVec3>(begin(pyramid_verts),end(pyramid_verts)), pose, density)
		{
		}
	};

	class PyramidStatic : public TriangleMesh
	{
	public:
		PyramidStatic(PxTransform pose=PxTransform(PxIdentity)) :
			TriangleMesh(vector<PxVec3>(begin(pyramid_verts),end(pyramid_verts)), vector<PxU32>(begin(pyramid_trigs),end(pyramid_trigs)), pose)
		{
		}
	};

	struct FilterGroup
	{
		enum Enum
		{
			ACTOR0		= (1 << 0),
			ACTOR1		= (1 << 1),
			ACTOR2		= (1 << 2)
			//add more if you need
		};
	};

	///An example class showing the use of springs (distance joints).
	class Trampoline
	{
		vector<DistanceJoint*> springs;
		Box *bottom, *top;

	public:
		Trampoline(const PxVec3& dimensions=PxVec3(1.f,1.f,1.f), PxReal stiffness=1.f, PxReal damping=1.f)
		{
			PxReal thickness = .1f;
			bottom = new Box(PxTransform(PxVec3(0.f,thickness,0.f)),PxVec3(dimensions.x,thickness,dimensions.z));
			top = new Box(PxTransform(PxVec3(0.f,dimensions.y+thickness,0.f)),PxVec3(dimensions.x,thickness,dimensions.z));
			springs.resize(4);
			springs[0] = new DistanceJoint(bottom, PxTransform(PxVec3(dimensions.x,thickness,dimensions.z)), top, PxTransform(PxVec3(dimensions.x,-dimensions.y,dimensions.z)));
			springs[1] = new DistanceJoint(bottom, PxTransform(PxVec3(dimensions.x,thickness,-dimensions.z)), top, PxTransform(PxVec3(dimensions.x,-dimensions.y,-dimensions.z)));
			springs[2] = new DistanceJoint(bottom, PxTransform(PxVec3(-dimensions.x,thickness,dimensions.z)), top, PxTransform(PxVec3(-dimensions.x,-dimensions.y,dimensions.z)));
			springs[3] = new DistanceJoint(bottom, PxTransform(PxVec3(-dimensions.x,thickness,-dimensions.z)), top, PxTransform(PxVec3(-dimensions.x,-dimensions.y,-dimensions.z)));

			for (unsigned int i = 0; i < springs.size(); i++)
			{
				springs[i]->Stiffness(stiffness);
				springs[i]->Damping(damping);
			}
		}

		void AddToScene(Scene* scene)
		{
			scene->Add(bottom);
			scene->Add(top);
		}

		~Trampoline()
		{
			for (unsigned int i = 0; i < springs.size(); i++)
				delete springs[i];
		}
	};

	///A customised collision class, implemneting various callbacks
	class MySimulationEventCallback : public PxSimulationEventCallback
	{
	public:
		//an example variable that will be checked in the main simulation loop
		bool trigger;

		MySimulationEventCallback() : trigger(false) {}

		///Method called when the contact with the trigger object is detected.
		virtual void onTrigger(PxTriggerPair* pairs, PxU32 count) 
		{
			//you can read the trigger information here
			for (PxU32 i = 0; i < count; i++)
			{
				//filter out contact with the planes
				if (pairs[i].otherShape->getGeometryType() != PxGeometryType::ePLANE)
				{
					//check if eNOTIFY_TOUCH_FOUND trigger
					if (pairs[i].status & PxPairFlag::eNOTIFY_TOUCH_FOUND)
					{
						cerr << "onTrigger::eNOTIFY_TOUCH_FOUND" << endl;
						trigger = true;
					}
					//check if eNOTIFY_TOUCH_LOST trigger
					if (pairs[i].status & PxPairFlag::eNOTIFY_TOUCH_LOST)
					{
						cerr << "onTrigger::eNOTIFY_TOUCH_LOST" << endl;
						trigger = false;
					}
				}
			}
		}

		///Method called when the contact by the filter shader is detected.
		virtual void onContact(const PxContactPairHeader &pairHeader, const PxContactPair *pairs, PxU32 nbPairs) 
		{
			cerr << "Contact found between " << pairHeader.actors[0]->getName() << " " << pairHeader.actors[1]->getName() << endl;

			//check all pairs
			for (PxU32 i = 0; i < nbPairs; i++)
			{
				//check eNOTIFY_TOUCH_FOUND
				if (pairs[i].events & PxPairFlag::eNOTIFY_TOUCH_FOUND)
				{
					cerr << "onContact::eNOTIFY_TOUCH_FOUND" << endl;
				}
				//check eNOTIFY_TOUCH_LOST
				if (pairs[i].events & PxPairFlag::eNOTIFY_TOUCH_LOST)
				{
					cerr << "onContact::eNOTIFY_TOUCH_LOST" << endl;
				}
			}
		}

		virtual void onConstraintBreak(PxConstraintInfo *constraints, PxU32 count) {}
		virtual void onWake(PxActor **actors, PxU32 count) {}
		virtual void onSleep(PxActor **actors, PxU32 count) {}
#if PX_PHYSICS_VERSION >= 0x304000
		virtual void onAdvance(const PxRigidBody *const *bodyBuffer, const PxTransform *poseBuffer, const PxU32 count) {}
#endif
	};

	//A simple filter shader based on PxDefaultSimulationFilterShader - without group filtering
	static PxFilterFlags CustomFilterShader( PxFilterObjectAttributes attributes0,	PxFilterData filterData0,
		PxFilterObjectAttributes attributes1,	PxFilterData filterData1,
		PxPairFlags& pairFlags,	const void* constantBlock,	PxU32 constantBlockSize)
	{
		// let triggers through
		if(PxFilterObjectIsTrigger(attributes0) || PxFilterObjectIsTrigger(attributes1))
		{
			pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
			return PxFilterFlags();
		}

		pairFlags = PxPairFlag::eCONTACT_DEFAULT;
		//enable continous collision detection
//		pairFlags |= PxPairFlag::eCCD_LINEAR;
		
		
		//customise collision filtering here
		//e.g.

		// trigger the contact callback for pairs (A,B) where 
		// the filtermask of A contains the ID of B and vice versa.
		if((filterData0.word0 & filterData1.word1) && (filterData1.word0 & filterData0.word1))
		{
			//trigger onContact callback for this pair of objects
			pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND;
			pairFlags |= PxPairFlag::eNOTIFY_TOUCH_LOST;
//			pairFlags |= PxPairFlag::eNOTIFY_CONTACT_POINTS;
		}

		return PxFilterFlags();
	};

	///Custom scene class
	class MyScene : public Scene
	{
		Plane* plane;
		Box* box, * box2;
		Sphere* sun, *planet1, *planet2, *planet3;
		Sphere* moon1, *moon2;
		RevoluteJoint* planet1Joint, *planet2Joint, *planet3Joint;
		RevoluteJoint* moon1Joint, *moon2Joint;
		PxDominanceGroup dominanceGroup;

		Trampoline* trampoline;
		MySimulationEventCallback* my_callback;
		RevoluteJoint* joint;
		PxReal gearVelo = 50.0f;
		
	public:
		//specify your custom filter shader here
		//PxDefaultSimulationFilterShader by default
		MyScene() : Scene() {};

		///A custom scene class
		void SetVisualisation()
		{
			px_scene->setVisualizationParameter(PxVisualizationParameter::eSCALE, 1.0f);
			px_scene->setVisualizationParameter(PxVisualizationParameter::eCOLLISION_SHAPES, 1.0f);
			//px_scene->setVisualizationParameter(PxVisualizationParameter::eACTOR_AXES, 2.0f);
			px_scene->setVisualizationParameter(PxVisualizationParameter::eBODY_LIN_VELOCITY, 1.5f);
			px_scene->setVisualizationParameter(PxVisualizationParameter::eJOINT_LOCAL_FRAMES, 2.0f);
			px_scene->setVisualizationParameter(PxVisualizationParameter::eJOINT_LIMITS, 2.0f);
		}

		//Custom scene initialisation
		virtual void CustomInit() 
		{
			SetVisualisation();			

			GetMaterial()->setDynamicFriction(.2f);

			///Initialise and set the customised event callback
			my_callback = new MySimulationEventCallback();
			px_scene->setSimulationEventCallback(my_callback);

			plane = new Plane();
			plane->Color(PxVec3(210.f/255.f,210.f/255.f,210.f/255.f));
			Add(plane);

			/*
			trampoline = new Trampoline(PxVec3(2.0f, 1.0f, 1.0f), 200.0f, 0.1f);
			trampoline->AddToScene(this);
			*/


			box = new Box(PxTransform(PxVec3(.0f,30.0f,.0f)));
			box->Color(color_palette[0]);

			//set collision filter flags
			// box->SetupFiltering(FilterGroup::ACTOR0, FilterGroup::ACTOR1);
			//use | operator to combine more actors e.g.
			// box->SetupFiltering(FilterGroup::ACTOR0, FilterGroup::ACTOR1 | FilterGroup::ACTOR2);
			//don't forget to set your flags for the matching actor as well, e.g.:
			// box2->SetupFiltering(FilterGroup::ACTOR1, FilterGroup::ACTOR0);
			box->Name("Box1");
			Add(box);

			
			box2 = new Box(PxTransform(PxVec3(.0f, 30.0f, 30.0f)));
			box2->Color(color_palette[1]);
			box2->Name("Box2");
			Add(box2);
			//joint two boxes together
			//the joint is fixed to the centre of the first box, oriented by 90 degrees around the Y axis
			//and has the second object attached 5 meters away along the Y axis from the first object.
			joint = new RevoluteJoint (box, PxTransform(PxVec3(0.f,0.f,0.f),PxQuat(PxPi/2,PxVec3(0.f,1.f,0.f))), box2, PxTransform(PxVec3(0.f,5.f,0.f)));
			
			joint->DriveVelocity(gearVelo);

			box->GetShape(0)->getActor()->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
			box2->GetShape(0)->getActor()->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);

			px_scene->setDominanceGroupPair(1, 2, PxDominanceGroupPair(0, 1));
			sun = new Sphere(PxTransform(PxVec3(.0f, 10.0f, .0f)),6.0f);
			sun->Color(solar_palette[0]);
			sun->Name("sun");
			Add(sun);
			sun->GetShape(0)->getActor()->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
			sun->GetShape(0)->getActor()->setDominanceGroup(1);		
			PxRigidDynamic* px_sun = (PxRigidDynamic*)sun->Get();
			px_sun->setMass(10000.0f);
			PxRigidBody* sun_rb = (PxRigidBody*)sun->Get();
			sun_rb->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);

			planet1 = new Sphere(PxTransform(PxVec3(.0f, 8.0f, .0f)), 0.7f);
			planet1->Color(solar_palette[1]);
			planet1->Name("planet1");
			Add(planet1);
			planet1->GetShape(0)->getActor()->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
			planet1Joint = new RevoluteJoint(sun, PxTransform(PxVec3(0.f, 0.f, 0.f), PxQuat(PxPi / 2, PxVec3(0.f, 0.f, 1.f))), planet1, PxTransform(PxVec3(0.f, 12.f, 0.f)));
			planet1Joint->DriveVelocity(2.0f);
			planet1->GetShape(0)->getActor()->setDominanceGroup(2);

			planet2 = new Sphere(PxTransform(PxVec3(.0f, 8.0f, .0f)), 1.2f);
			planet2->Color(solar_palette[2]);
			planet2->Name("planet2");
			Add(planet2);
			planet2->GetShape(0)->getActor()->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
			planet2Joint = new RevoluteJoint(sun, PxTransform(PxVec3(0.f, 0.f, 0.f), PxQuat(PxPi / 2, PxVec3(0.f, 0.f, 1.f))), planet2, PxTransform(PxVec3(0.f, 16.f, 0.f)));
			planet2Joint->DriveVelocity(1.0f);
			planet2->GetShape(0)->getActor()->setDominanceGroup(2);

			moon1 = new Sphere(PxTransform(PxVec3(.0f, 8.0f, .0f)), 0.2f);
			moon1->Color(solar_palette[3]);
			moon1->Name("moon1");
			Add(moon1);
			moon1->GetShape(0)->getActor()->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
			moon1Joint = new RevoluteJoint(planet2, PxTransform(PxVec3(0.f, 0.f, 0.f), PxQuat(PxPi / 2, PxVec3(0.f, 0.f, 1.f))), moon1, PxTransform(PxVec3(1.5f, 2.f, 0.f)));
			moon1Joint->DriveVelocity(3.0f);

			moon2 = new Sphere(PxTransform(PxVec3(.0f, 8.0f, .0f)), 0.2f);
			moon2->Color(solar_palette[3]);
			moon2->Name("moon2");
			Add(moon2);
			moon2->GetShape(0)->getActor()->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
			moon2Joint = new RevoluteJoint(planet2, PxTransform(PxVec3(0.f, 0.f, 0.f), PxQuat(PxPi / 2, PxVec3(0.f, 0.f, 1.f))), moon2, PxTransform(PxVec3(0.f, 2.f, 0.f)));
			moon2Joint->DriveVelocity(6.0f);

			planet3 = new Sphere(PxTransform(PxVec3(.0f, 8.0f, .0f)), 2.0f);
			planet3->Color(solar_palette[3]);
			planet3->Name("planet3");
			Add(planet3);
			planet3->GetShape(0)->getActor()->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
			planet3Joint = new RevoluteJoint(sun, PxTransform(PxVec3(0.f, 0.f, 0.f), PxQuat(PxPi / 2, PxVec3(0.f, 0.f, 1.f))), planet3, PxTransform(PxVec3(0.f, 20.f, 0.f)));
			planet3Joint->DriveVelocity(6.0f);
			planet3->GetShape(0)->getActor()->setDominanceGroup(2);
		}
		/*
		RevoluteJoint* GetJoint()
		{
			return joint;
		}
		 */
		void ReverseGear()
		{
			gearVelo = -gearVelo;
			joint->DriveVelocity(gearVelo);
		}

		//Custom udpate function
		virtual void CustomUpdate() 
		{
		}

		/// An example use of key release handling
		void ExampleKeyReleaseHandler()
		{
			cerr << "I am realeased!" << endl;
		}

		/// An example use of key presse handling
		void ExampleKeyPressHandler()
		{
			cerr << "I am pressed!" << endl;
		}
	};
}
