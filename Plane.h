#pragma once
#include "GeometryObject.h"


class Plane : public GeometryObject {
public:
	Plane(const FPoint3 coord, const FVector3 normal, BaseMaterial* material);
	~Plane() = default;

	Plane(const Plane& other) = delete;
	Plane(Plane&& other) = delete;
	Plane& operator=(Plane&& other) = delete;
	Plane& operator=(const Plane& other) = delete;


	bool Hit(const Ray& ray, HitRecord& hitRecord, const bool shading = false) const override;

private:
	const FVector3 m_Normal;

};

