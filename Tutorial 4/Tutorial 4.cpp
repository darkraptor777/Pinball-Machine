#include <iostream>
#include "VisualDebugger.h"

//SUMMARY
/*Implemented functionality includes; two paddles / flippers which use revolute joints; a plunger which uses a distance joint; 
a to - scale pinball machine based on online specifications; a pinball with accurate massand size based on specifications found online;
rotating cuboids using motorsand revolute joints; green trigger spheres which increase the players score; purple simulated cloths; pinball machine
frame / body is a compound shape; various objects are custom convex shapes such as the paddles.Original developments include; Corner class; Guide class;
paddle class; PinballMachine class; ramp class; scoring mechanisms; pinball machine controls(and relevant functions); all ‘anchors’(spheres used by 
corresponding moving objects / actors); all moving objects / actors;and the colours(hopefully they look nice ? ).*/

//Sources:
//Flippers(undated) How to level a pinball machine.Available from https ://www.flippers.be/basics/101_level_pinball_machine.html [accessed 29 April 2020]
//Dimensions(2020) Pinball Machines.Available from https ://www.dimensions.guide/element/pinball-machines [accessed 29 April 2020]
//How Products are Made(undated) Pinball.Available from https ://electronics.howstuffworks.com/pinball-machine1.html [accessed 29 April 2020]
//Howstuffworks(undated) How Pinball Machines Work.Available from http ://www.madehow.com/Volume-6/Pinball.html [accessed 29 April 2020]
//The Engineering ToolBox(2004) Frictionand Friction Coefficients.Available from https ://www.engineeringtoolbox.com/friction-coefficients-d_778.html [accessed 29 April 2020]
//The Physics Factbook(2005) Coefficients of Restitution.Available from  https ://hypertextbook.com/facts/2006/restitution.shtml [accessed 29 April 2020]



using namespace std;

int main()
{
	try 
	{ 
		VisualDebugger::Init("Pinball", 800, 800); 
	}
	catch (Exception exc) 
	{ 
		cerr << exc.what() << endl;
		return 0; 
	}

	VisualDebugger::Start();

	return 0;
}