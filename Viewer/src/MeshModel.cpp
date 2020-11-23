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
	VerticesCheck=std::vector<bool>(vertices_.size());
	for (int i = 0; i < VerticesCheck.size(); i++)
	{
		VerticesCheck[i] = false;
	}
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

bool MeshModel::GetVertexCheck(int index)
{
	return VerticesCheck[index - 1];
}

void MeshModel::SetVertexCheck(bool val, int index)
{
	VerticesCheck[index - 1] = val;
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

glm::mat4x4 MeshModel::Get_R_m()
{
	return R_m;
}

glm::mat4x4 MeshModel::Get_R_w()
{
	return R_w;
}

void MeshModel::ResetModel()
{
	this->S_m = Transformations::Identity4X4Matrix();
	this->R_m = Transformations::Identity4X4Matrix();
	this->T_m = Transformations::Identity4X4Matrix();
	this->S_w = Transformations::Identity4X4Matrix();
	this->R_w= Transformations::Identity4X4Matrix();
	this->T_w = Transformations::Identity4X4Matrix();
	this->M_Rotation_X = Transformations::Identity4X4Matrix();
	this->M_Rotation_Y = Transformations::Identity4X4Matrix();
	this->M_Rotation_Z = Transformations::Identity4X4Matrix();
	this->W_Rotation_X = Transformations::Identity4X4Matrix();
	this->W_Rotation_Y = Transformations::Identity4X4Matrix();
	this->W_Rotation_Z = Transformations::Identity4X4Matrix();
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

void MeshModel::SetNormalsFlag()
{
	NormalsFlag = !NormalsFlag;
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
			break;
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
			break;
		}
	}
}

bool MeshModel::GetNormalsFlag()
{
	return NormalsFlag;
}

glm::vec3 MeshModel::GetNormals(int index) const
{
	return normals_[(index - 1)];
}

int MeshModel::GetVertexCount()
{
	return vertices_.size();
}

void MeshModel::SetColors(float* BB, float* FN, float* VN, float* MC)
{
	BoundingBoxColor_ = glm::vec3(BB[0],BB[1],BB[2]);
	FacesNormalsColor_ = glm::vec3(FN[0], FN[1], FN[2]);
	NormalsColor_ = glm::vec3(VN[0], VN[1], VN[2]);
	ModelColor_ = glm::vec3(MC[0], MC[1], MC[2]);
}

glm::vec3& MeshModel::GetBB() 
{ 
	return BoundingBoxColor_;
}

glm::vec3& MeshModel::GetFN() 
{ return FacesNormalsColor_; }

glm::vec3& MeshModel::GetVN() 
{ return NormalsColor_; }

glm::vec3& MeshModel::GetMC() 
{ return ModelColor_; }