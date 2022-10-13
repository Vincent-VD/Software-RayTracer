#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(FVector3 direction, RGBColor color, float intensity)
	: Light(color, intensity)
	, m_Direction{ direction }
{
}

RGBColor DirectionalLight::Irradiance(HitRecord& hitrecord) {

	return m_Color * m_Intensity;
}

FVector3 DirectionalLight::GetDirection(const HitRecord& hitRecord) const {

	return m_Direction;
}

FVector3 DirectionalLight::GetTowardPointDirection(const HitRecord& hitRecord) const {


	return m_Direction;
}