#include "MyPrimitive.h"
MyPrimitive::MyPrimitive() { }
MyPrimitive::MyPrimitive(const MyPrimitive& other) { }
MyPrimitive& MyPrimitive::operator=(const MyPrimitive& other) { return *this; }
MyPrimitive::~MyPrimitive(void) { super::Release(); }
void MyPrimitive::CompileObject(vector3 a_v3Color)
{
	m_uVertexCount = static_cast<int> (m_lVertexPos.size());
	for (uint i = 0; i < m_uVertexCount; i++)
	{
		AddVertexColor(a_v3Color);
	}
	
	CompleteTriangleInfo(true);
	CompileOpenGL3X();

}
//C--D
//|\ |
//| \|
//A--B
//This will make the triang A->B->C and then the triang C->B->D
void MyPrimitive::AddQuad(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft, vector3 a_vTopRight)
{
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopLeft);

	AddVertexPosition(a_vTopLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopRight);
}
void MyPrimitive::GeneratePlane(float a_fSize, vector3 a_v3Color)
{
	if (a_fSize < 0.01f)
		a_fSize = 0.01f;

	Release();
	Init();

	float fValue = 0.5f * a_fSize;

	vector3 pointA(-fValue, -fValue, 0.0f); //0
	vector3 pointB(fValue, -fValue, 0.0f); //1
	vector3 pointC(fValue, fValue, 0.0f); //2
	vector3 pointD(-fValue, fValue, 0.0f); //3

	vector3 pointE(fValue, -fValue, -0.001f); //1
	vector3 pointF(-fValue, -fValue, -0.001f); //0
	vector3 pointG(fValue, fValue, -0.001f); //2
	vector3 pointH(-fValue, fValue, -0.001f); //3

											  //F
	AddQuad(pointA, pointB, pointD, pointC);
	//Double sided
	AddQuad(pointE, pointF, pointG, pointH);

	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateCube(float a_fSize, vector3 a_v3Color)
{
	if (a_fSize < 0.01f)
		a_fSize = 0.01f;

	Release();
	Init();

	float fValue = 0.5f * a_fSize;
	//3--2
	//|  |
	//0--1
	vector3 point0(-fValue, -fValue, fValue); //0
	vector3 point1(fValue, -fValue, fValue); //1
	vector3 point2(fValue, fValue, fValue); //2
	vector3 point3(-fValue, fValue, fValue); //3

	vector3 point4(-fValue, -fValue, -fValue); //4
	vector3 point5(fValue, -fValue, -fValue); //5
	vector3 point6(fValue, fValue, -fValue); //6
	vector3 point7(-fValue, fValue, -fValue); //7

											  //F
	AddQuad(point0, point1, point3, point2);

	//B
	AddQuad(point5, point4, point6, point7);

	//L
	AddQuad(point4, point0, point7, point3);

	//R
	AddQuad(point1, point5, point2, point6);

	//U
	AddQuad(point3, point2, point7, point6);

	//D
	AddQuad(point4, point5, point0, point1);

	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateCone(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	//Your code starts here

	float centralAngle = 360 / a_nSubdivisions; //angle from point to point is == 360 deg / number of sides

	std::vector<vector3> points;
	float halfHeight = a_fHeight / 2;
	vector3 top = vector3(0, halfHeight, 0);

	for (int i = 0; i < a_nSubdivisions; i++)
	{
		//vector3 temp = vector3(0, -halfHeight, 0);
		//temp = temp + vector3(a_fRadius, 0, 0);
		//temp = temp * matrix3[glm::cos(centralAngle*i), 0, -glm::sin(centralAngle*i), 0, 1, 0, glm::sin(centralAngle*i), 0, cos(centralAngle*i)];

		vector3 temp = vector3(a_fRadius, -halfHeight, 0);
		temp = glm::mat3(glm::cos(centralAngle*i), 0, -glm::sin(centralAngle*i), 0, 1, 0, glm::sin(centralAngle*i), 0, cos(centralAngle*i)) * temp;
		points.push_back(temp);
		std::cout << "| point: " << i << " is: " << temp.x << " , " << temp.y << " , " << temp.z;

	}

	for (int i = 0; i < a_nSubdivisions - 3; i++)
	{
		AddQuad(points[0 + i], points[1 + i], points[3+i], points[2 + i]);
		AddQuad(points[0 + i], points[1 + i], top, points[2 + i]);
	}

		//AddQuad(points[0], points[1], top, points[0]);

	
	//3--2
	//|  |
	//0--1
	//vector3 point0(-fValue, -fValue, fValue); //0
	//vector3 point1(fValue, -fValue, fValue); //1
	//vector3 point2(fValue, fValue, fValue); //2
	//vector3 point3(-fValue, fValue, fValue); //3



	//AddQuad(point0, point1, point3, point2);

	//Your code ends here
	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateCylinder(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	float centralAngle = 360 / a_nSubdivisions;
	std::vector<vector3> topPoints;
	std::vector<vector3> bottomPoints;
	float halfHeight = a_fHeight / 2;

	for (int i = 0; i < a_nSubdivisions; i++)
	{
		//vector3 temp = vector3(0, -halfHeight, 0);
		//temp = temp + vector3(a_fRadius, 0, 0);
		//temp = temp * matrix3[glm::cos(centralAngle*i), 0, -glm::sin(centralAngle*i), 0, 1, 0, glm::sin(centralAngle*i), 0, cos(centralAngle*i)];

		vector3 tempBottom = vector3(a_fRadius, -halfHeight, 0);
		vector3 tempTop = vector3(a_fRadius, halfHeight, 0);
		tempBottom = glm::mat3(glm::cos(centralAngle*i), 0, -glm::sin(centralAngle*i), 0, 1, 0, glm::sin(centralAngle*i), 0, cos(centralAngle*i)) * tempBottom;
		tempTop = glm::mat3(glm::cos(centralAngle*i), 0, -glm::sin(centralAngle*i), 0, 1, 0, glm::sin(centralAngle*i), 0, cos(centralAngle*i)) * tempTop;
		bottomPoints.push_back(tempBottom);
		topPoints.push_back(tempTop);
		//std::cout << "| point: " << i << " is: " << temp.x << " , " << temp.y << " , " << temp.z;

	}

	for (int i = 0; i < a_nSubdivisions - 1; i++)
	{
		AddQuad(bottomPoints[0 + i], bottomPoints[1 + i], topPoints[0 + i], topPoints[1 + i]);
		AddQuad(bottomPoints[1 + i], bottomPoints[0 + i], topPoints[1 + i], topPoints[0 + i]);
		AddQuad(topPoints[i], topPoints[1 + i], topPoints[topPoints.size() -1 - i], topPoints[topPoints.size() - 2 - i]);
		AddQuad(bottomPoints[i + 1], bottomPoints[i], bottomPoints[bottomPoints.size() - 2 - i], bottomPoints[bottomPoints.size() - 1 - i]);
		
	}

	//Your code starts here
	float fValue = 0.5f;
	//3--2
	//|  |
	//0--1
	//vector3 point0(-fValue, -fValue, fValue); //0
	//vector3 point1(fValue, -fValue, fValue); //1
	//vector3 point2(fValue, fValue, fValue); //2
	//vector3 point3(-fValue, fValue, fValue); //3

	//AddQuad(point0, point1, point3, point2);

	//Your code ends here
	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateTube(float a_fOuterRadius, float a_fInnerRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	float centralAngle = 360 / a_nSubdivisions;
	std::vector<vector3> topPointsOut;
	std::vector<vector3> bottomPointsOut;
	std::vector<vector3> topPointsIn;
	std::vector<vector3> bottomPointsIn;
	float halfHeight = a_fHeight / 2;

	for (int i = 0; i < a_nSubdivisions; i++)
	{
		//vector3 temp = vector3(0, -halfHeight, 0);
		//temp = temp + vector3(a_fRadius, 0, 0);
		//temp = temp * matrix3[glm::cos(centralAngle*i), 0, -glm::sin(centralAngle*i), 0, 1, 0, glm::sin(centralAngle*i), 0, cos(centralAngle*i)];

		vector3 tempBottomOut = vector3(a_fOuterRadius, -halfHeight, 0);
		vector3 tempTopOut = vector3(a_fOuterRadius, halfHeight, 0);
		tempBottomOut = glm::mat3(glm::cos(centralAngle*i), 0, -glm::sin(centralAngle*i), 0, 1, 0, glm::sin(centralAngle*i), 0, cos(centralAngle*i)) * tempBottomOut;
		tempTopOut = glm::mat3(glm::cos(centralAngle*i), 0, -glm::sin(centralAngle*i), 0, 1, 0, glm::sin(centralAngle*i), 0, cos(centralAngle*i)) * tempTopOut;
		bottomPointsOut.push_back(tempBottomOut);
		topPointsOut.push_back(tempTopOut);

		vector3 tempBottomIn = vector3(a_fInnerRadius, -halfHeight, 0);
		vector3 tempTopIn = vector3(a_fInnerRadius, halfHeight, 0);
		tempBottomIn = glm::mat3(glm::cos(centralAngle*i), 0, -glm::sin(centralAngle*i), 0, 1, 0, glm::sin(centralAngle*i), 0, cos(centralAngle*i)) * tempBottomIn;
		tempTopIn = glm::mat3(glm::cos(centralAngle*i), 0, -glm::sin(centralAngle*i), 0, 1, 0, glm::sin(centralAngle*i), 0, cos(centralAngle*i)) * tempTopIn;
		bottomPointsIn.push_back(tempBottomIn);
		topPointsIn.push_back(tempTopIn);
		//std::cout << "| point: " << i << " is: " << temp.x << " , " << temp.y << " , " << temp.z;

	}

	//Your code starts here
	float fValue = 0.5f;
	//3--2
	//|  |
	//0--1
	//vector3 point0(-fValue, -fValue, fValue); //0
	//vector3 point1(fValue, -fValue, fValue); //1
	//vector3 point2(fValue, fValue, fValue); //2
	//vector3 point3(-fValue, fValue, fValue); //3

	//AddQuad(point0, point1, point3, point2);

	for (int i = 0; i < a_nSubdivisions - 1 ; i++)
	{
		AddQuad(bottomPointsOut[0 + i], bottomPointsOut[1 + i], topPointsOut[0 + i], topPointsOut[1 + i]);
		//AddQuad(bottomPointsIn[0 + i], bottomPointsIn[1 + i], topPointsIn[0 + i], topPointsIn[1 + i]);
		AddQuad(bottomPointsIn[1 + i], bottomPointsIn[0 + i], topPointsIn[1 + i], topPointsIn[0 + i]);
		AddQuad(bottomPointsIn[0 + i], bottomPointsOut[0 + i], topPointsIn[0 + i], topPointsOut[0 + i]);
		AddQuad(bottomPointsIn[0 + i], bottomPointsIn[1 + i], bottomPointsOut[0 + i], bottomPointsOut[1 + i]);
		AddQuad(topPointsIn[1 + i], topPointsIn[0 + i], topPointsOut[1 + i], topPointsOut[0 + i]);
		
		
	}

	//Your code ends here
	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateTorus(float a_fOuterRadius, float a_fInnerRadius, int a_nSubdivisionsA, int a_nSubdivisionsB, vector3 a_v3Color)
{
	if (a_fOuterRadius <= a_fInnerRadius + 0.1f)
		return;

	if (a_nSubdivisionsA < 3)
		a_nSubdivisionsA = 3;
	if (a_nSubdivisionsA > 25)
		a_nSubdivisionsA = 25;

	if (a_nSubdivisionsB < 3)
		a_nSubdivisionsB = 3;
	if (a_nSubdivisionsB > 25)
		a_nSubdivisionsB = 25;

	Release();
	Init();

	//Your code starts here
	float fValue = 0.5f;
	//3--2
	//|  |
	//0--1
	vector3 point0(-fValue, -fValue, fValue); //0
	vector3 point1(fValue, -fValue, fValue); //1
	vector3 point2(fValue, fValue, fValue); //2
	vector3 point3(-fValue, fValue, fValue); //3

	AddQuad(point0, point1, point3, point2);

	//Your code ends here
	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateSphere(float a_fRadius, int a_nSubdivisions, vector3 a_v3Color)
{
	//Sets minimum and maximum of subdivisions
	if (a_nSubdivisions < 1)
	{
		GenerateCube(a_fRadius * 2, a_v3Color);
		return;
	}
	if (a_nSubdivisions > 6)
		a_nSubdivisions = 6;

	Release();
	Init();

	//Your code starts here
	float fValue = 0.5f;
	//3--2
	//|  |
	//0--1
	vector3 point0(-fValue, -fValue, fValue); //0
	vector3 point1(fValue, -fValue, fValue); //1
	vector3 point2(fValue, fValue, fValue); //2
	vector3 point3(-fValue, fValue, fValue); //3

	AddQuad(point0, point1, point3, point2);

	//Your code ends here
	CompileObject(a_v3Color);
}