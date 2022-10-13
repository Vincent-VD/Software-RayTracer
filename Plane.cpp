#include "Plane.h"

Plane::Plane(const FPoint3 coord, const FVector3 normal, BaseMaterial* material)
	: GeometryObject{coord, material}
	, m_Normal{ normal }
{
}

bool Plane::Hit(const Ray& ray, HitRecord& hitRecord, const bool shading) const {

	FVector3 rayPlane{ m_Center - ray.origin };
	const float t = (Dot(rayPlane, m_Normal)) / (Dot(ray.direction, m_Normal));
	
	if (t >= ray.tMin && t < ray.tMax) {
		if (shading) {
			return true;
		}
		hitRecord.hitPoint = ray.origin + (t * ray.direction);
		hitRecord.tValue = t;
		hitRecord.normal = m_Normal;
		hitRecord.material = m_pMaterial;
		return true;
	}
	return false;
}