#pragma once

#include "BasicActors.h"
#include "PinballMachine.h"
#include "Scene.h"
#include <iostream>
#include <iomanip>

//units: g, cm



namespace PhysicsEngine
{
	using namespace std;
	//colours
	static const PxVec3 red = PxVec3(240.f / 255.f, 0.f / 255.f, 0.f / 255.f);
	static const PxVec3 orange = PxVec3(255.f / 255.f, 45.f / 255.f, 0.f / 255.f);
	static const PxVec3 purple = PxVec3(155.f / 255.f, 5.f / 255.f, 155.f / 255.f);
	static const PxVec3 green = PxVec3(0.f / 255.f, 240.f / 255.f, 5.f / 255.f);
	static const PxVec3 grey = PxVec3(100.f / 255.f, 100.f / 255.f, 100.f / 255.f);
	static const PxVec3 yellow = PxVec3(211.f / 255.f, 169.f / 255.f, 0.f / 255.f);
	static const PxVec3 steel = PxVec3(164.f / 255.f, 171.f / 255.f, 181.f / 255.f);
	static const PxVec3 black = PxVec3(30.f / 255.f, 30.f / 255.f, 30.f / 255.f);
	static const PxVec3 color_palette[] = { red, orange, purple, green, grey, yellow, steel, black };
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
	
	///A customised collision class, implementing various callbacks
	class MySimulationEventCallback : public PxSimulationEventCallback
	{
	public:
		//an example variable that will be checked in the main simulation loop
		bool trigger;

		int score = 0;

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
						if(strcmp(pairs->otherActor->getName(), (const char*)"pinball")==0) //check if colliding with pinball
						{ //I forgot how much i dislike c++ string handling
							incrementScore(); 
							//((PxRigidDynamic*)pairs->otherActor)->addForce(((PxRigidDynamic*)pairs->otherActor)->getAngularVelocity() * -2.0f);
						}
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

		int getScore()//returns score (used by HUD)
		{
			return score;
		}
		void incrementScore()//increases score by ten
		{
			score += 10;
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
		Cloth* cloth;
		Cloth* cloth2;
		MySimulationEventCallback* my_callback;
		float x=0.0f;

		PinballMachine* pinballMach; //compound shape
		Ramp* bottomRamp;
		Corner* corner;
		Guide* lguide;
		Guide* rguide;

		Box* plunger;
		PxRigidDynamic* px_plunger;
		Sphere* plungeranchor;
		DistanceJoint* plungerjoint;

		Sphere* pinball;

		RevoluteJoint* paddleRjoint;
		RevoluteJoint* paddleLjoint;
		Sphere* paddleRanchor;
		Sphere* paddleLanchor;
		Paddle* paddleR;
		Paddle* paddleL;
		PxRigidDynamic* px_raddle;
		PxRigidDynamic* px_laddle;

		Box* spinnerL;
		Sphere* spinnerLanchor;
		RevoluteJoint* spinnerLjoint;
		Box* spinnerR;
		Sphere* spinnerRanchor;
		RevoluteJoint* spinnerRjoint;

		Sphere* bouncer;
		Sphere* bouncerTrigger;
		Sphere* bouncer2;
		Sphere* bouncerTrigger2;
		Sphere* bouncer3;
		Sphere* bouncerTrigger3;
		Sphere* bouncer4;
		Sphere* bouncerTrigger4;
		Sphere* bouncer5;
		Sphere* bouncerTrigger5;



	public:
		//specify your custom filter shader here
		//PxDefaultSimulationFilterShader by default
		MyScene() : Scene() {};

		int getScore()
		{
			return my_callback->getScore();
		}

		///A custom scene class
		void SetVisualisation()
		{
			px_scene->setVisualizationParameter(PxVisualizationParameter::eSCALE, 1.0f);
			px_scene->setVisualizationParameter(PxVisualizationParameter::eCOLLISION_SHAPES, 1.0f);

			//cloth visualisation
			px_scene->setVisualizationParameter(PxVisualizationParameter::eCLOTH_HORIZONTAL, 1.0f);
			px_scene->setVisualizationParameter(PxVisualizationParameter::eCLOTH_VERTICAL, 1.0f);
			px_scene->setVisualizationParameter(PxVisualizationParameter::eCLOTH_BENDING, 1.0f);
			px_scene->setVisualizationParameter(PxVisualizationParameter::eCLOTH_SHEARING, 1.0f);
		}

		//Custom scene initialisation
		virtual void CustomInit() 
		{
			SetVisualisation();			

			GetMaterial()->setDynamicFriction(.2f);
			CreateMaterial(0.3,0.3,0.603);//wood
			CreateMaterial(0.7, 0.42, 0.597);//steel
			CreateMaterial(0.2, 0.2, 0.688);//plastic

			///Initialise and set the customised event callback
			my_callback = new MySimulationEventCallback();
			px_scene->setSimulationEventCallback(my_callback);

			plane = new Plane(PxVec3(0.0f,1.0f,0.0f), 0.0f);
			plane->Color(PxVec3(210.f/255.f,210.f/255.f,210.f/255.f));
			Add(plane);


			//cloth bottom left
			cloth = new Cloth(PxTransform(PxVec3(-26.f,105.f,28.8f)), PxVec2(4.f,4.5f), 20, 4);
			cloth->Color(color_palette[2]);
			Add(cloth);
			//cloth bottom right (above plunger)
			cloth2 = new Cloth(PxTransform(PxVec3(22.f, 105.f, 28.8f)), PxVec2(4.f, 4.5f), 20, 4);
			cloth2->Color(color_palette[2]);
			Add(cloth2);
			//pinball machine frame/body
			pinballMach = new PinballMachine();
			pinballMach->Color(color_palette[4]);
			pinballMach->Name("PinballMach");
			Add(pinballMach);
			pinballMach->SetupFiltering(FilterGroup::ACTOR0, FilterGroup::ACTOR1);

			bottomRamp = new Ramp(PxTransform(PxVec3(0.f, 97.5f, 48.f), PxQuat(PxPi, PxVec3(1.f, 0.f, 0.f)) * PxQuat(1.0f, 0.0f, 0.0f, -0.113446f)), 1.0f);
			bottomRamp->Color(color_palette[1]);
			Add(bottomRamp);
			PxRigidDynamic* px_ramp = (PxRigidDynamic*)bottomRamp->Get();
			px_ramp->setMass(100.0f);
			PxRigidBody* ramp_rb = (PxRigidBody*)bottomRamp->Get();
			ramp_rb->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);

			corner = new Corner(PxTransform(PxVec3(0.f, 120.f, -49.f), PxQuat(1.0f, 0.0f, 0.0f, -0.113446f)), 1.0f);
			corner->Color(color_palette[1]);
			Add(corner);
			PxRigidDynamic* px_corner = (PxRigidDynamic*)corner->Get();
			px_corner->setMass(100.0f);
			PxRigidBody* corner_rb = (PxRigidBody*)corner->Get();
			corner_rb->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);

			rguide = new Guide(PxTransform(PxVec3(14.f, 109.f, 0.0f), PxQuat(1.0f, 0.0f, 0.0f, -0.113446f)), 1.0f);
			rguide->Color(color_palette[1]);
			Add(rguide);
			PxRigidDynamic* px_rguide = (PxRigidDynamic*)rguide->Get();
			px_rguide->setMass(100.0f);
			PxRigidBody* rguide_rb = (PxRigidBody*)rguide->Get();
			rguide_rb->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);

			lguide = new Guide(PxTransform(PxVec3(-14.f, 109.f, 0.0f), PxQuat(PxPi, PxVec3(0.f, 1.f, 0.f)) * PxQuat(PxPi, PxVec3(1.f, 0.f, 0.f)) * PxQuat(-1.0f, 0.0f, 0.0f, -0.113446f)), 1.0f);
			lguide->Color(color_palette[1]);
			Add(lguide);
			PxRigidDynamic* px_lguide = (PxRigidDynamic*)lguide->Get();
			px_lguide->setMass(100.0f);
			PxRigidBody* lguide_rb = (PxRigidBody*)lguide->Get();
			lguide_rb->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);

			plunger = new Box(PxTransform(PxVec3(24.f, 102.f, 34.f), PxQuat(1.0f, 0.0f, 0.0f, -0.113446f)), PxVec3(1.f, 1.f, 1.2f));
			plunger->Name("Plunger");
			plunger->Color(color_palette[5]);
			Add(plunger);
			plunger->SetupFiltering(FilterGroup::ACTOR1, FilterGroup::ACTOR0);
			plunger->GetShape(0)->getActor()->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
			px_plunger = (PxRigidDynamic*)plunger->Get();
			px_plunger->setMass(1000.0f);
			PxRigidBody* plunger_rb = (PxRigidBody*)plunger->Get();
			px_plunger->setRigidDynamicLockFlags(PxRigidDynamicLockFlag::eLOCK_LINEAR_X | PxRigidDynamicLockFlag::eLOCK_ANGULAR_X | PxRigidDynamicLockFlag::eLOCK_ANGULAR_X | PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z | PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y);//stop plunger moving weirdly
			px_plunger->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_CCD, true);//try to stop plunger getting stuck inside machine
			plungeranchor = new Sphere(PxTransform(PxVec3(24.f, 95.5f, 47.f)), 1.f, 1.0f);
			Add(plungeranchor);
			PxRigidDynamic* px_plungeranchor = (PxRigidDynamic*)plungeranchor->Get();
			px_plungeranchor->setMass(10.0f);
			PxRigidBody* plungeranchor_rb = (PxRigidBody*)plungeranchor->Get();
			plungeranchor_rb->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
			plungerjoint = new DistanceJoint(plungeranchor, PxTransform(PxVec3(0.f, 0.0f, 0.f)), plunger, PxTransform(PxVec3(0.f, 0.0f, 10.f)));
			plungerjoint->Damping(300.0f);
			plungerjoint->Stiffness(1590.0f);

			pinball = new Sphere(PxTransform(PxVec3(0.f, 100.0f, 0.f)), 1.5f, 8.0f);		//pinball mass is 80g with diameter of approx. 3cm
			pinball->Color(color_palette[6]);
			pinball->Name("pinball");
			bottomRamp->Material(GetMaterial(1));
			Add(pinball);
			PxRigidDynamic* px_pinball = (PxRigidDynamic*)pinball->Get();
			px_pinball->setMass(80);//mass is 80g? (not sure what units are)

			paddleR = new Paddle(PxTransform(PxVec3(5.f, 110.f, 0.f)), 1.0f);
			paddleR->Color(color_palette[5]);
			px_raddle = (PxRigidDynamic*)paddleR->Get();
			Add(paddleR);
			paddleRanchor = new Sphere(PxTransform(PxVec3(10.f, 100.5f, 30.f)), 0.1f, 1.0f);
			Add(paddleRanchor);
			PxRigidDynamic* px_ranchor = (PxRigidDynamic*)paddleRanchor->Get();
			px_ranchor->setMass(10000.0f);
			PxRigidBody* ranchor_rb = (PxRigidBody*)paddleRanchor->Get();
			ranchor_rb->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);

			paddleRjoint = new RevoluteJoint(paddleRanchor, PxTransform(PxVec3(0.f, 0.0f, 0.f), PxQuat(PxPi / 2, PxVec3(0.f, 0.f, 1.f))* PxQuat(0.0f, -1.0f, 0.0f, -0.113446f)), paddleR, PxTransform(PxVec3(0.f, 0.0f, -2.5f)));
			paddleRjoint->SetLimits(-2.0, -1.0);
			
			paddleL = new Paddle(PxTransform(PxVec3(-5.f, 110.f, 0.f)), 1.0f);
			paddleL->Color(color_palette[5]);
			px_laddle = (PxRigidDynamic*)paddleL->Get();
			Add(paddleL);
			paddleLanchor = new Sphere(PxTransform(PxVec3(-10.f, 100.5f, 30.f)), 0.1f, 1.0f);
			Add(paddleLanchor);
			PxRigidDynamic* px_lanchor = (PxRigidDynamic*)paddleLanchor->Get();
			px_lanchor->setMass(10000.0f);
			PxRigidBody* lanchor_rb = (PxRigidBody*)paddleLanchor->Get();
			lanchor_rb->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
			paddleLjoint = new RevoluteJoint(paddleLanchor, PxTransform(PxVec3(0.f, 0.0f, 0.f), PxQuat(PxPi / 2, PxVec3(0.f, 0.f, 1.f)) * PxQuat(0.0f, -1.0f, 0.0f, -0.113446f)), paddleL, PxTransform(PxVec3(0.f, 0.0f, -2.5f)));
			paddleLjoint->SetLimits(3-2.0, 3-1.0);


			spinnerL = new Box(PxTransform(PxVec3(-12.f, 102.f, 0.f), PxQuat(1.0f, 0.0f, 0.0f, -0.113446f)), PxVec3(1.f, 0.6f, 7.f));
			spinnerL->Color(color_palette[0]);
			Add(spinnerL);
			spinnerLanchor = new Sphere(PxTransform(PxVec3(-12.f, 112.5f, -20.f)), 0.1f, 1.0f);
			Add(spinnerLanchor);
			PxRigidDynamic* px_spinnerlanchor = (PxRigidDynamic*)spinnerLanchor->Get();
			px_spinnerlanchor->setMass(10000.0f);
			PxRigidBody* spinnerlanchor_rb = (PxRigidBody*)spinnerLanchor->Get();
			spinnerlanchor_rb->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
			spinnerLjoint = new RevoluteJoint(spinnerLanchor, PxTransform(PxVec3(0.f, 0.0f, 0.f), PxQuat(PxPi / 2, PxVec3(0.f, 0.f, 1.f)) * PxQuat(0.0f, -1.0f, 0.0f, -0.113446f)), spinnerL, PxTransform(PxVec3(0.f, 0.0f, 0.0f)));
			spinnerLjoint->DriveVelocity(10.0f);

			spinnerR = new Box(PxTransform(PxVec3(-12.f, 102.f, 0.f), PxQuat(1.0f, 0.0f, 0.0f, -0.113446f)), PxVec3(1.f, 0.6f, 4.f));
			spinnerR->Color(color_palette[0]);
			Add(spinnerR);
			spinnerRanchor = new Sphere(PxTransform(PxVec3(10.f, 109.5f, -6.f)), 0.1f, 1.0f);
			Add(spinnerRanchor);
			PxRigidDynamic* px_spinnerranchor = (PxRigidDynamic*)spinnerRanchor->Get();
			px_spinnerranchor->setMass(10000.0f);
			PxRigidBody* spinnerranchor_rb = (PxRigidBody*)spinnerRanchor->Get();
			spinnerranchor_rb->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
			spinnerRjoint = new RevoluteJoint(spinnerRanchor, PxTransform(PxVec3(0.f, 0.0f, 0.f), PxQuat(PxPi / 2, PxVec3(0.f, 0.f, 1.f)) * PxQuat(0.0f, -1.0f, 0.0f, -0.113446f)), spinnerR, PxTransform(PxVec3(0.f, 0.0f, 0.0f)));
			spinnerRjoint->DriveVelocity(-8.0f);

			//##########################################################################################  Green Point Scoring Spheres
			bouncer = new Sphere(PxTransform(PxVec3(12.f, 112.5f, -20.f)), 1.0f, 1.0f);
			Add(bouncer);
			PxRigidDynamic* px_bouncer = (PxRigidDynamic*)bouncer->Get();
			px_bouncer->setMass(10000.0f);
			PxRigidBody* bouncer_rb = (PxRigidBody*)bouncer->Get();
			bouncer_rb->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);

			bouncerTrigger = new Sphere(PxTransform(PxVec3(12.f, 112.5f, -20.f)), 1.1f, 1.0f);
			bouncerTrigger->Color(color_palette[3]);
			Add(bouncerTrigger);
			bouncerTrigger->SetTrigger(true);
			bouncerTrigger->SetKinematic(true);

			bouncer2 = new Sphere(PxTransform(PxVec3(-6.f, 101.5f, 24.f)), 1.0f, 1.0f);
			Add(bouncer2);
			PxRigidDynamic* px_bouncer2 = (PxRigidDynamic*)bouncer2->Get();
			px_bouncer2->setMass(10000.0f);
			PxRigidBody* bouncer2_rb = (PxRigidBody*)bouncer2->Get();
			bouncer2_rb->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);

			bouncerTrigger2 = new Sphere(PxTransform(PxVec3(-6.f, 101.5f, 24.f)), 1.1f, 1.0f);
			bouncerTrigger2->Color(color_palette[3]);
			Add(bouncerTrigger2);
			bouncerTrigger2->SetTrigger(true);
			bouncerTrigger2->SetKinematic(true);

			bouncer3 = new Sphere(PxTransform(PxVec3(-2.f, 107.5f, 0.f)), 1.0f, 1.0f);
			Add(bouncer3);
			PxRigidDynamic* px_bouncer3 = (PxRigidDynamic*)bouncer3->Get();
			px_bouncer3->setMass(10000.0f);
			PxRigidBody* bouncer3_rb = (PxRigidBody*)bouncer3->Get();
			bouncer3_rb->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);

			bouncerTrigger3 = new Sphere(PxTransform(PxVec3(-2.f, 107.5f, 0.f)), 1.1f, 1.0f);
			bouncerTrigger3->Color(color_palette[3]);
			Add(bouncerTrigger3);
			bouncerTrigger3->SetTrigger(true);
			bouncerTrigger3->SetKinematic(true);

			bouncer4 = new Sphere(PxTransform(PxVec3(-10.f, 115.5f, -36.f)), 1.0f, 1.0f);
			Add(bouncer4);
			PxRigidDynamic* px_bouncer4 = (PxRigidDynamic*)bouncer4->Get();
			px_bouncer4->setMass(10000.0f);
			PxRigidBody* bouncer4_rb = (PxRigidBody*)bouncer4->Get();
			bouncer4_rb->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);

			bouncerTrigger4 = new Sphere(PxTransform(PxVec3(-10.f, 115.5f, -36.f)), 1.1f, 1.0f);
			bouncerTrigger4->Color(color_palette[3]);
			Add(bouncerTrigger4);
			bouncerTrigger4->SetTrigger(true);
			bouncerTrigger4->SetKinematic(true);

			bouncer5 = new Sphere(PxTransform(PxVec3(6.f, 103.5f, 16.f)), 1.0f, 1.0f);
			Add(bouncer5);
			PxRigidDynamic* px_bouncer5 = (PxRigidDynamic*)bouncer5->Get();
			px_bouncer5->setMass(10000.0f);
			PxRigidBody* bouncer5_rb = (PxRigidBody*)bouncer5->Get();
			bouncer5_rb->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);

			bouncerTrigger5 = new Sphere(PxTransform(PxVec3(6.f, 103.5f, 16.f)), 1.1f, 1.0f);
			bouncerTrigger5->Color(color_palette[3]);
			Add(bouncerTrigger5);
			bouncerTrigger5->SetTrigger(true);
			bouncerTrigger5->SetKinematic(true);
			//#########################################################################################################
		}

		//Custom udpate function
		virtual void CustomUpdate() 
		{
			//((PxCloth*)(cloth->Get()))->setTargetPose(PxTransform(PxVec3(x+=0.1f, 9.f, 0.f)));	     //has inertia
			//((PxCloth*)(cloth->Get()))->setGlobalPose(PxTransform(PxVec3(x += 0.1f, 9.f, 0.f)));		//has no inertia
			//((PxCloth*)(cloth->Get()))->setExternalAcceleration(PxVec3(5.0f, 0.0f, 10.0f));
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

		void ActivatePlunger()//applies force to plunger
		{
			px_plunger->addForce(PxVec3(0.f, 1.f, -1000.f),PxForceMode::eIMPULSE);
		}
		void ActivatePaddle(int i) //applies force to paddle
		{
			if (i == 0)
			{
				px_laddle->addForce(PxVec3(0.f, 1.f, -1000.f), PxForceMode::eIMPULSE);
			}
			else if (i == 1)
			{
				px_raddle->addForce(PxVec3(0.f, 1.f, -1000.f), PxForceMode::eIMPULSE);
			}
			
		}
	};
}
