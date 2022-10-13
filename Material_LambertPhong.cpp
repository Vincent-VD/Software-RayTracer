#include "Material_LambertPhong.h"
#include "Ray.h"
#include "BRDF.h"


Material_LambertPhong::Material_LambertPhong(float diffRefl, RGBColor diffColor, float specRefl, float e)
	: BaseMaterial{ diffRefl, diffColor }
	, m_SpecularReflectance{ specRefl }
	, m_PhongExp{ e }
{
}

RGBColor Material_LambertPhong::Shade(const HitRecord& hitRecord, const FVector3& w0, const FVector3& w1) const {

	return BRDF::Lambert(RGBColor{ m_DiffuseReflectance, m_DiffuseReflectance, m_DiffuseReflectance }, m_DiffuseColor) +
	//return BRDF::Lambert(m_DiffuseReflectance, m_DiffuseColor) +
		BRDF::Phong(m_SpecularReflectance, m_PhongExp, w0, w1, hitRecord.normal);

}