#pragma once
#include <glm/glm.hpp>
#include <string>
#include "Transformations.h"
#include "Face.h"
#include "ModelParameters.h"
#include "Enums.h"

#define GRAYSCALE 999
#define RANDOM_COLORED 990
#define MODEL_COLOR 900

class MeshModel
{
public:
	MeshModel();
	MeshModel(ModelParameters& model);
	virtual ~MeshModel();
	const Face& GetFace(int index) const;
	int GetFacesCount() const;
	const std::string& GetModelName() const;
	void PrintModel()const;
	const glm::mat4x4& GetPreTransformation();
	const glm::vec3 MeshModel::GetVertex(int index)const;
	void Set_S_w(glm::mat4x4& Transformation);
	void Set_R_w();
	void Set_T_w(glm::mat4x4& Transformation);
	void Set_S_m(glm::mat4x4& Transformation);
	void Set_R_m();
	void Set_T_m(glm::mat4x4& Transformation);
	void SetRotationMatrix(glm::mat4x4& Transformation,bool IsWorld,int Axis);
	void SetBoundingBoxFlag();
	bool GetBoundingBoxFlag();
	const glm::mat4x4& GetTransformation()const;
	void SetTransformation();
	void ResetModel();
	glm::vec4 GetLeftTopNear();
	glm::vec4 GetRightTopNear();
	glm::vec4 GetLeftTopFar();
	glm::vec4 GetRightTopFar();
	glm::vec4 GetLeftBottomNear();
	glm::vec4 GetLeftBottomFar();
	glm::vec4 GetRightBottomNear();
	glm::vec4 GetRightBottomFar();
	int GetVertexCount();
	void SetFacesNormalsFlag();
	void SetNormalsFlag();
	glm::vec3 GetNormals(int index) const;
	bool GetFacesNormalsFlag();
	bool GetNormalsFlag();
	glm::mat4x4 Get_R_m();
	glm::mat4x4 Get_R_w();
	glm::mat4x4 Get_S_w();
	glm::mat4x4 Get_S_m();
	glm::mat4x4 Get_T_w();
	glm::mat4x4 Get_T_m();
	void SetWorldTransformation(glm::mat4x4 Transformation);
	void SetModelTransformation(glm::mat4x4 Transformation);
	glm::mat4x4 GetWorldTransformation();
	glm::mat4x4 GetModelTransformation();
	void SetColors(float* BB, float* FN, float* VN);
	glm::vec3& GetBB();
	glm::vec3& GetFN();
	glm::vec3& GetVN();
	float GetMinOrtho();
	float GetMaxOrtho();
	float GetTranslateFactor();
	glm::vec3 GetPreffered_Eye();
	void SetColorMethod(int meth);
	int GetColorMethod();
	glm::vec3 GetDiffuseColor();
	glm::vec3 GetAmbientColor();
	glm::vec3 GetSpecularColor();
	void SetDiffuseColor(glm::vec3& collor);
	void SetAmbientColor(glm::vec3& collor);
	void SetSpecularColor(glm::vec3& collor);
	glm::vec4 GetModelCenter();

private:
	std::vector<bool> VerticesCheck;
	std::vector<Face> faces_;
	std::vector<glm::vec3> vertices_;
	std::vector<glm::vec3> normals_;
	glm::mat4x4 PreTransformation;
	std::vector<glm::vec3> boundingBoxVertices;
	glm::mat4x4 S_w= Transformations::Identity4X4Matrix();
	glm::mat4x4 R_w= Transformations::Identity4X4Matrix();
	glm::mat4x4 T_w= Transformations::Identity4X4Matrix();
	glm::mat4x4 S_m= Transformations::Identity4X4Matrix();
	glm::mat4x4 R_m= Transformations::Identity4X4Matrix();
	glm::mat4x4 T_m= Transformations::Identity4X4Matrix();
	glm::mat4x4 M_Rotation_X = Transformations::Identity4X4Matrix();
	glm::mat4x4 M_Rotation_Y = Transformations::Identity4X4Matrix();
	glm::mat4x4 M_Rotation_Z= Transformations::Identity4X4Matrix();
	glm::mat4x4 W_Rotation_X = Transformations::Identity4X4Matrix();
	glm::mat4x4 W_Rotation_Y = Transformations::Identity4X4Matrix();
	glm::mat4x4 W_Rotation_Z = Transformations::Identity4X4Matrix();
	glm::mat4x4 WorldTransformation = Transformations::Identity4X4Matrix();
	glm::mat4x4 Transformation = Transformations::Identity4X4Matrix();
	glm::mat4x4 ModelTransformation = Transformations::Identity4X4Matrix();
	glm::vec4 leftTopNear_;
	glm::vec4 rightTopNear_;
	glm::vec4 leftTopFar_;
	glm::vec4 rightTopFar_;
	glm::vec4 leftBottomNear_;
	glm::vec4 rightBottomNear_;
	glm::vec4 leftBottomFar_;
	glm::vec4 rightBottomFar_;
	std::string model_name_;
	bool ShowOrHideBoundingBox = false;
	bool ShowOrHideFacesNormals = false;
	bool NormalsFlag = false;
	glm::vec3 BoundingBoxColor_;
	glm::vec3 FacesNormalsColor_;
	glm::vec3 NormalsColor_;
	glm::vec3 DModelColor;
	glm::vec3 AModelColor;
	glm::vec3 SModelColor;
	glm::vec3 Preffered_eye;
	float TranslateFactor;
	float minOrtho, maxOrtho;
	int colorMethod=GRAYSCALE;
	glm::vec3 ModelCenter;

	

};
