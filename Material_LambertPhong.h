#pragma once
#include "BaseMaterial.h"

class Material_LambertPhong : public BaseMaterial {
public:
	Material_LambertPhong(float diffRefl, RGBColor diffColor, float specRefl, float e);
	~Material_LambertPhong() = default;

	Material_LambertPhong(const Material_LambertPhong& other) = delete;
	Material_LambertPhong(Material_LambertPhong& other) = delete;
	Material_LambertPhong operator=(const Material_LambertPhong& other) = delete;
	Material_LambertPhong operator=(Material_LambertPhong& other) = delete;

	virtual RGBColor Shade(const HitRecord& hitRecord, const FVector3& w0, const FVector3& w1) const override;

private:
	const float m_SpecularReflectance;
	const float m_PhongExp;

};

