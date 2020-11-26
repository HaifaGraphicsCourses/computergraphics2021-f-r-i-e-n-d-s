#pragma once
#include <glm/glm.hpp>
#include <Transformations.h>

class Camera
{
public:
	Camera();
	virtual ~Camera();
	void SetCameraLookAt();
	glm::mat4x4 GetLookAt();
	const glm::mat4x4& GetProjectionTransformation() const;
	const glm::mat4x4& GetViewTransformation() const;
	void SetOrthographicWidth(float width);
	void UpdateOrthographicMat();
	void SetFovy(float fovy);
	void UpdatePerspectiveMat();
	bool GetIsOrthographic();
	void SetIsOrthographic(bool val);
	void SetAspectRatio(float W, float H);
	glm::vec3 GetEye();
	glm::vec3 GetAt();
	glm::vec3 GetUp();
	void SetEye(glm::vec3 eye);
	void SetAt(glm::vec3 at);
	void SetUp(glm::vec3 at);
	void SetWTransformation(glm::mat4x4 T);
	void SetCTransformation(glm::mat4x4 T);
	void SetTransformations();
	void SetRotationMatrix(glm::mat4x4& Transformation, bool IsWorld, int Axis);
	void ResetTransformations();
	

private:
	float width=1920;
	float height=1080;
	float aspectratio = width/height;   
	float left = -width / 2;
	float right = width / 2;
	float bottom = -height / 2;
	float top = height / 2;
	float Near = -0.1;
	float Far = 1000;
	float Fovy = glm::radians(45.f);
	glm::vec3 eye;
	glm::vec3 at;
	glm::vec3 up;
	glm::mat4x4 LookAt;
	glm::mat4x4 view_transformation_;
	glm::mat4x4 projection_transformation_;
	glm::mat4x4 WorldTransformation=Transformations::Identity4X4Matrix();
	glm::mat4x4 CameraTransformation=Transformations::Identity4X4Matrix();
	glm::mat4x4 C = Transformations::Identity4X4Matrix();
	glm::mat4x4 WRotationX=Transformations::Identity4X4Matrix();
	glm::mat4x4 WRotationY=Transformations::Identity4X4Matrix();
	glm::mat4x4 WRotationZ=Transformations::Identity4X4Matrix();
	glm::mat4x4 CRotationX=Transformations::Identity4X4Matrix();
	glm::mat4x4 CRotationY=Transformations::Identity4X4Matrix();
	glm::mat4x4 CRotationZ=Transformations::Identity4X4Matrix();
	glm::mat4x4 WTranslate = Transformations::Identity4X4Matrix();
	glm::mat4x4 CTranslate = Transformations::Identity4X4Matrix();
	bool IsOrthographic = true;
	
};
//#pragma once
//#include <glm/glm.hpp>
//#include "Transformations.h"
//#include <ModelParameters.h>
//#include "MeshModel.h"
//
//class Camera : public MeshModel
//{
//public:
//	Camera();
//	Camera(ModelParameters& model, float aspectRatio);
//	virtual ~Camera();
//	void SetCameraLookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up);
//	const glm::mat4x4& GetProjectionTransformation() const;
//	const glm::mat4x4& GetViewTransformation() const;
//	void UpdateProjectionMatrix();
//	void SetOrthographicProjection(const float height, const float aspectRatio, const float zNear, const float zFar);
//	void SetPerspectiveProjection(const float fovy, const float aspectRatio, const float zNear, const float zFar);
//	void SetOrthographicTransformation(float right, float left, float top, float bottom, float zNear, float zFar);
//	float GetZooming();
//	float GetFovy();



//	void SetIsUsed(bool value);
//	void ResetMatrices();
//	void UpdateViewVolume(float zooming);
//	void UpdateFovy(float fovy);
//	void UpdateViewTransformation();
//	void UpdateWorldTransformationEyeChanged(glm::vec3 difference);
//	const glm::vec4& GetCameraPosition() const;
//	void SetCameraPosition(glm::vec4 value);
//	void SetAspectRatio(float aspectRatio);
//	const float GetWorldRotate_X() const;
//	const float GetWorldRotate_Y() const;
//	const float GetWorldRotate_Z() const;
//	const float GetLocalRotate_X() const;
//	const float GetLocalRotate_Y() const;
//	const float GetLocalRotate_Z() const;
//	void SetWorldRotate_X(float angle);
//	void SetWorldRotate_Y(float angle);
//	void SetWorldRotate_Z(float angle);
//	void SetLocalRotate_X(float angle);
//	void SetLocalRotate_Y(float angle);
//	void SetLocalRotate_Z(float angle);
//
//private:
//	glm::mat4x4 LastLookAtMat = Transformations::Identity4X4Matrix();
//	glm::vec3 LastEye;
//	glm::vec3 Eye;
//	glm::vec3 At;
//	glm::vec3 Up;
//	glm::vec4 CameraPosition;
//	glm::mat4x4 view_transformation_;
//	glm::mat4x4 projection_transformation_;
//	glm::mat4x4 OrthographicTransformation;
//	glm::mat4x4 PerspectiveTransformation;
//	bool IsOrthographic;
//	glm::vec3 StartingEye;
//	glm::vec3 StartingAt;
//	glm::vec3 StartingUp;
//	float StartingRight;
//	float StartingLeft;
//	float StartingTop;
//	float StartingBottom;
//	float StartingNear;
//	float StartingFar;
//	float Right;
//	float Left;
//	float Top;
//	float Bottom;
//	float Near;
//	float Far;
//	float Fovy;
//	float Zooming;
//	bool IsUsed;
//	float AspectRatio;
//	float Height;
//	float Width;
//	float WorldRotate_X=0.f;
//	float WorldRotate_Y=0.f;
//	float WorldRotate_Z=0.f;
//	float LocalRotate_X=0.f;
//	float LocalRotate_Y=0.f;
//	float LocalRotate_Z=0.f;
//};
//