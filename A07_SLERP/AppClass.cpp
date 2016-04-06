#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("SLERP - jrm2516"); // Window Name

	//Setting the color to black
	m_v4ClearColor = vector4(0.0f);
}

void AppClass::InitVariables(void)
{
	//Setting the position in which the camera is looking and its interest point
	m_pCameraMngr->SetPositionTargetAndView(vector3(12.12f, 28.52f, 11.34f), ZERO_V3, REAXISY);

	//Setting the color to black
	m_v4ClearColor = vector4(0.0f);

	//Loading the models
	m_pMeshMngr->LoadModel("Planets\\00_Sun.obj", "Sun");
	m_pMeshMngr->LoadModel("Planets\\03_Earth.obj", "Earth");
	m_pMeshMngr->LoadModel("Planets\\03A_Moon.obj", "Moon");

	//Setting the days duration
	m_fDay = 1.0f;
}

void AppClass::Update(void)
{
	//Update the system's time
	m_pSystem->UpdateTime();

	//Update the mesh manager's time without updating for collision detection
	m_pMeshMngr->Update();

	//First person camera movement
	if (m_bFPC == true)
		CameraRotation();

	//Getting the time between calls
	double fCallTime = m_pSystem->LapClock();
	//Counting the cumulative time
	static double fRunTime = 0.0f;
	fRunTime += fCallTime;

	//hud values
	static int nEarthOrbits = 0;
	static int nEarthRevolutions = 0;
	static int nMoonOrbits = 0;

	//Earth Orbit
	double fEarthHalfOrbTime = 182.5f * m_fDay; //Earths orbit around the sun lasts 365 days / half the time for 2 stops
	float fEarthHalfRevTime = 0.5f * m_fDay; // Move for Half a day
	float fMoonHalfOrbTime = 14.0f * m_fDay; //Moon's orbit is 28 earth days, so half the time for half a route

	//this will check to see if a full earth orbit has been done, if so, set it to 0 and add 1 to the HUD
	static float fEarthHalfOrb = 0.0f;
	fEarthHalfOrb += fCallTime;
	if (fEarthHalfOrb > 182.5f * 2)
	{
		fEarthHalfOrb = 0.0f;
		nEarthOrbits++;
	}

	//changing our range from 0 to half an earth orbit to 0 and 1 for the lerp
	float earthOrbPerecent = MapValue((float)fEarthHalfOrb, 0.0f, (float)fEarthHalfOrbTime, 0.0f, 1.0f);	
	//you need the cos and sin to negate how quats are stored, also this uses half angles so have to /2 in the 2nd input of the slerp. Slerp range is from the MapValue function above.
	matrix4 earthM4 = glm::mat4_cast(glm::mix(glm::quat(cos(glm::radians(0.0f)), 0.0f, sin(glm::radians(0.0f)) * 1.0f, 0.0f), glm::quat(cos(glm::radians((float)fEarthHalfOrbTime / 2)), 0.0f, sin(glm::radians((float)fEarthHalfOrbTime / 2.0f)) * 1.0f, 0.0f), earthOrbPerecent));
	//we will then translate after we change the oritation of the earth
	earthM4 = glm::translate(earthM4,vector3(11.0f, 0.0f, 0.0f));

	//same if as above, but this time for the day counter
	static float fEarthDayHalfRev = 0.0f;
	fEarthDayHalfRev += fCallTime;
	if (fEarthDayHalfRev > 1.0f)
	{
		fEarthDayHalfRev = 0.0f;
		nEarthRevolutions++;
	}
	//again, same as above except for the moon orbits
	static float fMoonDayHalfOrb = 0.0f;
	fMoonDayHalfOrb += fCallTime;
	if (fMoonDayHalfOrb > 28.0f)
	{
		fMoonDayHalfOrb = 0.0f;
		nMoonOrbits++;
	}

	//change range from fMoonDay to 0 - 1
	float moonOrbPerecnt = MapValue((float)fMoonDayHalfOrb, 0.0f, (float)fMoonHalfOrbTime, 0.0f, 1.0f);
	
	//will start the moon matrix at the current postion of the earth before earth day rotation
	matrix4 moonM4 = earthM4;
	//will rotate moon based on the MapValue output
	moonM4 = moonM4 * glm::mat4_cast(glm::mix(glm::quat(cos(glm::radians(0.0f)), 0.0f, sin(glm::radians(0.0f)) * 1.0f, 0.0f), glm::quat(cos(glm::radians((float)fEarthHalfOrbTime / 2)), 0.0f, sin(glm::radians((float)fEarthHalfOrbTime / 2.0f)) * 1.0f, 0.0f), moonOrbPerecnt));
	//will translate moon based on scale compared to earth (this is hard coded for now)
	moonM4 = glm::translate(moonM4, vector3(2.0f *0.524f, 0.0f, 0.0f));
	//scale moon to its correct size
	moonM4 = glm::scale(moonM4, vector3(0.524f * 0.27f));

	//map value to figure out rotation percent on current day
	float earthDayPerecnt = MapValue((float)fEarthDayHalfRev, 0.0f, (float)fEarthHalfRevTime, 0.0f, 1.0f);

	//rotate to reflect current earthDayPercent
	matrix4 dayRot = glm::mat4_cast(glm::mix(glm::quat(cos(glm::radians(0.0f)), 0.0f, sin(glm::radians(0.0f)) * 1.0f, 0.0f), glm::quat(cos(glm::radians((float)fEarthHalfOrbTime / 2)), 0.0f, sin(glm::radians((float)fEarthHalfOrbTime / 2.0f)) * 1.0f, 0.0f), earthDayPerecnt));

	//start at current earthPos then rotate based on the day, then scale
	earthM4 = earthM4 * dayRot * glm::scale(IDENTITY_M4,vector3(0.524f));

	//sun at origin, just scale it.
	matrix4 sunM4 = IDENTITY_M4 * glm::scale(vector3(5.936f));
	
	//only problems with this program right now are some rounding errors overtime, could probably 
	//be correct if I set the counters to be equal to totalRunTime ever now and then.
	
	//Setting the matrices
	m_pMeshMngr->SetModelMatrix(sunM4, "Sun");
	m_pMeshMngr->SetModelMatrix(earthM4, "Earth");
	m_pMeshMngr->SetModelMatrix(moonM4, "Moon");

	//Adds all loaded instance to the render list
	m_pMeshMngr->AddInstanceToRenderList("ALL");

	

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();

	//Print info on the screen
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);
	
	m_pMeshMngr->Print("Time:");
	m_pMeshMngr->PrintLine(std::to_string(fRunTime));

	m_pMeshMngr->Print("Day:");
	m_pMeshMngr->PrintLine(std::to_string(m_fDay));

	m_pMeshMngr->Print("E_Orbits:");
	m_pMeshMngr->PrintLine(std::to_string(nEarthOrbits));

	m_pMeshMngr->Print("E_Revolutions:");
	m_pMeshMngr->PrintLine(std::to_string(nEarthRevolutions));

	m_pMeshMngr->Print("M_Orbits:");
	m_pMeshMngr->PrintLine(std::to_string(nMoonOrbits));

	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), RERED);
}

void AppClass::Display(void)
{
	//clear the screen
	ClearScreen();

	//Render the grid based on the camera's mode:
	switch (m_pCameraMngr->GetCameraMode())
	{
	default:
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
	
	m_pMeshMngr->Render(); //renders the render list

	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	super::Release(); //release the memory of the inherited fields
}