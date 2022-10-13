#include "Material_Lambert.h"
#include "BRDF.h"

Material_Lambert::Material_Lambert(float diffRefl, RGBColor diffColor)
	: BaseMaterial(diffRefl, diffColor)
{
}

RGBColor Material_Lambert::Shade(const HitRecord& hitRecord, const FVector3& w0, const FVector3& w1) const {

	return BRDF::Lambert(m_DiffuseReflectance, m_DiffuseColor);
}