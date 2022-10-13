#pragma once
#include "EMath.h"
#include "SDL.h"

using namespace Elite;

class Camera
{
public:
	Camera(const FPoint3 pos, float angleDeg);
	~Camera() = default;

	Camera(const Camera& other) = delete;
	Camera(Camera&& other) = delete;
	Camera& operator=(Camera&& other) = delete;
	Camera& operator=(const Camera& other) = delete;

	float GetFOV() const;
	FMatrix4 GetONB();
	FPoint3 GetPos() const;

	float Update(float elapsedSec);

private:
	FPoint3 m_Position;
	FVector3 m_Forward;
	FVector3 m_WorldUp;
	FVector3 m_Velocity;
	FMatrix4 m_ONB;
	FMatrix4 m_RotM;
	float m_FOV;
	bool m_ShouldRotate;

	int mouseX{};
	int mouseY{};
	float m_PitchAngle;
	float m_YawAngle; // x-axis, y-axis (resp.)

	void CalculateLookAt();
	void MoveRight(float elapsedSec);
	void MoveLeft(float elapsedSec);
	void MoveForward(float elapsedSec);
	void MoveBackward(float elapsedSec);
	void MoveUp(float elapsedSec);
	void MoveDown(float elapsedSec);
	void Pitch(float elapsedSec);
	void Yaw(float elapsedSec);
};

