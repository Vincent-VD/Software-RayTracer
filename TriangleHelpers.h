#pragma once
#include <vector>
#include "GeometryObject.h"

enum class Cullmode {
	frontSide = 0,
	backSide = 1,
	doublePrimitive = 2
};

namespace TriangleHelpers {

	void SetTranslation(const FPoint3& position, Elite::FMatrix4& translationMatrix);
	void SetRotationY(float radians, Elite::FMatrix4& rotationMatrix);
	void Update(float& totalRotationY, const Elite::FMatrix4& translationMatrix, Elite::FMatrix4& rotationMatrix, std::vector<Elite::FPoint3>& verts, std::vector<Elite::FPoint3>& transformedVerts, float elapsedSec, bool isRotating);

	bool HitTriangle(const FPoint3& v0, const FPoint3& v1, const FPoint3& v2, const Cullmode cullmode, const Ray& ray, HitRecord& hitRecord, BaseMaterial* material, const bool shading);

}

