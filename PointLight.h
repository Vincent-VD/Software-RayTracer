#pragma once
#include "Light.h"

class PointLight : public Light {
public:
	PointLight(FPoint3 position, RGBColor color, float intensity);
	~PointLight() = default;

	PointLight(const PointLight& other) = delete;
	PointLight(PointLight&& other) = delete;
	PointLight& operator=(PointLight&& other) = delete;
	PointLight& operator=(const PointLight& other) = delete;

	virtual RGBColor Irradiance(HitRecord& hitrecord) override;

	FVector3 GetDirection(const HitRecord& hitRecord) const;
	FVector3 GetTowardPointDirection(const HitRecord& hitRecord) const;

private:
	const FPoint3 m_Position;

};

