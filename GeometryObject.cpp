#include "GeometryObject.h"

using namespace Elite;

GeometryObject::GeometryObject(const FPoint3 center, BaseMaterial* material)
	: m_Center{ center }
	, m_pMaterial{ material }
{
}

