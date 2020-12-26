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
	void DrawTriangles(const int& colorMeth, glm::vec3 color);
	void ClearZ_Buffer();
	void CalcZ(glm::vec3& P, const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3);
	float GetZ(int i, int j) const;
	void PutZ(int i, int j, float z);
	bool ptInTriangle(const glm::vec3& p, const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2);
	float CalcArea(const glm::vec3& v, const glm::vec3& u, const glm::vec3& w);
	float sign(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3);
	void FillZ_Buffer(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3,const int& colorMeth);
	
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
	int viewport_width_;
	int viewport_height_;
	GLuint gl_screen_tex_;
	GLuint gl_screen_vtc_;
};
