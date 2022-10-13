#include "PointLight.h"
#include "EMath.h"

PointLight::PointLight(FPoint3 position, RGBColor color, float intensity)
	: Light(color, intensity)
	, m_Position(position)
{
}

RGBColor PointLight::Irradiance(HitRecord& hitrecord) {
	float mgn{};
	mgn = Elite::SqrDistance(m_Position, hitrecord.hitPoint);
	RGBColor res{};
	res = m_Color * (m_Intensity / mgn);

	return res;
}

FVector3 PointLight::GetDirection(const HitRecord& hitRecord) const {

	return m_Position - hitRecord.hitPoint;
}

FVector3 PointLight::GetTowardPointDirection(const HitRecord& hitRecord) const {

	return hitRecord.hitPoint - m_Position;
}