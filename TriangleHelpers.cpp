#include "TriangleHelpers.h"

void TriangleHelpers::Update(float& totalRotationY, const Elite::FMatrix4& translationMatrix, Elite::FMatrix4& rotationMatrix, std::vector<Elite::FPoint3>& verts, std::vector<Elite::FPoint3>& transformedVerts, float elapsedSec, bool isRotating) {

	if (isRotating) {

		totalRotationY += float(E_PI_DIV_4) * elapsedSec;
		SetRotationY(totalRotationY, rotationMatrix);

		const FMatrix4 finalTransform = translationMatrix * rotationMatrix;
		transformedVerts.clear();

		for (const FPoint3& v : verts)
		{
			transformedVerts.emplace_back((finalTransform * FPoint4{ v }).xyz);
		}
	}
}

bool TriangleHelpers::HitTriangle(const FPoint3& v0, const FPoint3& v1, const FPoint3& v2, const Cullmode cullmode, const Ray& ray, HitRecord& hitRecord, BaseMaterial* material, const bool shading) {
	FVector3 a{ v1 - v0 };
	FVector3 b{ v2 - v0 };

	FVector3 normal = GetNormalized(Cross(a, b));
	const float normalDotProd{ Dot(normal, ray.direction) };

	FPoint3 center = FPoint3{ (FVector3{v0} + FVector3{v1} + FVector3{v2}) * 0.333f };
	const FVector3 rayPlane{ center - static_cast<FVector3>(ray.origin) };

		const float tValue{ Dot(rayPlane, normal) / Dot(ray.direction, normal) };

		if (tValue >= ray.tMin && tValue < ray.tMax) {

			const FPoint3 hitPoint{ ray.origin + tValue * ray.direction };

			//edge A
			FVector3 edgeA{ v1 - v0 };
			FVector3 pointTo0{ hitPoint - v0 };
			if (Dot(normal, Cross(edgeA, pointTo0)) < 0.0f) {
				return false;
			}
			//edge B
			FVector3 edgeB{ v2 - v1 };
			FVector3 pointTo1{ hitPoint - v1 };
			if (Dot(normal, Cross(edgeB, pointTo1)) < 0.0f) {
				return false;
			}
			//edge C
			FVector3 edgeC{ v0 - v2 };
			FVector3 pointTo2{ hitPoint - v2 };
			if (Dot(normal, Cross(edgeC, pointTo2)) < 0.0f) {
				return false;
			}

			if (shading) {
				switch (cullmode) {
				case Cullmode::frontSide:
					if (normalDotProd < 0.0f) {
						return false;
					}
					break;
				case Cullmode::backSide:
					if (normalDotProd > 0.0f) {
						return false;
					}
					break;
				}
				return true;
			}
				bool toShade{ false };
			switch (cullmode) {
			case Cullmode::doublePrimitive:
				toShade = true;
				break;
			case Cullmode::frontSide:
				if (normalDotProd < 0.0f) {
					toShade = true;
				}
				break;
			case Cullmode::backSide:
				if (normalDotProd > 0.0f) {
					toShade = true;
				}
				break;
			}

			if (toShade) {
				hitRecord.hitPoint = hitPoint;
				hitRecord.normal = normal;
				hitRecord.tValue = tValue;
				hitRecord.material = material;
				return true;
			}

		}
		return false;
}


void TriangleHelpers::SetTranslation(const FPoint3& position, Elite::FMatrix4& translationMatrix)
{
	translationMatrix = MakeTranslation(FVector3(position));
}

void TriangleHelpers::SetRotationY(float radians, Elite::FMatrix4& rotationMatrix)
{
	rotationMatrix = MakeRotationY(radians);
}