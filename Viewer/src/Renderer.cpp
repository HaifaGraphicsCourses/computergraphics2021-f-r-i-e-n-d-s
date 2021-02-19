#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>
#include "Renderer.h"
#include "InitShader.h"
#include <iostream>
#include <Light.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#define PHONGSHADING 999
#define WIREFRAME 990
#define MODEL_COLOR 900

#define INDEX(width,x,y,c) ((x)+(y)*(width))*3+(c)
#define Z_INDEX(width,x,y) ((x)+(y)*(width))

Renderer::Renderer(int viewport_width, int viewport_height) :
	viewport_width_(viewport_width),
	viewport_height_(viewport_height)
{
	//InitOpenGLRendering();
	CreateBuffers(viewport_width, viewport_height);
}

Renderer::~Renderer()
{
	delete[] color_buffer_;
	delete[] Z_Buffer;
}

void Renderer::PutPixel(int i, int j, const glm::vec3& color)
{
	if (i < 0) return; if (i >= viewport_width_) return;
	if (j < 0) return; if (j >= viewport_height_) return;

	color_buffer_[INDEX(viewport_width_, i, j, 0)] = color.x;
	color_buffer_[INDEX(viewport_width_, i, j, 1)] = color.y;
	color_buffer_[INDEX(viewport_width_, i, j, 2)] = color.z;
}

void Renderer::DrawLine(const glm::vec3& p1, const glm::vec3& p2, glm::vec3& color)
{
	// TODO: Implement bresenham algorithm
	int x = p1.x, y = p1.y, ReflectFlag = 0, LoopVar = p2.x;
	double deltaP = (double)(p2.x - p1.x), deltaQ = (double)(p2.y - p1.y);
	double a = deltaQ / deltaP, e = (-1) * deltaP;
	if (a == -1)
	{
		if (deltaP > 0)
		{
			while (x < p2.x)
			{
				PutPixel(x, y, color);
				x = x + 1;
				y = y - 1;
			}

		}
		else
		{
			while (x > p2.x)
			{
				PutPixel(x, y, color);
				x = x - 1;
				y = y + 1;
			}
		}
	}
	else
		if (deltaQ == 0)
		{
			if (deltaP > 0)
				while (x <= p2.x)
				{
					PutPixel(x, y, color);
					x = x + 1;
				}
			else
			{
				while (x >= p2.x)
				{
					PutPixel(x, y, color);
					x = x - 1;
				}
			}
		}
		else
			if (deltaP == 0)
			{
				if (deltaQ > 0)
					while (y <= p2.y)
					{
						PutPixel(x, y, color);
						y = y + 1;
					}
				else
					while (y >= p2.y)
					{
						PutPixel(x, y, color);
						y = y - 1;
					}
			}
			else
				if (a > 1 && !(a < -1)) //switch x and y
				{
					x = p1.y;
					y = p1.x;
					deltaQ = deltaP;
					deltaP = (p2.y - p1.y);
					if (deltaQ > 0 && deltaP > 0) {
						while (x <= p2.y)
						{
							if (e > 0)
							{
								y = y + 1;
								e = e - (2 * deltaP);
							}
							PutPixel(y, x, color);
							x = x + 1;
							e = e + (2 * deltaQ);
						}
					}
					else
						DrawLine(p2, p1, color);
				}
				else
					if ((a > -1 && a < 0) && !(a < -1)) //reflect 
					{
						if (deltaQ < 0) {
							deltaQ = (-1) * deltaQ;
							while (x <= p2.x)
							{
								if (e > 0)
								{
									y = y - 1;
									e = e - (2 * deltaP);
								}
								PutPixel(x, y, color);
								x = x + 1;
								e = e + (2 * deltaQ);
							}
						}
						else
						{
							deltaP = (-1) * deltaP;
							while (x >= p2.x)
							{
								if (e > 0)
								{
									if (y < p2.y)
										y = y + 1;
									e = e - (2 * deltaP);
								}
								PutPixel(x, y, color);
								x = x - 1;
								e = e + (2 * deltaQ);
							}
						}
					}
					else
						if (a < -1)//swtich and reflect
						{
							x = p1.y;
							y = p1.x;
							deltaQ = deltaP;
							deltaP = (p2.y - p1.y);
							LoopVar = p2.y;
							if (deltaQ < 0) {
								deltaP = (-1) * deltaP;
								while (x <= LoopVar)
								{
									if (e < 0)
									{
										y = y - 1;
										e = e - (2 * deltaP);
									}
									PutPixel(y, x, color);
									x = x + 1;
									e = e + (2 * deltaQ);
								}
							}
							else
								DrawLine(p2, p1, color);
						}
						else
						{
							if (deltaP > 0 && deltaQ > 0) {
								while (x <= p2.x)
								{
									if (e > 0)
									{
										y = y + 1;
										e = e - (2 * deltaP);
									}
									PutPixel(x, y, color);
									x = x + 1;
									e = e + (2 * deltaQ);
								}
							}
							else
								DrawLine(p2, p1, color);
						}
}

void Renderer::CreateBuffers(const int w, const int h)
{
	CreateOpenGLBuffer(); //Do not remove this line.
	color_buffer_ = new float[3 * w * h];
	Z_Buffer = new float[w * h];
	ClearColorBuffer(glm::vec3(0.0f, 0.0f, 0.0f));
	ClearZ_Buffer();
}

void Renderer::InitOpenGLRendering()
{
	// Creates a unique identifier for an opengl texture.
	glGenTextures(1, &gl_screen_tex_);

	// Same for vertex array object (VAO). VAO is a set of buffers that describe a renderable object.
	glGenVertexArrays(1, &gl_screen_vtc_);

	GLuint buffer;

	// Makes this VAO the current one.
	glBindVertexArray(gl_screen_vtc_);

	// Creates a unique identifier for a buffer.
	glGenBuffers(1, &buffer);

	// (-1, 1)____(1, 1)
	//	     |\  |
	//	     | \ | <--- The exture is drawn over two triangles that stretch over the screen.
	//	     |__\|
	// (-1,-1)    (1,-1)
	const GLfloat vtc[] = {
		-1, -1,
		 1, -1,
		-1,  1,
		-1,  1,
		 1, -1,
		 1,  1
	};

	const GLfloat tex[] = {
		0,0,
		1,0,
		0,1,
		0,1,
		1,0,
		1,1 };

	// Makes this buffer the current one.
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	// This is the opengl way for doing malloc on the gpu. 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vtc) + sizeof(tex), NULL, GL_STATIC_DRAW);

	// memcopy vtc to buffer[0,sizeof(vtc)-1]
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vtc), vtc);

	// memcopy tex to buffer[sizeof(vtc),sizeof(vtc)+sizeof(tex)]
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vtc), sizeof(tex), tex);

	// Loads and compiles a sheder.
	GLuint program = InitShader("vshader_color.glsl", "fshader_color.glsl");

	// Make this program the current one.
	glUseProgram(program);

	// Tells the shader where to look for the vertex position data, and the data dimensions.
	GLint  vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, 0);

	// Same for texture coordinates data.
	GLint  vTexCoord = glGetAttribLocation(program, "vTexCoord");
	glEnableVertexAttribArray(vTexCoord);
	glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)sizeof(vtc));

	//glProgramUniform1i( program, glGetUniformLocation(program, "texture"), 0 );

	// Tells the shader to use GL_TEXTURE0 as the texture id.
	glUniform1i(glGetUniformLocation(program, "texture"), 0);
}

void Renderer::CreateOpenGLBuffer()
{
	// Makes GL_TEXTURE0 the current active texture unit
	glActiveTexture(GL_TEXTURE0);

	// Makes glScreenTex (which was allocated earlier) the current texture.
	glBindTexture(GL_TEXTURE_2D, gl_screen_tex_);

	// malloc for a texture on the gpu.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, viewport_width_, viewport_height_, 0, GL_RGB, GL_FLOAT, NULL);
	glViewport(0, 0, viewport_width_, viewport_height_);
}

void Renderer::SwapBuffers()
{
	// Makes GL_TEXTURE0 the current active texture unit
	glActiveTexture(GL_TEXTURE0);

	// Makes glScreenTex (which was allocated earlier) the current texture.
	glBindTexture(GL_TEXTURE_2D, gl_screen_tex_);

	// memcopy's colorBuffer into the gpu.
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, viewport_width_, viewport_height_, GL_RGB, GL_FLOAT, color_buffer_);

	// Tells opengl to use mipmapping
	glGenerateMipmap(GL_TEXTURE_2D);

	// Make glScreenVtc current VAO
	glBindVertexArray(gl_screen_vtc_);

	// Finally renders the data.
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::ClearZ_Buffer()
{
	MinZ = FLT_MAX;
	MaxZ = FLT_MIN;
	for (int i = 0; i < viewport_width_; i++)
	{
		for (int j = 0; j < viewport_height_; j++)
		{
			if (i < 0) return; if (i >= viewport_width_) return;
			if (j < 0) return; if (j >= viewport_height_) return;
			Z_Buffer[Z_INDEX(viewport_width_, i, j)] = FLT_MAX;
			color_buffer_[INDEX(viewport_width_, i, j, 0)] = 0.f;
			color_buffer_[INDEX(viewport_width_, i, j, 1)] = 0.f;
			color_buffer_[INDEX(viewport_width_, i, j, 2)] = 0.f;
		}
	}
}

void Renderer::ClearColorBuffer(const glm::vec3& color)
{
	for (int i = 0; i < viewport_width_; i++)
	{
		for (int j = 0; j < viewport_height_; j++)
		{
			PutPixel(i, j, color);
		}
	}
}

void Renderer::Render(Scene& scene)
{
	int half_width = viewport_width_ / 2;
	int half_height = viewport_height_ / 2;
	glm::vec3 AmbientColor[10];
	glm::vec3 DiffuseColor[10];
	glm::vec3 SpecularColor[10];
	glm::vec3 lightsPositions[10];
	glm::mat4 lightTransformations[10];
	glm::vec3 lightsDirections[10];
	float lightsTypes[10];
	int LightsNumber;
	int cameraCount = scene.GetCameraCount();

	if (cameraCount > 0)
	{
		int modelCount = scene.GetModelCount();
		std::shared_ptr<Camera> camera = scene.GetActiveCamera();

		for (int currentModelIndex = 0; currentModelIndex < modelCount; currentModelIndex++)
		{
			std::shared_ptr<MeshModel> currentModel = scene.GetModel(currentModelIndex);

			// Activate the 'colorShader' program (vertex and fragment shaders)
			for (int i = 0; i < scene.GetLightCount(); i++)
			{
				std::shared_ptr<Light> currentLight = scene.GetLight(i);
				AmbientColor[i] = currentLight->GetAmbientLightColor();
				DiffuseColor[i] = currentLight->GetDiffuseLightColor();
				SpecularColor[i] = currentLight->GetSpecularLightColor();
				lightsPositions[i] = currentLight->GetLightPosition();
				lightTransformations[i] = currentLight->GetWorldTransformation() * currentLight->GetLocalTransformation();
				lightsDirections[i] = normalize(currentLight->GetLightDirection());
				if (currentLight->GetLightType() == LightType::PARALLEL)
					lightsTypes[i] = 1;
				else
					lightsTypes[i] = 0;
			}
			// Set the uniform variables
			colorShader.use();
			colorShader.setUniform("AmbientColor", AmbientColor, scene.GetLightCount());
			colorShader.setUniform("DiffuseColor", DiffuseColor, scene.GetLightCount());
			colorShader.setUniform("SpecularColor", SpecularColor, scene.GetLightCount());
			colorShader.setUniform("lightsPositions", lightsPositions, scene.GetLightCount());
			colorShader.setUniform("lightTransformations", lightTransformations, scene.GetLightCount());
			colorShader.setUniform("lightsTypes", lightsTypes, scene.GetLightCount());
			colorShader.setUniform("lightsDirections", lightsDirections, scene.GetLightCount());
			colorShader.setUniform("LightsNumber", scene.GetLightCount());
			colorShader.setUniform("eye",normalize(camera->GetEye()));

			colorShader.setUniform("model", currentModel->GetTransformation());
			colorShader.setUniform("DrawLight", false);
			colorShader.setUniform("view", camera->GetLookAt() * camera->GetC_inv());
			colorShader.setUniform("projection", camera->GetProjectionTransformation());
			colorShader.setUniform("material.textureMap", 0);
			colorShader.setUniform("material.diffuseColor", currentModel->GetDiffuseColor());
			colorShader.setUniform("material.specularColor", currentModel->GetSpecularColor());
			colorShader.setUniform("material.ambientColor", currentModel->GetAmbientColor());
			colorShader.setUniform("material.alpha", scene.GetActiveModel()->GetAlpha());

			// Set 'texture1' as the active texture at slot #0
			texture1.bind(0);

			
			if (scene.GetActiveModel()->GetColorMethod() == PHONGSHADING)
			{
				// Drag our model's faces (triangles) in fill mode
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glBindVertexArray(currentModel->GetVAO());
				glDrawArrays(GL_TRIANGLES, 0, currentModel->GetModelVertices().size());
				glBindVertexArray(0);
			}

			// Unset 'texture1' as the active texture at slot #0
			texture1.unbind(0);

			if (scene.GetActiveModel()->GetColorMethod() == WIREFRAME)
			{
				// Drag our model's faces (triangles) in line mode (wireframe)
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glBindVertexArray(currentModel->GetVAO());
				glDrawArrays(GL_TRIANGLES, 0, currentModel->GetModelVertices().size());
				glBindVertexArray(0);
			}
		}
		if (scene.GetLightCount())
			DrawLights(scene);
	}
			/*for (int i = 0; i < model.GetFacesCount(); i++)
			{
				Face face = model.GetFace(i);
				int VertexIndex1 = face.GetVertexIndex(0), VertexIndex2 = face.GetVertexIndex(1), VertexIndex3 = face.GetVertexIndex(2);
				glm::vec3 v1Temp = model.GetVertex(VertexIndex1);
				glm::vec3 v2Temp = model.GetVertex(VertexIndex2);
				glm::vec3 v3Temp = model.GetVertex(VertexIndex3);
				glm::vec3 faceNormal = normalize(cross((v1Temp - v2Temp), (v1Temp - v3Temp)));
				FaceNormal = model.Get_R_w() * model.Get_R_m() * glm::vec4(faceNormal, 1);
				int Nindex1 = face.GetNormalIndex(0), Nindex2 = face.GetNormalIndex(1), Nindex3 = face.GetNormalIndex(2);
				glm::vec4 v1 = projectionTransformation * Lookat * C_inv * Transformation * glm::vec4(model.GetVertex(VertexIndex1), 1);
				glm::vec4 v2 = projectionTransformation * Lookat * C_inv * Transformation * glm::vec4(model.GetVertex(VertexIndex2), 1);
				glm::vec4 v3 = projectionTransformation * Lookat * C_inv * Transformation * glm::vec4(model.GetVertex(VertexIndex3), 1);
				glm::vec4 origin = projectionTransformation * Lookat * C_inv * glm::vec4(0, 0, 0, 1);
				glm::vec4 temp = scene.GetActiveModel().GetRightTopFar();
				glm::vec4 XAxis = projectionTransformation * Lookat * C_inv * glm::vec4(5 * temp.x, 0, 0, 1);
				glm::vec4 YAxis = projectionTransformation * Lookat * C_inv * glm::vec4(0, 1.5 * temp.y, 0, 1);
				if (!scene.GetActiveCamera().GetIsOrthographic())
				{
					v1 /= v1.w;
					v2 /= v2.w;
					v3 /= v3.w;
					origin /= origin.w;
					XAxis /= XAxis.w;
					YAxis /= YAxis.w;
				}
				v1 = ViewPortTransformation * v1;
				v2 = ViewPortTransformation * v2;
				v3 = ViewPortTransformation * v3;
				glm::vec4 vn1 = model.Get_R_w() * model.Get_R_m() * (glm::vec4(model.GetNormals(Nindex1), 1));
				glm::vec4 vn2 = model.Get_R_w() * model.Get_R_m() * (glm::vec4(model.GetNormals(Nindex2), 1));
				glm::vec4 vn3 = model.Get_R_w() * model.Get_R_m() * (glm::vec4(model.GetNormals(Nindex3), 1));
				FaceCenter = (v1 + v2 + v3) / 3.0f;
				color = glm::vec3(0.f, 0.f, 0.f);
				c1 = glm::vec3(0.f, 0.f, 0.f);
				c2 = glm::vec3(0.f, 0.f, 0.f);
				c3 = glm::vec3(0.f, 0.f, 0.f);
				if (scene.GetActiveModel().GetColorMethod() == RANDOM_COLORED)
					ScanConvert_ZBuffer(v1, v2, v3, scene);
				for (int i = 0; i < scene.GetLightCount(); i++)
				{
					Light& light = scene.GetLight(i);
					glm::mat4x4 LightTransformations = light.GetWorldTransformation() * light.GetLocalTransformation();
					LightPosition = projectionTransformation * Lookat * C_inv * LightTransformations * light.GetLightPosition();
					if (!scene.GetActiveCamera().GetIsOrthographic())
						LightPosition /= LightPosition.w;
					LightPosition = ViewPortTransformation * LightPosition;
					if (scene.GetShadingtype() == ShadingType::FLAT)
					{
						if (light.GetLightType() == LightType::POINT)
							LightDirection = glm::normalize(glm::vec3(FaceCenter) - glm::vec3(LightPosition));
						else
							LightDirection = normalize(light.GetLightDirection());
						color += GetDiffuseColor(FaceNormal, LightDirection, scene, light);
						color += GetAmbientColor(scene.GetActiveModel().GetAmbientColor(), light.GetAmbientLightColor());
						color += GetSpecularColor(LightDirection, FaceNormal,scene.GetActiveCamera().GetEye(), light, scene.GetActiveModel().GetSpecularColor());
					}
					else if (scene.GetShadingtype() == ShadingType::GORAUD)
					{
						Lighting = true;
						glm::vec3 LightDirection1, LightDirection3, LightDirection2;
						if (light.GetLightType() == LightType::POINT)
						{
							LightDirection1 = glm::normalize(glm::vec3(v1.x, v1.y, v1.z) - glm::vec3(LightPosition.x, LightPosition.y, LightPosition.z));
							LightDirection2 = glm::normalize(glm::vec3(v2.x, v2.y, v2.z) - glm::vec3(LightPosition.x, LightPosition.y, LightPosition.z));
							LightDirection3 = glm::normalize(glm::vec3(v3.x, v3.y, v3.z) - glm::vec3(LightPosition.x, LightPosition.y, LightPosition.z));
						}
						else
							LightDirection1 = LightDirection2 = LightDirection3 = normalize(light.GetLightDirection());
						c1 = GetDiffuseColor((vn1), LightDirection1, scene, light);
						c1 += GetAmbientColor(scene.GetActiveModel().GetAmbientColor(), light.GetAmbientLightColor());
						c1 += GetSpecularColor(LightDirection1, vn1, scene.GetActiveCamera().GetEye(), light, scene.GetActiveModel().GetSpecularColor());
						c2 = GetDiffuseColor(vn2, LightDirection2, scene, light);
						c2 += GetAmbientColor(scene.GetActiveModel().GetAmbientColor(), light.GetAmbientLightColor());
						c2 += GetSpecularColor(LightDirection2, vn2, scene.GetActiveCamera().GetEye(), light, scene.GetActiveModel().GetSpecularColor());
						c3 = GetDiffuseColor(vn3, LightDirection3, scene, light);
						c3 += GetAmbientColor(scene.GetActiveModel().GetAmbientColor(), light.GetAmbientLightColor());
						c3 += GetSpecularColor(LightDirection3, vn3, scene.GetActiveCamera().GetEye(), light, scene.GetActiveModel().GetSpecularColor());
						ScanConvert_Gouraud(v1, v2, v3, c1, c2, c3);
					}
					else if (scene.GetShadingtype() == ShadingType::PHONG)
					{
						Lighting = true;
						ScanConvert_Phong(v1, v2, v3, (vn1), (vn2), (vn3), scene, GetAmbientColor(scene.GetActiveModel().GetAmbientColor(), light.GetAmbientLightColor()), LightPosition, light);
					}
				}
				if ((scene.GetShadingtype() == ShadingType::FLAT || scene.GetActiveModel().GetColorMethod() == GRAYSCALE) && scene.GetActiveModel().GetColorMethod() != RANDOM_COLORED)
					ScanConvert_Flat(v1, v2, v3, scene.GetActiveModel().GetColorMethod(), color, scene, Lighting);

				//Draw X and Y Axes
				origin = ViewPortTransformation * origin;
				XAxis = ViewPortTransformation * XAxis;
				YAxis = ViewPortTransformation * YAxis;
				DrawLine(glm::vec3(origin.x, origin.y, origin.z), glm::vec3(XAxis.x, XAxis.y, XAxis.z), glm::vec3(255, 255, 255));
				DrawLine(glm::vec3(origin.x, origin.y, origin.z), glm::vec3(YAxis.x, YAxis.y, YAxis.z), glm::vec3(255, 50, 0));
				//Draw the normal per vertex
				if (model.GetNormalsFlag())
				{
					vn1 = Transformations::ScalingTransformation(100, 100, 100) * vn1;
					vn2 = Transformations::ScalingTransformation(100, 100, 100) * vn2;
					vn3 = Transformations::ScalingTransformation(100, 100, 100) * vn3;
					DrawLine(glm::vec3(v1.x, v1.y, v1.z), glm::vec3(vn1.x + v1.x, vn1.y + v1.y, vn1.z + vn2.z), model.GetVN());
					DrawLine(glm::vec3(v2.x, v2.y, v2.z), glm::vec3(vn2.x + v2.x, vn2.y + v2.y, vn1.z + vn2.z), model.GetVN());
					DrawLine(glm::vec3(v3.x, v3.y, v3.z), glm::vec3(vn3.x + v3.x, vn3.y + v3.y, vn1.z + vn2.z), model.GetVN());
				}
				//Draw normals per face
				FaceNormal = Transformations::ScalingTransformation(100, 100, 100) * FaceNormal + glm::vec4(FaceCenter, 0);
				if (model.GetFacesNormalsFlag())
					DrawLine(glm::vec3(FaceCenter.x, FaceCenter.y, FaceCenter.z), glm::vec3(FaceNormal.x, FaceNormal.y, FaceNormal.z), model.GetFN());
			}
			//Draw the bounding box
			if (model.GetBoundingBoxFlag())
			{
				glm::vec4 leftTopNear = projectionTransformation * Lookat * C_inv * Transformation * model.GetLeftTopNear();
				glm::vec4 rightTopNear = projectionTransformation * Lookat * C_inv * Transformation * model.GetRightTopNear();
				glm::vec4 leftTopFar = projectionTransformation * Lookat * C_inv * Transformation * model.GetLeftTopFar();
				glm::vec4 rightTopFar = projectionTransformation * Lookat * C_inv * Transformation * model.GetRightTopFar();
				glm::vec4 leftBottomNear = projectionTransformation * Lookat * C_inv * Transformation * model.GetLeftBottomNear();
				glm::vec4 leftBottomFar = projectionTransformation * Lookat * C_inv * Transformation * model.GetLeftBottomFar();
				glm::vec4 rightBottomFar = projectionTransformation * Lookat * C_inv * Transformation * model.GetRightBottomFar();
				glm::vec4 rightBottomNear = projectionTransformation * Lookat * C_inv * Transformation * model.GetRightBottomNear();
				if (!scene.GetActiveCamera().GetIsOrthographic())
				{
					leftTopNear /= leftTopNear.w;
					rightTopNear /= rightTopNear.w;
					leftTopFar /= leftTopFar.w;
					rightTopFar /= rightTopFar.w;
					leftBottomNear /= leftBottomNear.w;
					leftBottomFar /= leftBottomFar.w;
					rightBottomFar /= rightBottomFar.w;
					rightBottomNear /= rightBottomNear.w;
				}
				leftTopNear = ViewPortTransformation * leftTopNear;
				rightTopNear = ViewPortTransformation * rightTopNear;
				leftTopFar = ViewPortTransformation * leftTopFar;
				rightTopFar = ViewPortTransformation * rightTopFar;
				leftBottomNear = ViewPortTransformation * leftBottomNear;
				leftBottomFar = ViewPortTransformation * leftBottomFar;
				rightBottomFar = ViewPortTransformation * rightBottomFar;
				rightBottomNear = ViewPortTransformation * rightBottomNear;
				DrawLine(glm::vec3(leftTopNear.x, leftTopNear.y, leftTopNear.z), glm::vec3(rightTopNear.x, rightTopNear.y, 0), model.GetBB());
				DrawLine(glm::vec3(leftTopNear.x, leftTopNear.y, leftTopNear.z), glm::vec3(leftTopFar.x, leftTopFar.y, leftTopFar.z), model.GetBB());
				DrawLine(glm::vec3(leftTopFar.x, leftTopFar.y, leftTopFar.z), glm::vec3(rightTopFar.x, rightTopFar.y, rightTopFar.z), model.GetBB());
				DrawLine(glm::vec3(rightTopFar.x, rightTopFar.y, rightTopFar.z), glm::vec3(rightTopNear.x, rightTopNear.y, 0), model.GetBB());
				DrawLine(glm::vec3(leftTopNear.x, leftTopNear.y, leftTopNear.z), glm::vec3(leftBottomNear.x, leftBottomNear.y, 0), model.GetBB());
				DrawLine(glm::vec3(leftTopFar.x, leftTopFar.y, leftTopFar.z), glm::vec3(leftBottomFar.x, leftBottomFar.y, leftBottomFar.z), model.GetBB());
				DrawLine(glm::vec3(rightTopFar.x, rightTopFar.y, rightTopFar.z), glm::vec3(rightBottomFar.x, rightBottomFar.y, rightBottomFar.z), model.GetBB());
				DrawLine(glm::vec3(rightTopNear.x, rightTopNear.y, rightTopNear.z), glm::vec3(rightBottomNear.x, rightBottomNear.y, rightBottomNear.z), model.GetBB());
				DrawLine(glm::vec3(leftBottomNear.x, leftBottomNear.y, leftBottomNear.z), glm::vec3(rightBottomNear.x, rightBottomNear.y, rightBottomNear.z), model.GetBB());
				DrawLine(glm::vec3(leftBottomNear.x, leftBottomNear.y, leftBottomNear.z), glm::vec3(leftBottomFar.x, leftBottomFar.y, leftBottomFar.z), model.GetBB());
				DrawLine(glm::vec3(leftBottomFar.x, leftBottomFar.y, leftBottomFar.z), glm::vec3(rightBottomFar.x, rightBottomFar.y, rightBottomFar.z), model.GetBB());
				DrawLine(glm::vec3(rightBottomFar.x, rightBottomFar.y, rightBottomFar.z), glm::vec3(rightBottomNear.x, rightBottomNear.y, rightBottomNear.z), model.GetBB());
			}
		}
		FixColors(scene.GetColoring());
		
		if (scene.GetActiveModel().GetColorMethod() == GRAYSCALE)
			ScanConvert_Grayscale();
		if (scene.GetFog())
			FogExists(scene);
	}*/
}

int Renderer::GetViewportWidth() const
{
	return viewport_width_;
}

int Renderer::GetViewportHeight() const
{
	return viewport_height_;
}

void Renderer::SetViewportWidth(int w)
{
	viewport_width_ = w;
}

void Renderer::SetViewportHeight(int h)
{
	viewport_height_ = h;
}

glm::vec3 Renderer::RandColor()
{
	return glm::vec3(static_cast <float> (rand() / static_cast <float>(RAND_MAX)), static_cast <float> (rand() / static_cast <float>(RAND_MAX)), static_cast <float> (rand() / static_cast <float>(RAND_MAX)));
}

void Renderer::FogExists(Scene& scene)
{
	scene.SetFogStart(MinZ);
	scene.SetFogEnd(MaxZ);
	int half_width = viewport_width_ / 2;
	int half_height = viewport_height_ / 2;
	glm::vec3 c;
	for (int i = 0; i < viewport_width_; i++)
		for (int j = 0; j < viewport_height_; j++)
		{
			float z = Z_Buffer[Z_INDEX(viewport_width_, i, j)];
			if (z != FLT_MAX)
			{
				c = glm::vec3(color_buffer_[INDEX(viewport_width_, i, j, 0)], color_buffer_[INDEX(viewport_width_, i, j, 1)], color_buffer_[INDEX(viewport_width_, i, j, 2)]);
				float vertexViewDistance = z;
				float fogFactor;
				if (scene.GetIsLinearFog())
				{
					if (!scene.GetActiveCamera()->GetIsOrthographic())
						int x = 3;
					fogFactor = (scene.GetFogEnd() - vertexViewDistance) / (scene.GetFogEnd() - scene.GetFogStart());
				}
				else
				{
					fogFactor = std::exp(-(vertexViewDistance * vertexViewDistance * scene.GetFogDensity() * scene.GetFogDensity()));
					if (fogFactor < 0 || fogFactor > 1)
						fogFactor = 1;
				}
				c = ((1 - fogFactor) * glm::vec3(0.4, 0.4, 0.4) + fogFactor * c);
				color_buffer_[INDEX(viewport_width_, i, j, 0)] = c.x;
				color_buffer_[INDEX(viewport_width_, i, j, 1)] = c.y;
				color_buffer_[INDEX(viewport_width_, i, j, 2)] = c.z;
			}
		}
}

float Renderer::CalcArea(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3)
{
	return abs(((v2.x - v1.x) * (v3.y - v1.y) - (v3.x - v1.x) * (v2.y - v1.y)) / 2.0f);
}

glm::vec3 Renderer::CalcZ(glm::vec3& P, const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3, const glm::vec3& value1, const glm::vec3& value2, const glm::vec3& value3)
{
	float A;
	float A1 = CalcArea(P, v1, v2);
	float A2 = CalcArea(P, v1, v3);
	float A3 = CalcArea(P, v2, v3);
	A = A1 + A2 + A3;
	return ((A1 / A) * value3) + ((A2 / A) * value2) + ((A3 / A) * value1);
}

float Renderer::sign(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3)
{
	return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

bool Renderer::ptInTriangle(const glm::vec3& pt, const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3) {
	float d1, d2, d3;
	bool has_neg, has_pos;

	d1 = sign(pt, v1, v2);
	d2 = sign(pt, v2, v3);
	d3 = sign(pt, v3, v1);
	has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
	has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);
	return !(has_neg && has_pos);
}

float Renderer::GetZ(int i, int j) const
{
	if (i < 0) return FLT_MAX; if (i >= viewport_width_) return FLT_MAX;
	if (j < 0) return FLT_MAX; if (j >= viewport_height_) return FLT_MAX;
	return Z_Buffer[Z_INDEX(viewport_width_, i, j)];
}

void Renderer::PutZ(int i, int j, float z)
{
	if (i < 0) return; if (i >= viewport_width_) return;
	if (j < 0) return; if (j >= viewport_height_) return;
	this->Z_Buffer[Z_INDEX(viewport_width_, i, j)] = z;
}

void Renderer::DrawLights(Scene& scene)
{
	float half_width = viewport_width_ / 2;
	float half_height = viewport_height_ / 2;
	glm::vec3 color = glm::vec3(1, 1, 1);
	glm::mat4x4 Lookat = scene.GetActiveCamera()->GetLookAt();
	glm::mat4x4 projectionTransformation = scene.GetActiveCamera()->GetProjectionTransformation();
	glm::mat4x4 ViewPortTransformation = Transformations::ScalingTransformation(half_width, half_height, 1) * Transformations::TranslationTransformation(1, 1, 1);
	glm::mat4x4 C_inv = scene.GetActiveCamera()->GetC_inv();
	for (int i = 0; i < scene.GetLightCount(); i++)
	{
		std::shared_ptr<Light> light = scene.GetLight(i);
		glm::mat4x4 transformation = light->GetWorldTransformation() * light->GetLocalTransformation();
		colorShader.use();

		// Set the uniform variables
		colorShader.setUniform("LightTransformation", transformation);
		colorShader.setUniform("DrawLight", true);
		if (light->GetLightType() == LightType::PARALLEL)
			colorShader.setUniform("lightType", 1);
		else
			colorShader.setUniform("lightType", 0);
		colorShader.setUniform("view", Lookat * C_inv);
		colorShader.setUniform("projection", projectionTransformation);
		colorShader.setUniform("material.textureMap", 0);
		if (light->GetLightType() == LightType::POINT)
		{
			glm::vec3 position = light->GetLightPosition();
			// Drag our model's faces (triangles) in fill mode
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glBindVertexArray(light->GetVao());
			glDrawArrays(GL_POINTS, 0, 1);
			glEnable(GL_PROGRAM_POINT_SIZE);
			glBindVertexArray(0);
		}
		else
		{
			glBindVertexArray(light->GetVao());
			glDrawArrays(GL_LINES, 0, 18);

			//glBindVertexArray(light->GetVao());
			//glDrawArrays(GL_LINES, 0, 8);
			glBindVertexArray(0);
		}
	}
}

glm::vec3 Renderer::GetAmbientColor(const glm::vec3& Acolor, const glm::vec3& LightAcolor)
{
	glm::vec3 I_a(Acolor.x * LightAcolor.x, Acolor.y * LightAcolor.y, Acolor.z * LightAcolor.z);
	return I_a;
}

glm::vec3 Renderer::GetSpecularColor(glm::vec3& I, glm::vec3 n, const glm::vec3& eye, Light& light, const glm::vec3& Scolor)
{
	glm::vec3 temp_eye = normalize(eye);
	int alpha = 2;
	glm::vec3 temp = glm::vec3(Scolor.x * light.GetSpecularLightColor().x, Scolor.y * light.GetSpecularLightColor().y, Scolor.z * light.GetSpecularLightColor().z);
	glm::vec3 r = (2.f * glm::dot(-n, I) * n - I);
	float Power = (float)(std::pow(std::max(0.0f, glm::dot((r), (temp_eye))), alpha));
	glm::vec3 I_s(temp * Power);
	return I_s;
}

glm::vec3 Renderer::GetDiffuseColor(glm::vec3 normal, glm::vec3 I, Scene& scene,Light& light)
{
	glm::vec3 Dcolor = scene.GetActiveModel()->GetDiffuseColor();
	glm::vec3 temp(Dcolor.x * light.GetDiffuseLightColor().x, Dcolor.y * light.GetDiffuseLightColor().y, Dcolor.z * light.GetDiffuseLightColor().z);
	float IdotN = glm::dot(-(normal), I);
	return temp * IdotN;
}


void Renderer::ScanConvert_Grayscale()
{
	for (int i = 0; i < viewport_width_; i++)
		for (int j = 0; j < viewport_height_; j++)
		{
			float z = Z_Buffer[Z_INDEX(viewport_width_, i, j)];
			if (z != FLT_MAX)
			{
				float a = 1 / (MaxZ - MinZ);
				float b = -1 * a * MinZ;
				float gray = 1 - (a * z + b);
				color_buffer_[INDEX(viewport_width_, i, j, 0)] = gray;
				color_buffer_[INDEX(viewport_width_, i, j, 1)] = gray;
				color_buffer_[INDEX(viewport_width_, i, j, 2)] = gray;
			}
		}
}

void Renderer::FixColors(int coloring)
{
	for (int i = 0; i < viewport_width_; i++)
		for (int j = 0; j < viewport_height_; j++)
		{
			float z = Z_Buffer[Z_INDEX(viewport_width_, i, j)];
			if (z != FLT_MAX)
			{
				if (coloring)
				{
					if (color_buffer_[INDEX(viewport_width_, i, j, 0)] > 1.f) color_buffer_[INDEX(viewport_width_, i, j, 0)] = 1.f;
					if (color_buffer_[INDEX(viewport_width_, i, j, 1)] > 1.f) color_buffer_[INDEX(viewport_width_, i, j, 1)] = 1.f;
					if (color_buffer_[INDEX(viewport_width_, i, j, 2)] > 1.f) color_buffer_[INDEX(viewport_width_, i, j, 2)] = 1.f;
				}
				else
				{
					color_buffer_[INDEX(viewport_width_, i, j, 0)] /= 5;
					color_buffer_[INDEX(viewport_width_, i, j, 1)] /= 5;
					color_buffer_[INDEX(viewport_width_, i, j, 2)] /= 5;
				}
			}
		}
}

void Renderer::LoadShaders()
{
	colorShader.loadShaders("vshader_color.glsl", "fshader_color.glsl");
}

void Renderer::LoadTextures()
{
	if (!texture1.loadTexture("C:\\Users\\most_\\OneDrive\\Documents\\GitHub\\computergraphics2021-f-r-i-e-n-d-s\\Data\\armadillo.jpg", true))
	{
		texture1.loadTexture("C:\\Users\\most_\\OneDrive\\Documents\\GitHub\\computergraphics2021-f-r-i-e-n-d-s\\Data\\armadillo.jpg", true);
	}
}