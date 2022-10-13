#include "BaseMaterial.h"

BaseMaterial::BaseMaterial(float diffRefl, RGBColor diffColor)
	: m_DiffuseReflectance{ diffRefl }
	, m_DiffuseColor{ diffColor }
{
}