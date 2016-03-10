#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("jrm2516 - Assignment  06 - LERP"); // Window Name
}

void AppClass::InitVariables(void)
{
	m_pCameraMngr->SetPositionTargetAndView(vector3(0.0f, 0.0f, 15.0f), ZERO_V3, REAXISY);

	// Color of the screen
	m_v4ClearColor = vector4(REBLACK, 1); // Set the clear color to black

	m_pMeshMngr->LoadModel("Sorted\\WallEye.bto", "WallEye");

	fDuration = 1.0f;

	//populate the points vector with all our points that our wall eye will move to
	points.push_back(vector3(-4.0f, -2.0f, 5.0f));
	points.push_back(vector3(1.0f, -2.0f, 5.0f));
	points.push_back(vector3(-3.0f, -1.0f, 3.0f));
	points.push_back(vector3(2.0f, -1.0f, 3.0f));
	points.push_back(vector3(-2.0f, 0.0f, 0.0f));
	points.push_back(vector3(3.0f, 0.0f, 0.0f));
	points.push_back(vector3(-1.0f, 1.0f, -3.0f));
	points.push_back(vector3(4.0f, 1.0f, -3.0f));
	points.push_back(vector3(0.0f, 2.0f, -5.0f));
	points.push_back(vector3(5.0f, 2.0f, -5.0f));
	points.push_back(vector3(1.0f, 3.0f, -5.0f));

	//spheres array to hold all our sphere objects and a m4Points array to hold all our sphere location matricies.
	spheres = new PrimitiveClass[points.size()];
	m4Points = new matrix4[points.size()];

	//populate spheres array with sphere objects then fills the m4Points with matricies that represent the points[]
	for (int i = 0; i < points.size(); i++)
	{
		spheres[i].GenerateSphere(0.1f, 5, RERED);
		m4Points[i] = glm::translate(points[i]);
		
	}

}

void AppClass::Update(void)
{
#pragma region Does not change anything here
	//Update the system's time
	m_pSystem->UpdateTime();

	//Update the mesh manager's time without updating for collision detection
	m_pMeshMngr->Update();
#pragma region

#pragma region Does not need changes but feel free to change anything here
	//Lets us know how much time has passed since the last call
	double fTimeSpan = m_pSystem->LapClock(); //Delta time (between frame calls)

	//cumulative time
	static double fRunTime = 0.0f; //How much time has passed since the program started
	fRunTime += fTimeSpan; 
#pragma endregion

#pragma region Your Code goes here

	//will add dT between updates to duartionSub till it is greater than our lerp duration time, then set it to zero to reset it.
	durationSub += fTimeSpan;
	if (durationSub > fDuration)
	{
		durationSub--;
	}
	//our model starting matrix (doesn't really mater what it is)
	matrix4 modelLocation = IDENTITY_M4;

	//percent is number of duration in total run time
	float percent = (float)fRunTime / fDuration;
	//use this total to get the pointStep, which is how we will decide which points to move to
	int pointSteps = (int)glm::floor(percent) % points.size();

	//now our lerp distance using durationSub and fDuration and casting them to a scale of 0 -> 1
	float lerpAmount = MapValue(durationSub, 0.0f, fDuration, 0.0f, 1.0f);

	//special cast when moving from last point to first point
	if(pointSteps == points.size()-1)
	{ 
		vector3 lerp = glm::lerp(points[pointSteps], points[0], lerpAmount);
		modelLocation = glm::translate(lerp);
	}
	else // else all other points will go from current pointStep to next step in the array.
	{

		vector3 lerp = glm::lerp(points[pointSteps], points[pointSteps + 1], lerpAmount );
		modelLocation = glm::translate(lerp);
	}



	//now pass in our altered matrix from the LERP functions.
	m_pMeshMngr->SetModelMatrix(modelLocation, "WallEye");
#pragma endregion

#pragma region Does not need changes but feel free to change anything here
	//Adds all loaded instance to the render list
	m_pMeshMngr->AddInstanceToRenderList("ALL");

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();

	//Print info on the screen
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);
	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), RERED);
#pragma endregion
}

void AppClass::Display(void)
{
	//clear the screen
	ClearScreen();

	//Render the grid based on the camera's mode:
	switch (m_pCameraMngr->GetCameraMode())
	{
	default: //Perspective
		m_pMeshMngr->AddGridToQueue(1.0f, REAXIS::XY); //renders the XY grid with a 100% scale
		break;
	case CAMERAMODE::CAMROTHOX:
		m_pMeshMngr->AddGridToQueue(1.0f, REAXIS::YZ, RERED * 0.75f); //renders the YZ grid with a 100% scale
		break;
	case CAMERAMODE::CAMROTHOY:
		m_pMeshMngr->AddGridToQueue(1.0f, REAXIS::XZ, REGREEN * 0.75f); //renders the XZ grid with a 100% scale
		break;
	case CAMERAMODE::CAMROTHOZ:
		m_pMeshMngr->AddGridToQueue(1.0f, REAXIS::XY, REBLUE * 0.75f); //renders the XY grid with a 100% scale
		break;
	}

	//same code from E06 to draw all the spheres in the array based of position in spheres array
	matrix4 mProject = m_pCameraMngr->GetProjectionMatrix();
	matrix4 mView = m_pCameraMngr->GetViewMatrix();

	for (int i = 0; i < points.size(); i++)
	{
		//draw spheres where m4Points(the matrix equivlent of points[])
		spheres[i].Render(mProject, mView, m4Points[i]);
	}
	
	m_pMeshMngr->Render(); //renders the render list

	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{

	if (spheres != nullptr)
	{
		delete[] spheres; // release sphere array from memory
		spheres = nullptr;
	}

	if (m4Points != nullptr)
	{
		delete[] m4Points; // now for points
		m4Points = nullptr;
	}

	//the vector list should just fall out of scope, no need to release it

	super::Release(); //release the memory of the inherited fields
}