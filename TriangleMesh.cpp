#include "TriangleMesh.h"

TriangleMesh::TriangleMesh(const FPoint3 position, const std::string filename, const Cullmode cullmode, BaseMaterial* material, bool isRotating)
	: GeometryObject{ position, material }
	, m_pMaterial{ material }
	, m_Cullmode{ cullmode }
	, m_IsRotating{ isRotating }
	, m_TotalRotationY{ 0.f }
	, m_TranslationTransform{ FMatrix4::Identity() }
	, m_RotationTransform{ FMatrix4::Identity() }
{
	ParseObjFile(filename);
	TriangleHelpers::SetTranslation(position, m_TranslationTransform);
}

void TriangleMesh::SetTranslation(const FPoint3& position)
{
	m_TranslationTransform = MakeTranslation(FVector3(position));
}

void TriangleMesh::SetRotationY(float radians)
{
	m_RotationTransform = MakeRotationY(radians);
}

// Update for rotation
void TriangleMesh::Update(float elapsedSec) {

	TriangleHelpers::Update(m_TotalRotationY, m_TranslationTransform, m_RotationTransform, m_Vertices, m_TransformedVertices, elapsedSec, m_IsRotating);
}

// A ray can hit multiple triangles at once, only return the smallest one
bool TriangleMesh::Hit(const Ray& ray, HitRecord& hitRecord, const bool shading) const {
	bool res{ false };

	HitRecord temp{};
	temp.tValue = FLT_MAX;

	for (const IPoint3& indices : m_Indices)
	{
		//Triangle Hit Logic
		FPoint3 v0 = m_TransformedVertices[indices[0]];
		FPoint3 v1 = m_TransformedVertices[indices[1]];
		FPoint3 v2 = m_TransformedVertices[indices[2]];
		if (TriangleHelpers::HitTriangle(v0, v1, v2, m_Cullmode, ray, hitRecord, m_pMaterial, shading)) {
			if (hitRecord.tValue < temp.tValue) {
				temp = hitRecord;
			}
			res = true;
		}
	}

	if (!shading) {
		hitRecord = temp;
	}

	return res;
}

// Referenced from: https://stackoverflow.com/questions/21120699/c-obj-file-parser
void TriangleMesh::ParseObjFile(const std::string& filename) {

	std::vector<FVector3> vertices{};
	std::vector<FVector3> faceIndex{};
	// Open the file
	std::ifstream obj(filename.c_str());
	if (!obj){
		std::cerr << "Cannot open " << filename << std::endl;
		exit(1);
	}

	//Read lines from file
	std::string line;
	while (std::getline(obj, line)) {
		//check 'v'
		if (line.substr(0, 2) == "v ") {
			float x, y, z;
			const char* chh = line.c_str();
			sscanf_s(chh, "v %f %f %f", &x, &y, &z);
			FPoint3 vert = FPoint3{ x, y, z };
			m_Vertices.push_back(vert);
		}
		//check 'f'
		else if (line.substr(0, 2) == "f ") {
			int v0, v1, v2;
			const char* chh = line.c_str();
			sscanf_s(chh, "f %i %i %i", &v0, &v1, &v2);
			v0--;  v1--;  v2--; //OBJ files start at idx 1, not 0
			IPoint3 vert = IPoint3(v0, v1, v2);
			m_Indices.push_back(vert);

		}
	}

	Update(0);

}
