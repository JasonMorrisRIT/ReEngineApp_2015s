#include "AppClass.h"
void AppClass::ProcessKeyboard(void)
{
	bool bModifier = false;
	float fSpeed = 0.01f;

#pragma region ON PRESS/RELEASE DEFINITION
	static bool	bLastF1 = false, bLastF2 = false, bLastF3 = false, bLastF4 = false, bLastF5 = false,
				bLastF6 = false, bLastF7 = false, bLastF8 = false, bLastF9 = false, bLastF10 = false,
				bLastEscape = false;
#define ON_KEY_PRESS_RELEASE(key, pressed_action, released_action){  \
			bool pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::key);			\
			if(pressed){											\
				if(!bLast##key) pressed_action;}/*Just pressed? */\
			else if(bLast##key) released_action;/*Just released?*/\
			bLast##key = pressed; } //remember the state
#pragma endregion

#pragma region Modifiers
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
		bModifier = true;
#pragma endregion

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
	{
		m_v3Orientation = vector3(0.0f);
		//should reset steve, but makes him face the wrong way
		tempOrientation = glm::quat(0.0f, vector3(0.0f,1.0f,0.0f));
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
	{
		//rellay small number due to continueness add operations
		if (!bModifier) m_v3Orientation.x += 0.01f;
		else m_v3Orientation.x -= 0.01f;

		
		//adds current orientation quat to the new changed orientation quat that gets increased by the floats in the m_v3Orientation vector
		if (!bModifier) tempOrientation = tempOrientation * glm::quat(cos(glm::radians(m_v3Orientation.x)), sin(glm::radians(m_v3Orientation.x)) * 1.0f, 0.0f, 0.0f) ;
		else tempOrientation = tempOrientation * glm::quat(cos(glm::radians(m_v3Orientation.x)), sin(glm::radians(m_v3Orientation.x)) * 1.0f, 0.0f, 0.0f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y))
	{
		if (!bModifier) m_v3Orientation.y += 0.01f;
		else m_v3Orientation.y -= 0.01f;

		if (!bModifier) tempOrientation = tempOrientation *  glm::quat(cos(glm::radians(m_v3Orientation.y)), 0.0f, sin(glm::radians(m_v3Orientation.y)) * 1.0f, 0.0f);
		else tempOrientation = tempOrientation * glm::quat(cos(glm::radians(m_v3Orientation.y)), 0.0f, sin(glm::radians(m_v3Orientation.y)) * 1.0f, 0.0f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
	{
		if (!bModifier) m_v3Orientation.z += 0.01f;
		else m_v3Orientation.z -= 0.01f;

		if (!bModifier) tempOrientation = tempOrientation * glm::quat(cos(glm::radians(m_v3Orientation.z)), 0.0f, 0.0f , sin(glm::radians(m_v3Orientation.z)) * 1.0f);
		else tempOrientation = tempOrientation * glm::quat(cos(glm::radians(m_v3Orientation.z)), 0.0f, 0.0f, sin(glm::radians(m_v3Orientation.z)) * 1.0f);
	}

#pragma region Camera Positioning
	if(bModifier)
		fSpeed *= 10.0f;
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		m_pCameraMngr->MoveForward(fSpeed);

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		m_pCameraMngr->MoveForward(-fSpeed);
	
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		m_pCameraMngr->MoveSideways(-fSpeed);

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		m_pCameraMngr->MoveSideways(fSpeed);
	m_pCameraMngr->CalculateView();
#pragma endregion

#pragma region Other Actions
	ON_KEY_PRESS_RELEASE(Escape, NULL, PostMessage(m_pWindow->GetHandler(), WM_QUIT, NULL, NULL))
#pragma endregion
}
void AppClass::ProcessMouse(void)
{
	m_bArcBall = false;
	m_bFPC = false;
	
	if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Middle))
		m_bArcBall = true;
	
	if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
		m_bFPC = true;
}
