#pragma once
#include <glm/glm.hpp>
#include <string>
#include "../../build/Transformations.h"
#include "Face.h"

class MeshModel
{
public:
	MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, const std::string& model_name,glm::mat4x4& Transformation);
	virtual ~MeshModel();
	const Face& GetFace(int index) const;
	int GetFacesCount() const;
	const std::string& GetModelName() const;
	void PrintModel()const;
	const glm::mat4x4& GetPreTransformation();
	const glm::vec3 MeshModel::GetVertex(int index)const;

	void Set_S_w(glm::mat4x4& Transformation);
	void Set_R_w(glm::mat4x4& Transformation);
	void Set_T_w(glm::mat4x4& Transformation);
	void Set_S_m(glm::mat4x4& Transformation);
	void Set_R_m(glm::mat4x4& Transformation);
	void Set_T_m(glm::mat4x4& Transformation);


	const glm::mat4x4& GetTransformation()const;
	void SetTransformation();
	void ResetModel();

private:
	std::vector<Face> faces_;
	std::vector<glm::vec3> vertices_;
	std::vector<glm::vec3> normals_;
	glm::mat4x4 PreTransformation;
	glm::mat4x4 S_w= Transformations::Identity4X4Matrix();
	glm::mat4x4 R_w= Transformations::Identity4X4Matrix();
	glm::mat4x4 T_w= Transformations::Identity4X4Matrix();
	glm::mat4x4 S_m= Transformations::Identity4X4Matrix();
	glm::mat4x4 R_m= Transformations::Identity4X4Matrix();
	glm::mat4x4 T_m= Transformations::Identity4X4Matrix();

	glm::mat4x4 Transformation = Transformations::Identity4X4Matrix();
	std::string model_name_;
};
