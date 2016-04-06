#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("Sandbox"); // Window Name

	// Set the clear color based on Microsoft's CornflowerBlue (default in XNA)
	//if this line is in Init Application it will depend on the .cfg file, if it
	//is on the InitVariables it will always force it regardless of the .cfg
	m_v4ClearColor = vector4(0.4f, 0.6f, 0.9f, 0.0f);
}

void AppClass::InitVariables(void)
{
<<<<<<< HEAD
	//m_selection = std::pair<int, int>(-1, -1);
	//Set the camera at a position other than the default
	m_pCameraMngr->SetPositionTargetAndView(vector3(0.0f, 2.5f, 12.0f), vector3(0.0f, 2.5f, 11.0f), REAXISY);

	//m_pLightMngr->SetColor(REWHITE, 0);
	//m_pLightMngr->SetIntensity(0.1f, 0);
	//m_pLightMngr->SetColor(REWHITE, 1);
	//m_pLightMngr->SetIntensity(0.5f, 1);
//	m_pLightMngr->SetPosition(vector3(0.0f, 1.0f,-1.0f), 1);

	//Load a model onto the Mesh manager
	//m_pMeshMngr->LoadModel("tests\\Cubev.fbx", "Unikitty");
	//int nCubes = 10;
	//vector3 v3Start(-nCubes/2.0f, 0.0f, -nCubes / 2.0f);
	m_pMeshMngr->LoadModel("Minecraft\\Steve.obj", "Steve");
//	m_pMeshMngr->LoadModel("Minecraft\\Creeper.obj", "Creep");

	//m_pMeshMngr->SetShaderProgramByName("ElCubo", "Phong");
	//for (uint n = 0; n < nCubes; n++)
	//{
	//	if (v3Start != vector3(0.0f))
	//	{
		//	String sName = "Cube_" + std::to_string(n);
		//	m_pMeshMngr->LoadModel("Cube.obj", sName, false, glm::translate(v3Start));
		//	m_pMeshMngr->SetShaderProgramByName(sName, "Phong");
		//}
		//v3Start += vector3(1.0f, 0.0f, 1.0f);
	//}
=======
	//Reset the selection to -1, -1
	m_selection = std::pair<int, int>(-1, -1);
	//Set the camera position
	m_pCameraMngr->SetPositionTargetAndView(
		vector3(0.0f, 2.5f, 15.0f),//Camera position
		vector3(0.0f, 2.5f, 0.0f),//What Im looking at
		REAXISY);//What is up
	//Load a model onto the Mesh manager
	m_pMeshMngr->LoadModel("Lego\\Unikitty.bto", "Unikitty");
>>>>>>> 00eb2ab2dfb674c6ae171360e8b5714a2552d94c
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

	//Call the arcball method
	ArcBall();
<<<<<<< HEAD

	//matrix4 m4Temp = IDENTITY_M4 * glm::translate(vector3(4.0f,2.0f,1.0f));

	////m4Temp = m4Temp * glm::translate(vector3(-4.0f, -2.0f, -1.0f));
	//matrix3 m3Temp = matrix3(m4Temp);
	//m3Temp = glm::transpose(m3Temp);
	//
	//m4Temp = m4Temp * matrix4(m3Temp);

	//m4Temp[3] = m4Temp[3] + (m4Temp[3] * -1);
	//m4Temp[3][3] = 1;
	////m4Temp = m4Temp * glm::inverse(m4Temp);

	vector3 v3Start = vector3((-5.0f, 0.0f, 0.0f));
	vector3 v3End = vector3((5.0f, 0.0f, 0.0f));
	static float percent = 0.0f;

	vector3 v3Interpolation = glm::lerp(v3Start, v3End, percent);

	percent += 0.1f;
	matrix4 m4Temp =  glm::translate(v3Interpolation);

	m_pMeshMngr->SetModelMatrix(m4Temp, "Steve");

=======
	
	//Set the model matrix for the first model to be the arcball
	m_pMeshMngr->SetModelMatrix(ToMatrix4(m_qArcBall), 0);
>>>>>>> 00eb2ab2dfb674c6ae171360e8b5714a2552d94c
	
	//Adds all loaded instance to the render list
	m_pMeshMngr->AddInstanceToRenderList("ALL");

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();
	//print info into the console
	//printf("FPS: %d            \r", nFPS);//print the Frames per Second
	//Print info on the screen
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);

	m_pMeshMngr->Print("Selection: ");
	m_pMeshMngr->PrintLine(m_pMeshMngr->GetInstanceGroupName(m_selection.first, m_selection.second), REYELLOW);
	
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
	
	m_pMeshMngr->Render(); //renders the render list

	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	super::Release(); //release the memory of the inherited fields
}