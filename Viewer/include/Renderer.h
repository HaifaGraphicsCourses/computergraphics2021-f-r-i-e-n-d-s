#pragma once
#include "Scene.h"
#include <glad/glad.h>
#include <glm/glm.hpp>

class Renderer
{
public:
	Renderer(int viewportWidth, int viewportHeight);
	virtual ~Renderer();
	void Render(Scene& scene);
	void SwapBuffers();
	void ClearColorBuffer(const glm::vec3& color);
	int GetViewportWidth() const;
	int GetViewportHeight() const;
	void SetViewportWidth(int w);
	void SetViewportHeight(int h);
	glm::vec3 RandColor();
	void FogExists(Scene& scene);
	void ClearZ_Buffer();
	glm::vec3 CalcZ(glm::vec3& P, const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3, const glm::vec3& value1, const glm::vec3& value2, const glm::vec3& value3);
	float GetZ(int i, int j) const;
	void PutZ(int i, int j, float z);
	bool ptInTriangle(const glm::vec3& p, const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2);
	float CalcArea(const glm::vec3& v, const glm::vec3& u, const glm::vec3& w);
	float sign(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3);
	void FillZ_Buffer(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3, Scene& scene);
	void DrawLights(Scene& scene);
	glm::vec3 GetAmbientColor(const glm::vec3& Acolor, const glm::vec3& LightAcolor);
	glm::vec3 GetSpecularColor(glm::vec3& I, glm::vec3 n, const glm::vec3& eye, Light& light, const glm::vec3& Scolor);
	glm::vec3 GetDiffuseColor(glm::vec3 normal, glm::vec3 I, Scene& scene);
	glm::vec3 GetColor(glm::vec3 normal, glm::vec3 LightDirection, Scene& scene);
	void ScanConvert_Gouraud(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3, const glm::vec3& c1, const glm::vec3& c2, const glm::vec3& c3);
	void ScanConvert_Phong (const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3, const glm::vec3& vn1, const glm::vec3& vn2, const glm::vec3& vn3, Scene& scene, glm::vec3 color, glm::vec3 LightPosition,Light& light);
	void ScanConvert_Flat(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3, const int& colorMeth, glm::vec3 color, Scene& scene, bool lighting);
	void ScanConvert_Grayscale();
	float GetMinz();
	float GetMaxz();
	void ScanConvert_ZBuffer(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3, Scene& scene);

private:
	void PutPixel(const int i, const int j, const glm::vec3& color);
	void DrawLine(const glm::vec3& p1, const glm::vec3& p2,glm::vec3& color);
	void CreateBuffers(int w, int h);
	void CreateOpenGLBuffer();
	void InitOpenGLRendering();
	float* color_buffer_;
	float* Z_Buffer;
	float MinZ=FLT_MAX;
	float MaxZ=FLT_MIN;
	float MaxC=FLT_MIN;
	int viewport_width_;
	int viewport_height_;
	GLuint gl_screen_tex_;
	GLuint gl_screen_vtc_;
	glm::vec4 parallelLights = glm::vec4(1800, 500, 0, 1);
};
