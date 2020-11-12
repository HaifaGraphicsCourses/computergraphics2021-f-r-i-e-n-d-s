#include "MeshModel.h"
#include <iostream>


MeshModel::MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, const std::string& model_name, glm::mat4x4& Transformation) :
	faces_(faces),
	vertices_(vertices),
	normals_(normals),
	PreTransformation(Transformation)
{

}

MeshModel::~MeshModel()
{
}

const Face& MeshModel::GetFace(int index) const
{
	return faces_[index];
}

int MeshModel::GetFacesCount() const
{
	return faces_.size();
}

const std::string& MeshModel::GetModelName() const
{
	return model_name_;
}

void MeshModel::PrintModel() const
{
	std::cout << "Model Name is: " << model_name_<<"\n";
	std::cout << "Printing Faces of the Model:\n------------------------------------------------------\n";
	for (int i = 0; i < this->GetFacesCount(); i++) {
		std::cout << "Face" << i + 1 << ": ";
		this->GetFace(i).PrintFace();
		std::cout << "\n";
	}
	std::cout << "\n------------------------------------------------------\n";
	std::cout << "Printing Vertices of the Model:\n------------------------------------------------------\n";
	for (std::vector<int>::size_type i = 0; i != vertices_.size(); i++) {
		std::cout << vertices_[i].x << " " << vertices_[i].y << " " << vertices_[i].z << "\n";
	}
	std::cout << "Printing Normals of the Model:\n------------------------------------------------------\n";
	for (std::vector<int>::size_type i = 0; i != normals_.size(); i++) {
		std::cout << normals_[i].x << " " << normals_[i].y << " " << normals_[i].z << "\n";
	}
}

const glm::vec3 MeshModel::GetVertex(int index)const {
	return vertices_[index-1];
}

const glm::mat4x4& MeshModel::GetPreTransformation() {
	return PreTransformation;
}
void MeshModel::SetTransformation() {
	this->Transformation = S_w * R_w * T_w * S_m * R_m * T_m;
}
const glm::mat4x4& MeshModel::GetTransformation()const {
	return this->Transformation;
}
void MeshModel::Set_S_w(glm::mat4x4& Transformation) {
	S_w = Transformation;
}
void MeshModel::Set_R_w(glm::mat4x4& Transformation) {
	R_w = Transformation;
}
void MeshModel::Set_T_w(glm::mat4x4& Transformation) {
	T_w = Transformation;
}
void MeshModel::Set_S_m(glm::mat4x4& Transformation) {
	S_m = Transformation;
}
void MeshModel::Set_R_m(glm::mat4x4& Transformation) {
	R_m = Transformation;
}
void MeshModel::Set_T_m(glm::mat4x4& Transformation) {
	T_m = Transformation;
}
void MeshModel::ResetModel()
{
	this->S_m = Transformations::Identity4X4Matrix();
	this->R_m = Transformations::Identity4X4Matrix();
	this->T_m = Transformations::Identity4X4Matrix();
	this->S_w = Transformations::Identity4X4Matrix();
	this->R_w= Transformations::Identity4X4Matrix();
	this->T_w = Transformations::Identity4X4Matrix();
	this->Transformation = Transformations::Identity4X4Matrix();
}