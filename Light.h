#pragma once
#include "EMath.h"
#include "SDL.h"
#include "Ray.h"

using namespace Elite;

class Light {
public:
	Light(RGBColor color, float intensity);
	~Light() = default;

	Light(const Light& other) = delete;
	Light(Light&& other) = delete;
	Light& operator=(Light&& other) = delete;
	Light& operator=(const Light& other) = delete;


	bool IsLightOn() const {
		return m_IsOn;
	}
	void ChangeLightStatus() {
		if (m_IsOn) {
			m_IsOn = false;
		}
		else {
			m_IsOn = true;
		}
	}

	virtual RGBColor Irradiance(HitRecord& hitrecord) = 0;

	virtual FVector3 GetDirection(const HitRecord& hitRecord) const = 0;
	virtual FVector3 GetTowardPointDirection(const HitRecord& hitRecord) const = 0;

protected:
	const RGBColor m_Color;
	const float m_Intensity;
	bool m_IsOn = true;

};

