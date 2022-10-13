#include "Camera.h"
#include <iostream>

#define scaleFactor 0.02f;

Camera::Camera(const FPoint3 pos, float angleDeg)
	: m_Position{ pos }
	, m_Forward{ FVector3{0.f, 0.f, 1.f} }
	, m_FOV{ ToRadians(angleDeg) }
	, m_WorldUp{ FVector3{0.f, 1.f, 0.f} }
	, m_Velocity{ FVector3{ 100.f, 100.f, -100.f} }
	, m_PitchAngle{ 0.f }
	, m_YawAngle{ 0.f }
	, m_ONB{ FMatrix4::Identity() }
	, m_RotM{ FMatrix4::Identity() }
	, m_ShouldRotate{ false }
{
	CalculateLookAt();
}

float Camera::GetFOV() const {

	return m_FOV;
}

FMatrix4 Camera::GetONB() {

	CalculateLookAt();
	return m_ONB;
}

FPoint3 Camera::GetPos() const {

	return m_Position;
}

void Camera::CalculateLookAt() {

	FVector3 forward{}, right{}, up{};
	if (m_ShouldRotate) {
		forward = Elite::MakeRotationY(m_YawAngle) * m_Forward;
		right = GetNormalized(Cross(m_WorldUp, forward));
		forward = Elite::MakeRotation(m_PitchAngle, right) * forward;
		right = GetNormalized(Cross(m_WorldUp, forward));
		up = GetNormalized(Cross(m_Forward, right));
		m_Forward = forward;
	}
	else {
		right = GetNormalized(Cross(m_WorldUp, m_Forward));
		up = GetNormalized(Cross(m_Forward, right));
	}

	m_ONB[0] = FVector4{ right.x, right.y, right.z, 0.f };
	m_ONB[1] = FVector4{ up.x, up.y, up.z, 0.f };
	m_ONB[2] = FVector4{ m_Forward.x, m_Forward.y, m_Forward.z, 0.f };
	m_ONB[3] = FVector4{ m_Position.x, m_Position.y, m_Position.z, 1.f };
	m_PitchAngle = 0;
	m_YawAngle = 0;
	m_ShouldRotate = false;
}

void Camera::MoveRight(float elapsedSec) {

	m_Position += m_ONB[0].xyz * m_Velocity.y * elapsedSec * scaleFactor;
	m_ONB[3] = FVector3(m_Position.x, m_Position.y, m_Position.z); //update position
}

void Camera::MoveLeft(float elapsedSec) {

	m_Position += -m_ONB[0].xyz * m_Velocity.y * elapsedSec * scaleFactor;
	m_ONB[3] = FVector3(m_Position.x, m_Position.y, m_Position.z); //update position
}

void Camera::MoveForward(float elapsedSec) {

	m_Position += m_Forward * m_Velocity.z * elapsedSec * scaleFactor;
	m_ONB[3].xyz = FVector3(m_Position.x, m_Position.y, m_Position.z); //update position
}

void Camera::MoveBackward(float elapsedSec) {

	m_Position -= m_Forward * m_Velocity.z * elapsedSec * scaleFactor;
	m_ONB[3].xyz = FVector3(m_Position.x, m_Position.y, m_Position.z); //update position
}

void Camera::MoveUp(float elapsedSec) {

	m_Position += m_ONB[1].xyz * m_Velocity.x * elapsedSec * scaleFactor;
	m_ONB[3].xyz = FVector3(m_Position.x, m_Position.y, m_Position.z); //update position
}

void Camera::MoveDown(float elapsedSec) {

	m_Position -= m_ONB[1].xyz * m_Velocity.x * elapsedSec * 0.2f;
	m_ONB[3].xyz = FVector3(m_Position.x, m_Position.y, m_Position.z); //update position
}

void Camera::Pitch(float elapsedSec) {

	const float angle{ static_cast<float>(mouseY) * elapsedSec };
	m_PitchAngle += angle * static_cast<float>(E_TO_RADIANS);
	m_ShouldRotate = true;
}

void Camera::Yaw(float elapsedSec) {

	float angle{ mouseX * elapsedSec };
	m_YawAngle += angle * static_cast<float>(E_TO_RADIANS);
	m_ShouldRotate = true;
}

float Camera::Update(float elapsedSec) {

	Uint32 mouseState{ SDL_GetRelativeMouseState(&mouseX, &mouseY) };
	if (mouseState & SDL_BUTTON_LMASK && mouseState & SDL_BUTTON_RMASK) {
		if (mouseY < 0) {
			MoveUp(elapsedSec);
		}
		else if (mouseY > 0) {
			MoveDown(elapsedSec);
		}
	}
	if (mouseState & SDL_BUTTON_LMASK) {
		if (mouseY < 0) {
			MoveForward(elapsedSec);
		}
		else if (mouseY > 0) {
			MoveBackward(elapsedSec);
		}
	}
	if (mouseState & SDL_BUTTON_RMASK) {
		Yaw(elapsedSec);
		Pitch(elapsedSec);
	}

	const Uint8* keys{ SDL_GetKeyboardState(0) };
	if (keys[SDL_SCANCODE_W]) {
		MoveForward(elapsedSec);
	}
	if (keys[SDL_SCANCODE_S]) {
		MoveBackward(elapsedSec);
	}
	if (keys[SDL_SCANCODE_A]) {
		MoveLeft(elapsedSec);
	}
	if (keys[SDL_SCANCODE_D]) {
		MoveRight(elapsedSec);
	}


	return 0.f;
}