#define _USE_MATH_DEFINES
#include <cmath>
#include <imgui/imgui.h>
#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <nfd.h>
#include <string>
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Renderer.h"
#include "Scene.h"
#include "Utils.h"
#include <fstream>
#include <iostream>

/**
 * Fields
 */
bool show_demo_window = false;
bool show_another_window = false;
glm::vec4 clear_color = glm::vec4(0.8f, 0.8f, 0.8f, 1.00f);

/**
 * Function declarations
 */
static void GlfwErrorCallback(int error, const char* description);
GLFWwindow* SetupGlfwWindow(int w, int h, const char* window_name);
ImGuiIO& SetupDearImgui(GLFWwindow* window);
void StartFrame();
void RenderFrame(GLFWwindow* window, Scene& scene, Renderer& renderer, ImGuiIO& io);
void Cleanup(GLFWwindow* window);
void DrawImguiMenus(ImGuiIO& io, Scene& scene);

void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
	// TODO: Handle mouse scroll here
}

int main(int argc, char **argv)
{
	int windowWidth = 1280, windowHeight = 720;
	GLFWwindow* window = SetupGlfwWindow(windowWidth, windowHeight, "Mesh Viewer");
	if (!window)
		return 1;

	int frameBufferWidth, frameBufferHeight;
	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);

	Renderer renderer = Renderer(frameBufferWidth, frameBufferHeight);
	Scene scene = Scene();
	ImGuiIO& io = SetupDearImgui(window);
	glfwSetScrollCallback(window, ScrollCallback);
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
		StartFrame();
		DrawImguiMenus(io, scene);
		RenderFrame(window, scene, renderer, io);
    }

	Cleanup(window);
    return 0;
}

static void GlfwErrorCallback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

GLFWwindow* SetupGlfwWindow(int w, int h, const char* window_name)
{
	glfwSetErrorCallback(GlfwErrorCallback);
	if (!glfwInit())
		return NULL;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	#if __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif
	
	GLFWwindow* window = glfwCreateWindow(w, h, window_name, NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync
						 // very importent!! initialization of glad
						 // https://stackoverflow.com/questions/48582444/imgui-with-the-glad-opengl-loader-throws-segmentation-fault-core-dumped

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	return window;
}

ImGuiIO& SetupDearImgui(GLFWwindow* window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();
	ImGui::StyleColorsDark();
	return io;
}

void StartFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void RenderFrame(GLFWwindow* window, Scene& scene, Renderer& renderer, ImGuiIO& io)
{
	ImGui::Render();
	int frameBufferWidth, frameBufferHeight;
	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);
	
	if (frameBufferWidth != renderer.GetViewportWidth() || frameBufferHeight != renderer.GetViewportHeight())
	{
		// TODO: Set new aspect ratio
	}

	if (!io.WantCaptureKeyboard)
	{
		// TODO: Handle keyboard events here
		if (io.KeysDown[65])
		{
			// A key is down
			// Use the ASCII table for more key codes (https://www.asciitable.com/)
		}
	}

	if (!io.WantCaptureMouse)
	{
		// TODO: Handle mouse events here
		if (io.MouseDown[0])
		{
			// Left mouse button is down
		}
	}

	renderer.ClearColorBuffer(clear_color);
	renderer.Render(scene);
	renderer.SwapBuffers();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	glfwMakeContextCurrent(window);
	glfwSwapBuffers(window);
}

void Cleanup(GLFWwindow* window)
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
}

void DrawImguiMenus(ImGuiIO& io, Scene& scene)
{
	/**
	 * MeshViewer menu
	 */
	ImGui::Begin("MeshViewer Menu");
	
	// Menu Bar
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open", "CTRL+O"))
			{
				nfdchar_t* outPath = NULL;
				nfdresult_t result = NFD_OpenDialog("obj;", NULL, &outPath);
				if (result == NFD_OKAY)
				{
					scene.AddModel(Utils::LoadMeshModel(outPath));
					free(outPath);
				}
				else if (result == NFD_CANCEL)
				{
				}
				else
				{
				}

			}
			ImGui::EndMenu();
		}

		// TODO: Add more menubar items (if you want to)
		ImGui::EndMainMenuBar();
	}

	// Controls
	ImGui::ColorEdit3("Clear Color", (float*)&clear_color);
	// TODO: Add more controls as needed
	
	ImGui::End();

	/**
	 * Imgui demo - you can remove it once you are familiar with imgui
	 */
	
	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	{
		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Another Window", &show_another_window);

		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

	// 3. Show another simple window.
	if (show_another_window)
	{
		ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			show_another_window = false;
		ImGui::End();
	}
	//My Transformations Window
	if(scene.GetModelCount()>0)
	{
		const static char* items[] = { "Scale","Rotate","Translate"};
		const static char* TransformItems[] = {"World Transformation","Local Transformation"};
		const static char* Axis[] = { "X Axis","Y Axis","Z Axis" };
		static int SelectedItem = 0;
		static int SelectedTransform = 0;
		static int SelectedAxis;
		static float ScaleX = 1.f;
		static float ScaleY = 1.f;
		static float ScaleZ = 1.f;
		static float WScaleX = 1.f;
		static float WScaleY = 1.f;
		static float WScaleZ = 1.f;
		glm::mat4x4 Transformation;
		static int TranslateX = 0;
		static int TranslateY = 0;
		static int TranslateZ = 0;
		static int WTranslateX = 0;
		static int WTranslateY = 0;
		static int WTranslateZ = 0;
		static float Angle=0;
		static float WAngle=0;
		ImGui::Begin("Transformations Window");
		ImGui::ListBox("World Or Local", &SelectedTransform, TransformItems, IM_ARRAYSIZE(TransformItems),2);
		ImGui::ListBox("Choose Transformation",&SelectedItem,items,IM_ARRAYSIZE(items),3);
		if (SelectedTransform) {
			switch (SelectedItem)
			{
			case 0:
				ImGui::SliderFloat("Scale Factor X", &ScaleX, 0.f, 3.f);
				ImGui::SliderFloat("Scale Factor Y", &ScaleY, 0.f, 3.f);
				ImGui::SliderFloat("Scale Factor Z", &ScaleZ, 0.f, 3.f);
				Transformation = Transformations::ScalingTransformation(ScaleX, ScaleY, ScaleZ);
				scene.GetActiveModel().Set_S_m(Transformation);
				break;
			case 1:
				ImGui::ListBox("Choose Axis to rotate around", &SelectedAxis, Axis, IM_ARRAYSIZE(Axis), 3);
				ImGui::SliderFloat("Rotation Angle", &Angle,0,360);
				Transformation = (SelectedAxis == 0 ? Transformations::XRotationTransformation(Angle) : SelectedAxis == 1 ? Transformations::YRotationTransformation(Angle) : Transformations::ZRotationTransformation(Angle));
				scene.GetActiveModel().SetRotationMatrix(Transformation, false, SelectedAxis + 1);
				scene.GetActiveModel().Set_R_m();
				break;
			case 2:
				ImGui::SliderInt("Translate Factor X", &TranslateX, -500, 500);
				ImGui::SliderInt("Translate Factor Y", &TranslateY, -500, 500);
				ImGui::SliderInt("Translate Factor Z", &TranslateZ, -500, 500);
				Transformation = Transformations::TranslationTransformation(TranslateX, TranslateY, TranslateZ);
			    scene.GetActiveModel().Set_T_m(Transformation);
				break;
			default:
				break;
			}
		}
		else
		{
			
			switch (SelectedItem)
			{
			case 0:
				ImGui::SliderFloat("World Scale Factor X", &WScaleX, 0.f, 3.f);
				ImGui::SliderFloat("World Scale Factor Y", &WScaleY, 0.f, 3.f);
				ImGui::SliderFloat("World Scale Factor Z", &WScaleZ, 0.f, 3.f);
				Transformation = Transformations::ScalingTransformation(WScaleX, WScaleY, WScaleZ);
				scene.GetActiveModel().Set_S_w(Transformation);
				break;
			case 1:
				ImGui::ListBox("Choose World Axis to rotate around", &SelectedAxis, Axis, IM_ARRAYSIZE(Axis), 3);
				ImGui::SliderFloat("Rotation Angle", &WAngle,0,360);
				Transformation = (SelectedAxis == 0 ? Transformations::XRotationTransformation(WAngle) : SelectedAxis == 1 ? Transformations::YRotationTransformation(WAngle) : Transformations::ZRotationTransformation(WAngle));
				scene.GetActiveModel().SetRotationMatrix(Transformation, true, SelectedAxis + 1);
				scene.GetActiveModel().Set_R_w();
				break;
			case 2:
				ImGui::SliderInt("World Translate Factor X", &WTranslateX, -500, 500);
				ImGui::SliderInt("World Translate Factor Y", &WTranslateY, -500, 500);
				ImGui::SliderInt("World Translate Factor Z", &WTranslateZ, -500, 500);
				Transformation = Transformations::TranslationTransformation(WTranslateX, WTranslateY, WTranslateZ);
				scene.GetActiveModel().Set_T_w(Transformation);
				break;
			default:
				break;
			}
		}
		scene.GetActiveModel().SetTransformation();
		if (ImGui::Button("Reset"))
		{
			ScaleX = 1.f;
			ScaleY = 1.f;
			ScaleZ = 1.f;
			WScaleX = 1.f;
			WScaleY = 1.f;
			WScaleZ = 1.f;
			TranslateX = 0;
			TranslateY = 0;
			TranslateZ = 0;
			WTranslateX = 0;
			WTranslateY = 0;
			WTranslateZ = 0;
			Angle = 0;
			WAngle = 0;
			scene.GetActiveModel().ResetModel();
		}
		if (ImGui::Button("Show/Hide Bounding Box"))
			scene.GetActiveModel().SetBoundingBoxFlag();
		if (ImGui::Button("Show/Hide Faces Normals"))
			scene.GetActiveModel().SetFacesNormalsFlag();
		ImGui::End();
	}
}