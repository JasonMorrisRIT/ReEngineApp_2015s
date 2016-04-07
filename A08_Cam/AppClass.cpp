#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("A08 | Jason Morris | Alex Bronner"); // Window Name
}
void AppClass::InitVariables(void)
{
	//Generate the Cone
	m_pCone = new PrimitiveClass();
	m_pCone->GenerateCone(1.0f, 2.0f, 10, RERED);

	//Generate the Cylinder
	m_pCylinder = new PrimitiveClass();
	m_pCylinder->GenerateCylinder(1.0f, 2.0f, 10, REGREEN);

	cam->SetPosition(vector3(0.0f, 4.0f, -4.0f));
	cam->SetTarget(vector3(0.0f, 0.0f, 0.0f));
	cam->SetUp(vector3(0.0f, 1.0f, 0.0f));

	//Calculate the first projections
	m_m4Projection = cam->GetProjection(false);
	m_m4View = cam->GetView();
}

void AppClass::Update(void)
{
	//Update the system's time
	m_pSystem->UpdateTime();

	//Update the mesh manager's time without updating for collision detection
	m_pMeshMngr->Update();

	//Stuff
	m_m4Projection = cam->GetProjection(false);
	m_m4View = cam->GetView();

	//Adds all loaded instance to the render list
	m_pMeshMngr->AddInstanceToRenderList("ALL");
}

void AppClass::Display(void)
{
	//Clear the window
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Render the grid
	//m_pMeshMngr->AddGridToQueue(1.0f, REAXIS::XY);

	//Render the cone
	m_pCone->Render(m_m4Projection, m_m4View, IDENTITY_M4);

	//Render the cylinder
	m_pCylinder->Render(m_m4Projection, m_m4View, glm::translate(IDENTITY_M4, REAXISZ * -2.0f));

	//Render the rest of the meshes
	m_pMeshMngr->Render();

	//Swaps the OpenGL buffers
	m_pGLSystem->GLSwapBuffers();
}

void AppClass::Release(void)
{
	//Release the memory of the member fields
	SafeDelete(m_pCone);
	SafeDelete(m_pCylinder);

	//Release the memory of the inherited fields
	super::Release(); 
}