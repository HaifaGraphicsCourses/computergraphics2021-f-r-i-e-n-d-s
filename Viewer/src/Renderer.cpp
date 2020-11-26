#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>
#include "Renderer.h"
#include "InitShader.h"
#include <iostream>

#define INDEX(width,x,y,c) ((x)+(y)*(width))*3+(c)
#define Z_INDEX(width,x,y) ((x)+(y)*(width))
Renderer::Renderer(int viewport_width, int viewport_height) :
	viewport_width_(viewport_width),
	viewport_height_(viewport_height)
{
	InitOpenGLRendering();
	CreateBuffers(viewport_width, viewport_height);
}

Renderer::~Renderer()
{
	delete[] color_buffer_;
}

void Renderer::PutPixel(int i, int j, const glm::vec3& color)
{
	if (i < 0) return; if (i >= viewport_width_) return;
	if (j < 0) return; if (j >= viewport_height_) return;
	
	color_buffer_[INDEX(viewport_width_, i, j, 0)] = color.x;
	color_buffer_[INDEX(viewport_width_, i, j, 1)] = color.y;
	color_buffer_[INDEX(viewport_width_, i, j, 2)] = color.z;
}

void Renderer::DrawLine(const glm::ivec2& p1, const glm::ivec2& p2,glm::vec3& color)
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
		if(deltaP>0)
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
	}else
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

void Renderer::CreateBuffers(int w, int h)
{
	CreateOpenGLBuffer(); //Do not remove this line.
	color_buffer_ = new float[3 * w * h];
	ClearColorBuffer(glm::vec3(0.0f, 0.0f, 0.0f));
}

//##############################
//##OpenGL stuff. Don't touch.##
//##############################

// Basic tutorial on how opengl works:
// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/
// don't linger here for now, we will have a few tutorials about opengl later.
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
	const GLfloat vtc[]={
		-1, -1,
		 1, -1,
		-1,  1,
		-1,  1,
		 1, -1,
		 1,  1
	};

	const GLfloat tex[]={
		0,0,
		1,0,
		0,1,
		0,1,
		1,0,
		1,1};

	// Makes this buffer the current one.
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	// This is the opengl way for doing malloc on the gpu. 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vtc)+sizeof(tex), NULL, GL_STATIC_DRAW);

	// memcopy vtc to buffer[0,sizeof(vtc)-1]
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vtc), vtc);

	// memcopy tex to buffer[sizeof(vtc),sizeof(vtc)+sizeof(tex)]
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vtc), sizeof(tex), tex);

	// Loads and compiles a sheder.
	GLuint program = InitShader( "vshader.glsl", "fshader.glsl" );

	// Make this program the current one.
	glUseProgram(program);

	// Tells the shader where to look for the vertex position data, and the data dimensions.
	GLint  vPosition = glGetAttribLocation( program, "vPosition" );
	glEnableVertexAttribArray( vPosition );
	glVertexAttribPointer( vPosition,2,GL_FLOAT,GL_FALSE,0,0 );

	// Same for texture coordinates data.
	GLint  vTexCoord = glGetAttribLocation( program, "vTexCoord" );
	glEnableVertexAttribArray( vTexCoord );
	glVertexAttribPointer( vTexCoord,2,GL_FLOAT,GL_FALSE,0,(GLvoid *)sizeof(vtc) );

	//glProgramUniform1i( program, glGetUniformLocation(program, "texture"), 0 );

	// Tells the shader to use GL_TEXTURE0 as the texture id.
	glUniform1i(glGetUniformLocation(program, "texture"),0);
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
	// TODO: Replace this code with real scene rendering code
	int half_width = viewport_width_/2 ;
	int half_height = viewport_height_/2 ;
	int thickness = 15;
	int VertexIndex1, VertexIndex2, VertexIndex3;
	std::vector<int> VerticesIndices;
	glm::vec3 Vertex;
	glm::vec4 Vertex1, Vertex2, Vertex3;
	if (scene.GetModelCount() > 0) {
		auto model = scene.GetActiveModel();
		glm::mat4x4 Transformation = model.GetTransformation();
		glm::mat4x4 ViewPortTransformation = Transformations::ScalingTransformation(half_width, half_height, 1) * Transformations::TranslationTransformation(1, 1, 0);
		glm::mat4x4 Lookat = scene.GetActiveCamera().GetLookAt();
		//glm::mat4x4 viewTransformation = scene.GetActiveCamera().GetViewTransformation();
		glm::mat4x4 projectionTransformation =(scene.GetActiveCamera().GetProjectionTransformation());
		//glm::mat4x4 graphicPipelineMat = projectionTransformation *viewTransformation * scene.GetActiveCamera().GetTransformation();
		//scene.GetActiveCamera().SetCameraPosition(graphicPipelineMat * glm::vec4(scene.GetActiveCamera().GetAt(), 1));
		if (model.GetFacesNormalsFlag())
		{
			for (int faceIndex = 0; faceIndex < model.GetFacesCount(); ++faceIndex)
			{
				Face face = model.GetFace(faceIndex);
				int VertexIndex1 = face.GetVertexIndex(0);
				int VertexIndex2 = face.GetVertexIndex(1);
				int VertexIndex3 = face.GetVertexIndex(2);
				glm::vec3 v1Temp = model.GetVertex(VertexIndex1);
				glm::vec3 v2Temp = model.GetVertex(VertexIndex2);
				glm::vec3 v3Temp = model.GetVertex(VertexIndex3);
				glm::vec3 faceNormal = normalize(cross(glm::vec3(v1Temp - v2Temp), glm::vec3(v1Temp - v3Temp)));
				glm::vec4 v1 =projectionTransformation * Lookat * Transformation * glm::vec4(model.GetVertex(VertexIndex1), 1);
				glm::vec4 v2 =projectionTransformation * Lookat * Transformation * glm::vec4(model.GetVertex(VertexIndex2), 1);
				glm::vec4 v3 =projectionTransformation * Lookat * Transformation * glm::vec4(model.GetVertex(VertexIndex3), 1);
				if (!scene.GetActiveCamera().GetIsOrthographic())
				{
					v1 /= v1.w;
					v2 /= v2.w;
					v3 /= v3.w;
				}
				v1 = ViewPortTransformation * v1;
				v2 = ViewPortTransformation * v2;
				v3 = ViewPortTransformation * v3;
				glm::vec3 FaceCenter = (v1 + v2 + v3) / 3.0f;
				glm::vec4 FaceNormal = Transformations::ScalingTransformation(100,100,100) * model.Get_R_w() * model.Get_R_m()* glm::vec4(faceNormal, 1) + glm::vec4(FaceCenter, 0);
				DrawLine(glm::ivec2(FaceCenter.x, FaceCenter.y), glm::ivec2(FaceNormal.x, FaceNormal.y), model.GetFN());
			}
		}
		// draw the faces normals
		if (model.GetNormalsFlag())
		{
			for (int i = 0; i < model.GetFacesCount(); i++)
			{
				Face face = model.GetFace(i);
				int VertexIndex1 = face.GetVertexIndex(0), VertexIndex2 = face.GetVertexIndex(1), VertexIndex3 = face.GetVertexIndex(2);
				int Nindex1 = face.GetNormalIndex(0), Nindex2 = face.GetNormalIndex(1), Nindex3 = face.GetNormalIndex(2);
				glm::vec4 v1=projectionTransformation * Lookat * Transformation * glm::vec4(model.GetVertex(VertexIndex1), 1);
				glm::vec4 v2=projectionTransformation * Lookat * Transformation * glm::vec4(model.GetVertex(VertexIndex2), 1);
				glm::vec4 v3=projectionTransformation * Lookat * Transformation * glm::vec4(model.GetVertex(VertexIndex3), 1);
				if (!scene.GetActiveCamera().GetIsOrthographic())
				{
					v1 /= v1.w;
					v2 /= v2.w;
					v3 /= v3.w;
				}
				v1 = ViewPortTransformation * v1;
				v2 = ViewPortTransformation * v2;
				v3 = ViewPortTransformation * v3;
				glm::vec4 vn1 = Transformations::ScalingTransformation(100,100,100) * model.Get_R_w() * model.Get_R_m() * glm::vec4(model.GetNormals(Nindex1),1);
				glm::vec4 vn2 = Transformations::ScalingTransformation(100,100,100) * model.Get_R_w() * model.Get_R_m() * glm::vec4(model.GetNormals(Nindex2),1);
				glm::vec4 vn3 = Transformations::ScalingTransformation(100,100,100) * model.Get_R_w() * model.Get_R_m() * glm::vec4(model.GetNormals(Nindex3),1);
				DrawLine(glm::ivec2(v1.x, v1.y), glm::ivec2(vn1.x + v1.x, vn1.y + v1.y), model.GetVN());
				DrawLine(glm::ivec2(v2.x, v2.y), glm::ivec2(vn2.x + v2.x, vn2.y + v2.y), model.GetVN());
				DrawLine(glm::ivec2(v3.x, v3.y), glm::ivec2(vn3.x + v3.x, vn3.y + v3.y), model.GetVN());
			}
		}
		// draw normals
		for (int i = 0; i < model.GetFacesCount(); i++)
		{
			Face face = model.GetFace(i);
			VertexIndex1 = face.GetVertexIndex(0);
			VertexIndex2 = face.GetVertexIndex(1);
			VertexIndex3 = face.GetVertexIndex(2);
			Vertex = model.GetVertex(VertexIndex1);
			glm::mat4x4 Transformation = model.GetTransformation();
			Vertex1 = projectionTransformation * Lookat * Transformation * glm::vec4(Vertex.x, Vertex.y, Vertex.z, 1);
			Vertex = model.GetVertex(VertexIndex2);
			Vertex2 =projectionTransformation * Lookat * Transformation * glm::vec4(Vertex.x, Vertex.y, Vertex.z, 1);
			Vertex = model.GetVertex(VertexIndex3);
			Vertex3 =projectionTransformation * Lookat * Transformation * glm::vec4(Vertex.x, Vertex.y, Vertex.z, 1);
			if (!scene.GetActiveCamera().GetIsOrthographic())
			{
				Vertex1 /= Vertex1.w;
				Vertex2 /= Vertex2.w;
				Vertex3 /= Vertex3.w;
			}
			Vertex1 = ViewPortTransformation * Vertex1;
			Vertex2 = ViewPortTransformation * Vertex2;
			Vertex3 = ViewPortTransformation * Vertex3;
			DrawLine(glm::ivec2(Vertex1.x , Vertex1.y), glm::ivec2(Vertex2.x , Vertex2.y), model.GetMC());
			DrawLine(glm::ivec2(Vertex1.x , Vertex1.y), glm::ivec2(Vertex3.x , Vertex3.y), model.GetMC());
			DrawLine(glm::ivec2(Vertex3.x , Vertex3.y), glm::ivec2(Vertex2.x , Vertex2.y), model.GetMC());
		}
		// draw the model
		
		if (model.GetBoundingBoxFlag())
		{
			glm::vec4 leftTopNear     = projectionTransformation * Transformation * model.GetLeftTopNear();
			glm::vec4 rightTopNear    = projectionTransformation * Transformation * model.GetRightTopNear();
			glm::vec4 leftTopFar      = projectionTransformation * Transformation * model.GetLeftTopFar();
			glm::vec4 rightTopFar     = projectionTransformation * Transformation * model.GetRightTopFar();
			glm::vec4 leftBottomNear  = projectionTransformation * Transformation * model.GetLeftBottomNear();
			glm::vec4 leftBottomFar   = projectionTransformation * Transformation * model.GetLeftBottomFar();
			glm::vec4 rightBottomFar  = projectionTransformation * Transformation * model.GetRightBottomFar();
			glm::vec4 rightBottomNear = projectionTransformation * Transformation * model.GetRightBottomNear();
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
			leftTopNear            = ViewPortTransformation* leftTopNear;
			rightTopNear		   = ViewPortTransformation * rightTopNear;
			leftTopFar			   = ViewPortTransformation * leftTopFar;
			rightTopFar			   = ViewPortTransformation * rightTopFar;
			leftBottomNear		   = ViewPortTransformation * leftBottomNear;
			leftBottomFar		   = ViewPortTransformation * leftBottomFar	;
			rightBottomFar		   = ViewPortTransformation * rightBottomFar;
			rightBottomNear		   = ViewPortTransformation * rightBottomNear;
			DrawLine(glm::vec4(leftTopNear.x, leftTopNear.y, 0, 0), glm::vec4(rightTopNear.x, rightTopNear.y , 0, 0), model.GetBB());
			DrawLine(glm::vec4(leftTopNear.x, leftTopNear.y, 0, 0), glm::vec4(leftTopFar.x, leftTopFar.y, 0, 0), model.GetBB());
			DrawLine(glm::vec4(leftTopFar.x,  leftTopFar.y, 0, 0), glm::vec4(rightTopFar.x, rightTopFar.y, 0, 0), model.GetBB());
			DrawLine( glm::vec4(rightTopFar.x, rightTopFar.y, 0, 0), glm::vec4(rightTopNear.x, rightTopNear.y, 0, 0), model.GetBB());
			DrawLine( glm::vec4(leftTopNear.x, leftTopNear.y, 0, 0), glm::vec4(leftBottomNear.x, leftBottomNear.y, 0, 0), model.GetBB());
			DrawLine( glm::vec4(leftTopFar.x, leftTopFar.y, 0, 0), glm::vec4(leftBottomFar.x, leftBottomFar.y, 0, 0), model.GetBB());
			DrawLine( glm::vec4(rightTopFar.x, rightTopFar.y, 0, 0), glm::vec4(rightBottomFar.x, rightBottomFar.y, 0, 0), model.GetBB());
			DrawLine( glm::vec4(rightTopNear.x, rightTopNear.y, 0, 0), glm::vec4(rightBottomNear.x, rightBottomNear.y, 0, 0), model.GetBB());
			DrawLine( glm::vec4(leftBottomNear.x, leftBottomNear.y, 0, 0), glm::vec4(rightBottomNear.x, rightBottomNear.y, 0, 0), model.GetBB());
			DrawLine( glm::vec4(leftBottomNear.x, leftBottomNear.y, 0, 0), glm::vec4(leftBottomFar.x, leftBottomFar.y, 0, 0), model.GetBB());
			DrawLine( glm::vec4(leftBottomFar.x, leftBottomFar.y, 0, 0), glm::vec4(rightBottomFar.x, rightBottomFar.y, 0, 0), model.GetBB());
			DrawLine( glm::vec4(rightBottomFar.x, rightBottomFar.y, 0, 0), glm::vec4(rightBottomNear.x, rightBottomNear.y, 0, 0), model.GetBB());
		}
		// draw the bounding box
	}
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