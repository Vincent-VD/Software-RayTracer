#pragma once
#include "EMath.h"
#include "ERGBColor.h"
#include "Ray.h"

using namespace Elite;

namespace BRDF {

	RGBColor Lambert(const float diffRefl, const RGBColor diffColor);
	RGBColor Lambert(const RGBColor diffColor, const RGBColor diffRefl);
	
	RGBColor Phong(const float specRefl, const float exp, const FVector3& w0, const FVector3& w1, const FVector3& normal);

	RGBColor CookTerrance(const float d, const RGBColor f, const float g, const FVector3& l, const FVector3& v, const FVector3& normal);

}