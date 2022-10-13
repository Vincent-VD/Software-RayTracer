#pragma once
#include "GeometryObject.h"

class Sphere : public GeometryObject {
public:
	Sphere(const FPoint3 coord, const float radius, BaseMaterial* material);
	~Sphere() = default;

	Sphere(const Sphere& other) = delete;
	Sphere(Sphere&& other) = delete;
	Sphere& operator=(Sphere&& other) = delete;
	Sphere& operator=(const Sphere& other) = delete;


	bool Hit(const Ray& ray, HitRecord& hitRecord, const bool shading = false) const override;

private:
	const float m_Radius;
	const float m_InvRadius;

};