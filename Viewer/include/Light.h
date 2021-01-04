#pragma once
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include "MeshModel.h"
#include "Enums.h"


class Light 
{
public:
	Light();

	const glm::vec3& GetDiffuseLightColor() const;
	const glm::vec3& GetAmbientLightColor() const;
	const glm::vec3& GetSpecularLightColor() const;
	const glm::vec3& GetLightDirection() const;
	const glm::vec4& GetLightPosition() const;
	const LightType& GetLightType() const;
	glm::mat4x4 GetWorldTransformation() const;
	glm::mat4x4 GetLocalTransformation() const;
	void SetDiffuseLightColor(glm::vec3 color);
	void SetLightDirection(glm::vec3 direction);
	void SetLightPosition(glm::vec4 position);
	void SetLightType(LightType type);
	void SetAmbientLightColor(glm::vec3 color);
	void SetSpecularLightColor(glm::vec3 color);
	void SetRotationMatrix(glm::mat4x4& Transformation, bool IsWorld, int Axis);
	void SetTranslationMatrix(const glm::mat4x4& Transformation, bool IsWorld);
	void SetWorldTransformation();
	void SetLocalTransformation();
	void ResetTransformations();
	void SetAlpha(int a);
	int GetAlpha();
	void UpdatePosition();

private:
	glm::vec3 DlightColor;
	glm::vec3 AlightColor;
	glm::vec3 SlightColor;
	glm::vec3 lightDirection;
	glm::vec4 lightPosition;
	LightType lightType;
	int alpha;
	glm::mat4x4 WorldTransformation = Transformations::Identity4X4Matrix();
	glm::mat4x4 LocalTransformation = Transformations::Identity4X4Matrix();
	glm::mat4x4 WRotationX = Transformations::Identity4X4Matrix();
	glm::mat4x4 WRotationY = Transformations::Identity4X4Matrix();
	glm::mat4x4 WRotationZ = Transformations::Identity4X4Matrix();
	glm::mat4x4 LRotationX = Transformations::Identity4X4Matrix();
	glm::mat4x4 LRotationY = Transformations::Identity4X4Matrix();
	glm::mat4x4 LRotationZ = Transformations::Identity4X4Matrix();
	glm::mat4x4 WTranslate = Transformations::Identity4X4Matrix();
	glm::mat4x4 LTranslate = Transformations::Identity4X4Matrix();
};