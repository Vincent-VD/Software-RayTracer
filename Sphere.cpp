#include "Sphere.h"
#include <iostream>

Sphere::Sphere(const FPoint3 coord, const float radius, BaseMaterial* material)
	: GeometryObject(coord, material)
	, m_Radius{ radius }
	, m_InvRadius{ 1 / radius * radius }
{
}

bool Sphere::Hit(const Ray& ray, HitRecord& hitRecord, const bool shading) const {
	
	FVector3 originDist{ ray.origin - m_Center };
	const float a{ Dot(ray.direction, ray.direction) };
	const float b{ 2 * Dot(ray.direction, originDist) };
	const float c{ Dot(originDist, originDist) - (m_Radius * m_Radius) };
	const float discr{ b * b - 4 * a * c };

	if (discr < 0.0001f) {
		return false;
	}

	const float t{ (-b - sqrtf(discr)) * (0.5f * a) };
	if (t >= ray.tMin && t <= ray.tMax) {
		if (shading) {
			return true;
		}
		//fill in hitRecord
		hitRecord.hitPoint = ray.origin + t * ray.direction;
		hitRecord.normal = FVector3{ hitRecord.hitPoint - m_Center };
		Normalize(hitRecord.normal);
		hitRecord.tValue = t;
		hitRecord.material = m_pMaterial;
		return true;
	}
	return false;
}