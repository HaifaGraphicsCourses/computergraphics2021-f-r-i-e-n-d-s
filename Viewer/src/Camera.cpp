#include "Camera.h"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtx/transform.hpp>
#include <Transformations.h>

Camera::Camera()
{
	this->width = 1920;
	this->height = 1080;
	this->eye = glm::vec3(0, 0, 100);
	this->at = glm::vec3(0, 0, 0);
	this->up = glm::vec3(0, 1, 0);
	WorldTransformation = Transformations::Identity4X4Matrix();
	SetCameraLookAt();
	view_transformation_ = Transformations::Identity4X4Matrix();
	projection_transformation_ = Transformations::Identity4X4Matrix();
}

Camera::~Camera()
{

}

const glm::mat4x4& Camera::GetProjectionTransformation() const
{
	return projection_transformation_;
}

const glm::mat4x4& Camera::GetViewTransformation() const
{
	return view_transformation_;
}

void Camera::SetCameraLookAt()
{
	LookAt = glm::lookAt(eye, at, up);
}

glm::mat4x4 Camera::GetLookAt()
{
	return this->LookAt;
}

void Camera::SetOrthographicWidth(float Orthowidth) 
{
	if (Orthowidth == 0)
		return;
	this->width = Orthowidth;
	this->height = Orthowidth / aspectratio;
	this->left = -Orthowidth / 2;
	this->right = Orthowidth / 2;
	this->bottom = -this->height/2;
	this->top = this->height / 2;
	UpdateOrthographicMat();
}

void Camera::SetFovy(float fovy) {
	this->Fovy = glm::radians(fovy);
	UpdatePerspectiveMat();
}

void Camera::UpdateOrthographicMat()
{
	projection_transformation_ = glm::ortho(this->left, this->right, this->bottom, this->top,this->Near,this->Far);
}

void Camera::UpdatePerspectiveMat() 
{
	projection_transformation_ = glm::perspective(this->Fovy, aspectratio, Near, Far);
}

bool Camera::GetIsOrthographic()
{
	return IsOrthographic;
}

void Camera::SetIsOrthographic(bool val)
{
	this->IsOrthographic = val;
}

void Camera::SetAspectRatio(float W, float H) 
{
	this->width = W;
	this->height = H;
	this->aspectratio = W / H;
}

glm::vec3 Camera::GetEye()
{
	return eye;
}

glm::vec3 Camera::GetAt()
{
	return at;
}

glm::vec3 Camera::GetUp()
{
	return up;
}

void Camera::SetEye(glm::vec3 Eye)
{
	this->eye = Eye;
}

void Camera::SetAt(glm::vec3 At)
{
	this->at = At;
}

void Camera::SetUp(glm::vec3 Up)
{
	this->up = Up;
}

void Camera::SetWTransformation(glm::mat4x4 T)
{
	WTranslate = T;
	WorldTransformation = (WRotationX) *(WRotationY) *(WRotationZ) * T;
}

void Camera::SetCTransformation(glm::mat4x4 T)
{
	CTranslate = T;
	CameraTransformation = (CRotationX) *(CRotationY) *(CRotationZ) * T;
}

void Camera::SetTransformations()
{
	C = WorldTransformation * CameraTransformation;
	LookAt = glm::inverse(C);
}

void Camera::SetRotationMatrix(glm::mat4x4& Transformation, bool IsWorld, int Axis)
{
	if (IsWorld)
	{
		switch (Axis) {
		case 1:
			WRotationX = Transformation;
			break;
		case 2:
			WRotationY = Transformation;
			break;
		case 3:
			WRotationZ = Transformation;
			break;
		}
	}
	else
	{
		switch (Axis) {
		case 1:
			CRotationX = Transformation;
			break;
		case 2:
			CRotationY = Transformation;
			break;
		case 3:
			CRotationZ = Transformation;
			break;
		}
	}
}

void Camera::ResetTransformations()
{
	WRotationX = Transformations::Identity4X4Matrix();
	WRotationY = Transformations::Identity4X4Matrix();
	WRotationZ = Transformations::Identity4X4Matrix();
	CRotationX = Transformations::Identity4X4Matrix();
	CRotationY = Transformations::Identity4X4Matrix();
	CRotationZ = Transformations::Identity4X4Matrix();
	WTranslate = Transformations::Identity4X4Matrix();
	CTranslate = Transformations::Identity4X4Matrix();
	WorldTransformation = Transformations::Identity4X4Matrix();
	CameraTransformation = Transformations::Identity4X4Matrix();
	C = Transformations::Identity4X4Matrix();
}










//Camera::Camera()
//{
//	Right = 400;
//	Left = -400;
//	Top = 400;
//	Bottom = -400;
//	Near = 0.4;
//	Far = 800;
//	Fovy= glm::pi<float>() / 4.0f;
//	OrthographicTransformation = glm::ortho(Left, Right, Bottom, Top, Near, Far);
//	IsOrthographic = true;
//	Eye = glm::vec3(0, 0, 5);
//	At = glm::vec3(0, 0, 0);
//	Up = glm::vec3(0, 1, 0);
//	Zooming = 1;
//	StartingEye = glm::vec3(0, 0, 5);
//	StartingAt = glm::vec3(0, 0, 0);
//	StartingUp = glm::vec3(0, 1, 0);
//}
//
//Camera::Camera(ModelParameters& model, float ascpectRatio):MeshModel(model)
//{
//	Right = 400;
//	Left = -400;
//	Top = 400;
//	Bottom = -400;
//	Near = 0.4;
//	Far = 800;
//	Fovy = glm::pi<float>() / 4.0f;
//	StartingRight = 100;
//	StartingLeft = -100;
//	StartingTop = 100;
//	StartingBottom = -100;
//	StartingNear = 0.1;//100;
//	StartingFar = 200;// -100;
//	AspectRatio = ascpectRatio;
//	IsOrthographic = false;
//	IsUsed = false;
//	Eye = glm::vec3(0, 0, 100);
//	At = glm::vec3(0, 0, 0);
//	Up = glm::vec3(0, 1, 0);
//	StartingEye = glm::vec3(0, 0, 100);
//	StartingAt = glm::vec3(0, 0, 0);
//	StartingUp = glm::vec3(0, 1, 0);
//	view_transformation_ = Transformations::Identity4X4Matrix();
//	SetWorldTransformation(Transformations::TranslationTransformation(Eye.x, Eye.y, Eye.z) * Transformations::ScalingTransformation(0.1, 0.1, 0.1));
//	LastEye = Eye;
//	Zooming = 1;
//	Height = 200;
//	UpdateProjectionMatrix();
//}
//
//Camera::~Camera()
//{
//	
//}
//
//const glm::mat4x4& Camera::GetViewTransformation() const
//{
//	return view_transformation_;
//}
//
//void Camera::UpdateProjectionMatrix()
//{
//	if (IsOrthographic)
//	{
//		SetOrthographicProjection(Height, AspectRatio, Near, Far);
//	}
//	else
//	{
//		SetPerspectiveProjection(Fovy, AspectRatio, Near, Far);
//	}
//}
//
//void Camera::SetPerspectiveProjection(const float fovy, const float aspectRatio, const float zNear, const float zFar)
//{
//	IsOrthographic = false;
//	projection_transformation_ = glm::perspective(fovy, aspectRatio, zNear, zFar);
//}
//
//void Camera::SetCameraLookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up)
//{
//	glm::vec3 z = normalize(Eye-At);
//	glm::vec3 x = normalize(cross(Up, z));
//	glm::vec3 y = normalize(cross(z, x));
//
//	glm::vec4 t = glm::vec4(0, 0, 0, 1);
//
//	glm::mat4x4 c = glm::mat4x4(glm::vec4(x, 0), glm::vec4(y, 0), glm::vec4(z, 0), t);
//	glm::mat4x4 translation = glm::mat4x4(
//		1, 0, 0, LastEye[0] - Eye[0],
//		0, 1, 0, LastEye[1] - Eye[1],
//		0, 0, 1, LastEye[2] - Eye[2],
//		0, 0, 0, 1
//	);
//	LastEye = Eye;
//	SetModelTransformation(Transformations::Identity4X4Matrix());
//	SetWorldTransformation(MeshModel::GetWorldTransformation() * inverse(LastLookAtMat) * translation * c);
//	LastLookAtMat = translation * c;
//}
//















//
//bool Camera::GetIsOrthographic()
//{
//	return IsOrthographic;
//}
//
//float Camera::GetZooming()
//{
//	return Zooming;
//}
//
//float Camera::GetFovy()
//{
//	return Fovy;
//}
//














//
//void Camera::SetIsUsed(bool value)
//{
//	IsUsed = value;
//}
//
//void Camera::SetIsOrthographic(bool value)
//{
//	IsOrthographic = value;
//}
//
//void Camera::SetOrthographicTransformation(float right, float left, float top, float bottom, float zNear, float zFar)
//{
//	OrthographicTransformation = glm::mat4x4(
//		2 / (right - left), 0, 0, -((right + left) / (right - left)),
//		0, 2 / (top - bottom), 0, -((top + bottom) / (top - bottom)),
//		0, 0, 2 / (zNear - zFar), -((zNear + zFar) / (zFar - zNear)),
//		0, 0, 0, 1
//	);
//}
//
//void Camera::ResetMatrices()
//{
//	Eye = StartingEye;
//	Up = StartingUp;
//	At = StartingAt;
//	Left = StartingLeft;
//	Right = StartingRight;
//	Top = StartingTop;
//	Bottom = StartingBottom;
//	Near = StartingNear;
//	Far = StartingFar;
//	view_transformation_ = Transformations::Identity4X4Matrix();
//	LastLookAtMat = Transformations::Identity4X4Matrix();
//	LastEye = Eye;
//}
//
//void Camera::UpdateViewVolume(float zooming)
//{
//	Left = StartingLeft / zooming;
//	Right = StartingRight / zooming;
//	Top = StartingTop / zooming;
//	Bottom = StartingBottom / zooming;
//	Near = StartingNear / zooming;
//	Far = StartingFar / zooming;
//}
//
//void Camera::UpdateFovy(float fovy)
//{
//	Fovy = fovy;
//	UpdateProjectionMatrix();
//}
//
//void Camera::UpdateViewTransformation()
//{
//	view_transformation_ = glm::inverse(GetWorldTransformation() * GetModelTransformation());
//}
//
//void Camera::UpdateWorldTransformationEyeChanged(glm::vec3 difference)
//{
//	SetWorldTransformation(GetWorldTransformation() * Transformations::TranslationTransformation(difference.x, difference.y, difference.z));
//}
//
//const glm::vec4& Camera::GetCameraPosition() const
//{
//	return CameraPosition;
//}
//
//void Camera::SetCameraPosition(glm::vec4 value)
//{
//	CameraPosition = value;
//}
//
//void Camera::SetOrthographicProjection(const float height, const float aspectRatio, const float zNear, const float zFar)
//{
//	IsOrthographic = true;
//	float width = aspectRatio * height;
//	projection_transformation_ = glm::ortho(-width / 2, width / 2, -height / 2, height / 2, zNear, zFar);
//}
//
//void Camera::SetAspectRatio(float aspectRatio)
//{
//	AspectRatio = aspectRatio;
//	UpdateProjectionMatrix();
//}
//
//const float Camera::GetWorldRotate_X() const { return WorldRotate_X; }
//const float Camera::GetWorldRotate_Y() const { return WorldRotate_Y; }
//const float Camera::GetWorldRotate_Z() const { return WorldRotate_Z; }
//const float Camera::GetLocalRotate_X() const { return LocalRotate_X; }
//const float Camera::GetLocalRotate_Y() const { return LocalRotate_Y; }
//const float Camera::GetLocalRotate_Z() const { return LocalRotate_Z; }
//
//void Camera::SetWorldRotate_X(float angle){WorldRotate_X=angle;}
//void Camera::SetWorldRotate_Y(float angle){WorldRotate_Y=angle;}
//void Camera::SetWorldRotate_Z(float angle){WorldRotate_Z=angle;}
//void Camera::SetLocalRotate_X(float angle){LocalRotate_X=angle;}
//void Camera::SetLocalRotate_Y(float angle){LocalRotate_Y=angle;}
//void Camera::SetLocalRotate_Z(float angle){LocalRotate_Z=angle;}