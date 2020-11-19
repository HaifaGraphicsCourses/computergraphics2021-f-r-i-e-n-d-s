#include "MeshModel.h"
#include <iostream>
#include "Renderer.h"


MeshModel::MeshModel(ModelParameters model) :
	faces_(model.faces),
	vertices_(model.vertices),
	normals_(model.normals),
	PreTransformation(model.preTransformation),
	leftTopNear_(model.leftTopNear),
	rightTopNear_(model.rightTopNear),
	leftTopFar_(model.leftTopFar),
	rightTopFar_(model.rightTopFar),
	leftBottomNear_(model.leftBottomNear),
	rightBottomNear_(model.rightBottomNear),
	leftBottomFar_(model.leftBottomFar),
	rightBottomFar_(model.rightBottomFar)
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
	this->Transformation = S_w * R_w * T_w * S_m * T_m *R_m ;
}
const glm::mat4x4& MeshModel::GetTransformation()const {
	return this->Transformation;
}
void MeshModel::Set_S_w(glm::mat4x4& Transformation) {
	S_w = Transformation;
}
void MeshModel::Set_R_w() {
	R_w = (W_Rotation_X) *(W_Rotation_Y) *(W_Rotation_Z);
}
void MeshModel::Set_T_w(glm::mat4x4& Transformation) {
	T_w = Transformation;
}
void MeshModel::Set_S_m(glm::mat4x4& Transformation) {
	S_m = Transformation;
}
void MeshModel::Set_R_m() {
	R_m =(M_Rotation_X)*(M_Rotation_Y)*(M_Rotation_Z);
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
void MeshModel::SetBoundingBoxFlag()
{
	ShowOrHideBoundingBox = !ShowOrHideBoundingBox;
}
bool MeshModel::GetBoundingBoxFlag()
{
	return ShowOrHideBoundingBox;
}
void MeshModel::SetFacesNormalsFlag()
{
	ShowOrHideFacesNormals = !ShowOrHideFacesNormals;
}
bool MeshModel::GetFacesNormalsFlag()
{
	return ShowOrHideFacesNormals;
}
glm::vec4 MeshModel::GetLeftTopNear()
{
	return leftTopNear_;
}
glm::vec4 MeshModel::GetRightTopNear()
{
	return rightTopNear_;
}
glm::vec4 MeshModel::GetLeftTopFar()
{
	return leftTopFar_;
}
glm::vec4 MeshModel::GetRightTopFar()
{
	return rightTopFar_;
}
glm::vec4 MeshModel::GetLeftBottomNear()
{
	return leftBottomNear_;
}
glm::vec4 MeshModel::GetLeftBottomFar()
{
	return leftBottomFar_;
}
glm::vec4 MeshModel::GetRightBottomNear()
{
	return rightBottomNear_;
}
glm::vec4 MeshModel::GetRightBottomFar()
{
	return rightBottomFar_;
}
void MeshModel::SetRotationMatrix(glm::mat4x4& Transformation, bool IsWorld, int Axis)
{
	if (IsWorld)
	{
		switch (Axis) {
		case 1:
			W_Rotation_X = Transformation;
			break;
		case 2:
			W_Rotation_Y = Transformation;
			break;
		case 3:
			W_Rotation_Z = Transformation;
		}
	}
	else
	{
		switch (Axis) {
		case 1:
			M_Rotation_X = Transformation;
			break;
		case 2:
			M_Rotation_Y = Transformation;
			break;
		case 3:
			M_Rotation_Z = Transformation;
		}
	}
}
void MeshModel::ComputeFacesNormals(glm::mat4x4 Transformation)
{
	for (int faceIndex = 0; faceIndex < faces_.size(); ++faceIndex)
	{
		Face face = GetFace(faceIndex);
		int VertexIndex1 = face.GetVertexIndex(0);
		int VertexIndex2 = face.GetVertexIndex(1);
		int VertexIndex3 = face.GetVertexIndex(2);
		glm::vec4 v1Temp = Transformation * glm::vec4(GetVertex(VertexIndex1),1);
		glm::vec4 v2Temp = Transformation * glm::vec4(GetVertex(VertexIndex2),1);
		glm::vec4 v3Temp = Transformation * glm::vec4(GetVertex(VertexIndex3),1);
		glm::vec3 v1 (v1Temp.x / v1Temp.w, v1Temp.y / v1Temp.w, v1Temp.z / v1Temp.w);
		glm::vec3 v2 (v2Temp.x / v2Temp.w, v2Temp.y / v2Temp.w, v2Temp.z / v2Temp.w);
		glm::vec3 v3 (v3Temp.x / v3Temp.w, v3Temp.y / v3Temp.w, v3Temp.z / v3Temp.w);
		glm::vec3 faceCenter = (v1 + v2 + v3) / 3.0f;
		glm::vec3 faceNormal = normalize(cross(glm::vec3(v1 - v2), glm::vec3(v1 - v3)));
		faces_[faceIndex].SetNormal(faceNormal);
		faces_[faceIndex].SetCenter(faceCenter);
	}
}