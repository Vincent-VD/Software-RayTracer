#pragma once

#include "EMath.h"
#include "ERGBColor.h"
#include "Ray.h"
#include "BaseMaterial.h"
#include "Material_Lambert.h"

using namespace Elite;

class GeometryObject
{
public:
	GeometryObject(const FPoint3 center, BaseMaterial* material);
	virtual ~GeometryObject() = default;

	GeometryObject(const GeometryObject& other) = delete;
	GeometryObject(GeometryObject&& other) = delete;
	GeometryObject& operator=(GeometryObject&& other) = delete;
	GeometryObject& operator=(const GeometryObject& other) = delete;

	virtual void Update(float elapsedSec) {

	}

	virtual bool Hit(const Ray& ray, HitRecord& hitRecord, const bool shading = false) const = 0;

	//virtual bool Hit(const Ray& ray) const = 0;

	BaseMaterial* GetMaterial() {
		return m_pMaterial;
	}


protected:
	const FPoint3 m_Center;
	BaseMaterial* m_pMaterial;

};

