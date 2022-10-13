#pragma once
#include "BaseMaterial.h"

class Material_PBR : public BaseMaterial {
public:

	Material_PBR(	const float diffRefl, 
					const  RGBColor color, 
					const  float specRefl, 
					const  RGBColor albedo, 
					const  bool isMetallic, 
					const  float roughness, 
					const bool isDirectLighting);
	~Material_PBR() = default;

	Material_PBR(const Material_PBR& other) = delete;
	Material_PBR(Material_PBR&& other) = delete;
	Material_PBR& operator=(Material_PBR&& other) = delete;
	Material_PBR& operator=(const Material_PBR& other) = delete;

	virtual RGBColor Shade(const HitRecord& hitRecord, const FVector3& w0, const FVector3& w1) const override;

private:
	float m_SpecRefl;
	RGBColor m_AlbedoColor;
	bool m_IsMetallic;
	float m_Roughness;
	bool m_IsDirectLighting;

	float TrowbridgeReitzGGX(const FVector3& normal, const FVector3& halfVector) const;
	RGBColor Schlick(const FVector3& halfVector, const FVector3& v, const RGBColor F0) const;
	float SchlickGGX(const FVector3& normal, const FVector3& v) const;
	float Smith(const FVector3& normal, const FVector3& v, const FVector3& l) const;
};

