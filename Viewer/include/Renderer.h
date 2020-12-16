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
	void DrawTriangle(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3,glm::vec3 color);
	void ClearZ_Buffer();
	glm::vec3 CalcZ(int i, int  j, const glm::vec3& p1, const glm::vec3& p2);
	float GetZ(int i, int j) const;
	void PutZ(int i, int j, float z);
	void IntersectionLines(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3, int y, bool& L12, bool& L13, bool& L23);
	
private:
	void PutPixel(const int i, const int j, const glm::vec3& color);
	std::vector<glm::ivec2> DrawLine(const glm::ivec2& p1, const glm::ivec2& p2,glm::vec3& color);
	void CreateBuffers(int w, int h);
	void CreateOpenGLBuffer();
	void InitOpenGLRendering();
	float* color_buffer_;
	float* Z_Buffer;
	int viewport_width_;
	int viewport_height_;
	GLuint gl_screen_tex_;
	GLuint gl_screen_vtc_;
};
