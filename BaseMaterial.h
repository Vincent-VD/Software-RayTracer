#pragma once
#include "EMath.h"
#include "SDL.h"
#include "ERGBColor.h"

using namespace Elite;

struct HitRecord;
class BaseMaterial {
public:
	BaseMaterial() = default;
	BaseMaterial(float diffRefl, RGBColor diffColor);
	~BaseMaterial() = default;

	BaseMaterial(const BaseMaterial& other) = delete;
	BaseMaterial(BaseMaterial&& other) = delete;
	BaseMaterial& operator=(BaseMaterial&& other) = delete;
	BaseMaterial& operator=(const BaseMaterial& other) = delete;

	virtual RGBColor Shade(const HitRecord& hitRecord, const FVector3& w0, const FVector3& w1) const = 0;

protected:
	const float m_DiffuseReflectance;
	const RGBColor m_DiffuseColor;
};

