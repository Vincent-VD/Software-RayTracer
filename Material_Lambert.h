#pragma once
#include "BaseMaterial.h"

class Material_Lambert : public BaseMaterial {
public:
	Material_Lambert(float diffRefl, RGBColor diffColor);
	~Material_Lambert() = default;

	Material_Lambert(const Material_Lambert& other) = delete;
	Material_Lambert(Material_Lambert&& other) = delete;
	Material_Lambert& operator=(Material_Lambert&& other) = delete;
	Material_Lambert& operator=(const Material_Lambert& other) = delete;
	

	virtual RGBColor Shade(const HitRecord& hitRecord, const FVector3& w0, const FVector3& w1) const override;

};

