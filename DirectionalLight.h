#pragma once
#include "Light.h"


class DirectionalLight : public Light {
public:
	DirectionalLight(FVector3 direction, RGBColor color, float intensity);
	~DirectionalLight() = default;

	DirectionalLight(const DirectionalLight& other) = delete;
	DirectionalLight(DirectionalLight&& other) = delete;
	DirectionalLight& operator=(DirectionalLight&& other) = delete;
	DirectionalLight& operator=(const DirectionalLight& other) = delete;

	virtual RGBColor Irradiance(HitRecord& hitrecord) override;
	
	FVector3 GetDirection(const HitRecord& hitRecord) const;
	FVector3 GetTowardPointDirection(const HitRecord& hitRecord) const;

private:
	const FVector3 m_Direction;
};

