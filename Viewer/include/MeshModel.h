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
	void SetWorldTransformation(glm::mat4x4& Transformation);
	void SetLocalTransformation(glm::mat4x4& Transformation);
	const glm::mat4x4& GetWorldTransformation()const;
	const glm::mat4x4& GetLocalTransformation()const;

private:
	std::vector<Face> faces_;
	std::vector<glm::vec3> vertices_;
	std::vector<glm::vec3> normals_;
	glm::mat4x4 PreTransformation;
	glm::mat4x4 WorldTransformation=Transformations::Identity4X4Matrix();
	glm::mat4x4 ModelTransformation= Transformations::Identity4X4Matrix();
	std::string model_name_;
};
