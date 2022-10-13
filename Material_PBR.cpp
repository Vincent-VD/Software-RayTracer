#include "Material_PBR.h"
#include "BRDF.h"

Material_PBR::Material_PBR(	const float diffRefl, 
							const RGBColor color, 
							const float specRefl, 
							const RGBColor albedo, 
							const bool isMetallic, 
							const float roughness, 
							const bool isDirectLighting)
	: BaseMaterial(diffRefl, color)
	, m_SpecRefl{ specRefl }
	, m_AlbedoColor{ albedo }
	, m_IsMetallic{ isMetallic }
	, m_Roughness{ roughness }
	, m_IsDirectLighting{ isDirectLighting }
{
}

RGBColor Material_PBR::Shade(const HitRecord& hitRecord, const FVector3& l, const FVector3& v) const {
	
	const RGBColor F0{ (!m_IsMetallic) ? (RGBColor{0.04f, 0.04f, 0.04f}) : (m_AlbedoColor) };
	FVector3 vl{ l + v };
	float mgn{ Magnitude(vl) };
	FVector3 halfVector{ vl / mgn };
	Normalize(halfVector);
	const RGBColor F{ Schlick(halfVector, v, F0) };
	const float D{ TrowbridgeReitzGGX(hitRecord.normal, halfVector) };
	const float G{ Smith(hitRecord.normal, v, l) };
	const RGBColor CookTerrance{ BRDF::CookTerrance(D, F, G, l, v, hitRecord.normal) };
	RGBColor kd{};
	if (m_IsMetallic) {
		kd = RGBColor{ 0, 0, 0 };
	}
	else {
		kd = RGBColor{ 1, 1, 1 } - F;
	}
	
	return kd * BRDF::Lambert(RGBColor{ m_DiffuseReflectance, m_DiffuseReflectance , m_DiffuseReflectance }, m_DiffuseColor) + CookTerrance * m_SpecRefl;
}

// Normal distribution function
float Material_PBR::TrowbridgeReitzGGX(const FVector3& normal, const FVector3& halfVector) const {

	float alpha{ Elite::Square(m_Roughness) };
	float alphaSquared{ Elite::Square(alpha) };
	float dotProd{ Dot(normal, halfVector) };
	float denum{ (Elite::Square(dotProd) * (alphaSquared - 1)) + 1 };
	return alphaSquared / (static_cast<float>(E_PI) * (Elite::Square(denum)));
}

// Fresnel function
RGBColor Material_PBR::Schlick(const FVector3& halfVector, const FVector3& v, const RGBColor F0) const {

	RGBColor ones{ 1, 1, 1 };
	float inter{ 1 - Dot(halfVector, v) };
	float pow5{ powf(inter, 5) };
	return F0 + (ones - F0) * pow5;
}

// Geometry function
float Material_PBR::SchlickGGX(const FVector3& normal, const FVector3& v) const {

	float k{};
	if (m_IsDirectLighting) {
		k = Elite::Square(m_Roughness + 1) / 8;
	}
	else {
		k = Elite::Square(Elite::Square(m_Roughness)) / 2;
	}
	float dotProd{ Dot(normal, v) };

	return dotProd / ((dotProd * (1 - k)) + k);
}

// Shadowing & masking function
float Material_PBR::Smith(const FVector3& normal, const FVector3& v, const FVector3& l) const {

	return SchlickGGX(normal, v) * SchlickGGX(normal, l);
}