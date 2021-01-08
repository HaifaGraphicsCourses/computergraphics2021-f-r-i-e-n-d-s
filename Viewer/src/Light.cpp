#include "Light.h"


Light::Light()
{
	DlightColor = glm::vec3(0.37, 0.37, 0.37);
	AlightColor = glm::vec3(0.37, 0.37, 0.37);
	SlightColor = glm::vec3(0.37, 0.37, 0.37);
	lightDirection = glm::vec3(0, 0, -1);
	lightPosition = glm::vec4();
}

const glm::vec3& Light::GetDiffuseLightColor() const
{
	return DlightColor;
}

const glm::vec3& Light::GetAmbientLightColor() const
{
	return AlightColor;
}

const glm::vec3& Light::GetSpecularLightColor() const
{
	return SlightColor;
}

const glm::vec3& Light::GetLightDirection() const
{
	return lightDirection;
}

const glm::vec4& Light::GetLightPosition() const
{
	return lightPosition;
}

const LightType& Light::GetLightType() const
{
	return lightType;
}

void Light::SetDiffuseLightColor(glm::vec3 color)
{
	DlightColor = color;
}

void Light::SetAmbientLightColor(glm::vec3 color)
{
	AlightColor = color;
}

void Light::SetSpecularLightColor(glm::vec3 color)
{
	SlightColor = color;
}

void Light::SetLightDirection(glm::vec3 direction)
{
	lightDirection = direction;
}

void Light::SetLightPosition(glm::vec4 position)
{
	lightPosition = position;
}

void Light::SetLightType(LightType type)
{
	lightType = type;
}

void Light::SetRotationMatrix(float angle, bool IsWorld, int Axis)
{
	if (IsWorld)
	{
		switch (Axis) {
		case 1:
			WRotationX = angle;
			break;
		case 2:
			WRotationY = angle;
			break;
		case 3:
			WRotationZ = angle;
			break;
		}
	}
	else
	{
		switch (Axis) {
		case 1:
			LRotationX = angle;
			break;
		case 2:
			LRotationY = angle;
			break;
		case 3:
			LRotationZ = angle;
			break;
		}
	}
}

void Light::SetTranslationMatrix(float Tx, float Ty, float Tz, bool IsWorld)
{
	if (IsWorld)
	{
			WTranslateX = Tx;
			WTranslateY = Ty;
			WTranslateZ = Tz;
	}
	else
	{
		LTranslateX = Tx;
		LTranslateY = Ty;
		LTranslateZ = Tz;
	}
}

void Light::SetWorldTransformation()
{
	WorldTransformation = Transformations::XRotationTransformation(WRotationX) * Transformations::XRotationTransformation(WRotationY) * Transformations::XRotationTransformation(WRotationZ) * Transformations::TranslationTransformation(WTranslateX, WTranslateY, WTranslateZ);
}

void Light::SetLocalTransformation()
{
	LocalTransformation = WorldTransformation = Transformations::XRotationTransformation(LRotationX) * Transformations::XRotationTransformation(LRotationY) * Transformations::XRotationTransformation(LRotationZ) * Transformations::TranslationTransformation(LTranslateX, LTranslateY, LTranslateZ);

}

glm::mat4x4 Light::GetWorldTransformation()const
{
	return WorldTransformation;
}

glm::mat4x4 Light::GetLocalTransformation()const 
{
	return LocalTransformation;
}

void Light::ResetTransformations()
{
	WorldTransformation = Transformations::Identity4X4Matrix();
	LocalTransformation = Transformations::Identity4X4Matrix();
	WRotationX = 0.f;
	WRotationY = 0.f;
	WRotationZ = 0.f;
	LRotationX = 0.f;
	LRotationY = 0.f;
	LRotationZ = 0.f;
	WTranslateX = 0.f;
	WTranslateY = 0.f;
	WTranslateZ = 0.f;
	LTranslateX = 0.f;
	LTranslateY = 0.f;
	LTranslateZ = 0.f;
}

void Light::SetAlpha(int a)
{
	alpha = a;
}

int  Light::GetAlpha()
{
	return alpha;
}

float Light::GetWRotationX() {
	return WRotationX;
}
float Light::GetWRotationY() {
	return WRotationY;
}
float Light::GetWRotationZ() {
	return WRotationZ;
}
float Light::GetLRotationX() {
	return LRotationX;
}
float Light::GetLRotationY() {
	return LRotationY;
}
float Light::GetLRotationZ() {
	return LRotationZ;
}
float Light::GetWTranslateX() { return WTranslateX; }
float Light::GetWTranslateY() { return WTranslateY; }
float Light::GetWTranslateZ() { return WTranslateZ; }
float Light::GetLTranslateX() { return LTranslateX; }
float Light::GetLTranslateY() { return LTranslateY; }
float Light::GetLTranslateZ() { return LTranslateZ; }