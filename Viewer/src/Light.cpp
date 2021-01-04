#include "Light.h"


Light::Light()
{
	DlightColor = glm::vec3(0.37, 0.37, 0.37);
	AlightColor = glm::vec3(0.37, 0.37, 0.37);
	SlightColor = glm::vec3(0.37, 0.37, 0.37);
	lightDirection = glm::vec3(0, 0, -1);
	lightPosition = glm::vec4(-0.06,0,0, 1);
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

void Light::SetRotationMatrix(glm::mat4x4& Transformation, bool IsWorld, int Axis)
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
			LRotationX = Transformation;
			break;
		case 2:
			LRotationY = Transformation;
			break;
		case 3:
			LRotationZ = Transformation;
			break;
		}
	}
}

void Light::SetTranslationMatrix(const glm::mat4x4& Transformation, bool IsWorld)
{
	if (IsWorld)
		WTranslate = Transformation;
	else
		LTranslate = Transformation;
}

void Light::SetWorldTransformation()
{
	WorldTransformation = WRotationX * WRotationY * WRotationZ * WTranslate;
}

void Light::SetLocalTransformation()
{
	LocalTransformation =LRotationX * LRotationY * LRotationZ * LTranslate;
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
	WRotationX = Transformations::Identity4X4Matrix();
	WRotationY = Transformations::Identity4X4Matrix();
	WRotationZ = Transformations::Identity4X4Matrix();
	LRotationX = Transformations::Identity4X4Matrix();
	LRotationY = Transformations::Identity4X4Matrix();
	LRotationZ = Transformations::Identity4X4Matrix();
	WTranslate = Transformations::Identity4X4Matrix();
	LTranslate = Transformations::Identity4X4Matrix();
}
void Light::SetAlpha(int a)
{
	alpha = a;
}
int  Light::GetAlpha()
{
	return alpha;
}
void Light::UpdatePosition()
{
	lightPosition = WorldTransformation * LocalTransformation * lightPosition;
}