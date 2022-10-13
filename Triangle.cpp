#include "Triangle.h"


Triangle::Triangle(const FPoint3 position, const FPoint3& v0, const FPoint3& v1, const FPoint3& v2, const Cullmode cm, BaseMaterial* material, bool isRotating)
	: GeometryObject{ position, material }
	, m_Cullmode{ cm }
	, m_IsRotating{ isRotating }
	, m_TotalRotationY{ 0.f }
	, m_TranslationTransform{ FMatrix4::Identity() }
	, m_RotationTransform{ FMatrix4::Identity() }
{
	m_Vertices[0] = v0;
	m_Vertices[1] = v1;
	m_Vertices[2] = v2;

	FVector3 a{ v1 - v0 };
	FVector3 b{ v2 - v0 };

	m_Normal = GetNormalized(Cross(a, b));
	TriangleHelpers::SetTranslation(position, m_TranslationTransform);
}

void Triangle::SetTranslation(const FPoint3& position)
{
	m_TranslationTransform = MakeTranslation(FVector3(position));
}

void Triangle::SetRotationY(float radians)
{
	m_RotationTransform = MakeRotationY(radians);
}

void Triangle::Update(float elapsedSec) {

	TriangleHelpers::Update(m_TotalRotationY, m_TranslationTransform, m_RotationTransform, m_Vertices, m_TransformedVertices, elapsedSec, m_IsRotating);
}

bool Triangle::Hit(const Ray& ray, HitRecord& hitRecord, const bool shading) const {

	FPoint3 v0{ m_TransformedVertices[0] };
	FPoint3 v1{ m_TransformedVertices[1] };
	FPoint3 v2{ m_TransformedVertices[2] };

	return TriangleHelpers::HitTriangle(v0, v1, v2, m_Cullmode, ray, hitRecord, m_pMaterial, shading);

}