#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	//Using Base InitWindow method
	super::InitWindow("Instance Rendering jrm2516");
}

void AppClass::InitVariables(void)
{
	m_pCameraMngr->SetPositionTargetAndView(vector3(0.0f, 0.0f, 15.0f), vector3(0.0f, 0.0f, 0.0f), REAXISY);

	m_pMesh = new MyMesh();
	
	//Creating the Mesh points
	m_pMesh->AddVertexPosition(vector3(-1.0f, -1.0f, 0.0f));
	m_pMesh->AddVertexColor(RERED);
	m_pMesh->AddVertexPosition(vector3( 1.0f, -1.0f, 0.0f));
	m_pMesh->AddVertexColor(REGREEN);
	m_pMesh->AddVertexPosition(vector3(0.0f,  1.0f, 0.0f));
	m_pMesh->AddVertexColor(REBLUE);
	//m_pMesh->AddVertexPosition(vector3(-1.0f,  1.0f, 0.0f));
	//m_pMesh->AddVertexColor(REBLUE);
	//m_pMesh->AddVertexPosition(vector3(1.0f, -1.0f, 0.0f));
	//m_pMesh->AddVertexColor(REBLUE);
	//m_pMesh->AddVertexPosition(vector3( 1.0f, 1.0f, 0.0f));
	//m_pMesh->AddVertexColor(REBLUE);

	//Compiling the mesh
	m_pMesh->CompileOpenGL3X();

	//Initializing the array
	//m_nObjects = 100;
	
	// We need 16 floats for each object (each matrix has 16 floats)
	m_fMatrixArray = new float[m_nObjects * 16];
	//Initializing the whole spaces to the position at the origin just to play it safe

	int iterationCount = 8;			//Number of Sierpinski iterations
	std::vector<vector3> tris;		//Triangles
	std::vector<int> iteration;		//Values from the current iteration
	iteration.push_back(1);			//Starting iteration

	//For each iteration
	for (int a = 0; a < iterationCount; a++)
	{
		//For each value in the current iteration
		for (int b = 0; b < iteration.size(); b++)
		{
			//If the value is odd, ad a triangle at its position
			if(iteration[b] % 2 != 0)
				tris.push_back(vector3(2 * b - a, -2 * a, 0.0f));
		}
		iteration = GetSierpinskiIteration(iteration);	//Get next iteration
	}
	
	m_nObjects = tris.size();
	
	for (uint n = 0; n < m_nObjects; n++)
	{
		const float* m4MVP = glm::value_ptr(glm::translate(tris[n]));
		memcpy(&m_fMatrixArray[n * 16], m4MVP, 16 * sizeof(float));
	}
}

std::vector<int> AppClass::GetSierpinskiIteration(std::vector<int> iteration)
{
	int length = iteration.size();
	std::vector<int> iterationNew;	//New iteration
	iterationNew.push_back(1);		//First value

	//Between values
	for (uint i = 0; i < length - 1; i++)
	{
		iterationNew.push_back(iteration[i] + iteration[i + 1]);
	}

	iterationNew.push_back(iteration[length - 1]);	//Last value
	
	//Print iteration
	for (uint i = 0; i < iterationNew.size(); i++)
	{
		std::cout << iterationNew[i] << " ";
	}
	std::cout << std::endl;

	return iterationNew;
}

void AppClass::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->UpdateTime();

	//Is the arcball active?
	if (m_bArcBall == true)
		ArcBall();

	//Is the first person camera active?
	if (m_bFPC == true)
		CameraRotation();

	//Calculate Camera
	m_pCameraMngr->CalculateView();

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();
	//print info into the console
	printf("FPS: %d            \r", nFPS);//print the Frames per Second
										  //Print info on the screen
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);
	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), RERED);
}

void AppClass::Display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the window
	
	//Matrices from the camera
	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix();
	matrix4 m4View = m_pCameraMngr->GetViewMatrix();

	//m_pMesh->Render(m4Projection, m4View, IDENTITY_M4);//Rendering nObjects

	for (uint i = 0; i < m_nObjects; i++)
	{
		m_pMesh->RenderList(m4Projection, m4View, m_fMatrixArray, m_nObjects);

	}



	m_pMeshMngr->Render();

	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	SafeDelete(m_fMatrixArray);
	if (m_pMesh != nullptr)
	{
		delete m_pMesh;
		m_pMesh = nullptr;
	}
	super::Release();
}