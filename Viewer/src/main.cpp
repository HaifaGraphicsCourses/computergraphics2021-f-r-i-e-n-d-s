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

static void RGBtoHSV(float fR, float fG, float fB, float& fH, float& fS, float& fV) {
	float min, max, delta;

	min = fR < fG ? fR : fG;
	min = min < fB ? min : fB;

	max = fR > fG ? fR : fG;
	max = max > fB ? max : fB;

	fV = max;
	delta = max - min;
	if (delta < 0.00001)
	{
		fS = 0;
		fH = 0;
	}
	if (max > 0.0) {
		fS = (delta / max);
	}
	else {
		fS = 0.0;
		fH = 0;
		return ;
	}
	if (fR >= max)                           // > is bogus, just keeps compilor happy
		fH = (fG - fB) / delta;        // between yellow & magenta
	else
		if (fG >= max)
			fH = 2.0 + (fB - fR) / delta;  // between cyan & yellow
		else
			fH = 4.0 + (fR - fG) / delta;  // between magenta & cyan

	fH *= 60.0;                              // degrees

	if (fH < 0.0)
		fH += 360.0;

	return;
}
glm::vec4 clear_color = glm::vec4(0.2f, 0.2f, 0.2f, 1.00f);
static float BoundingBoxColor[3] = { 1.f, 0.f, 0.f };
static float NormalsColor[3]= { 0.585f, 0.f, 0.6045f };
static float FacesNormalsColor[3] = { 0.f,0.f,1.f };
static float ModelColor[3]= {0.2f, 0.75f, 0.8f};

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
	int windowWidth = 1920, windowHeight = 1080;
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
		glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);
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
	glfwSetWindowAspectRatio(window, renderer.GetViewportWidth(), renderer.GetViewportHeight());
	if (frameBufferWidth != renderer.GetViewportWidth() || frameBufferHeight != renderer.GetViewportHeight())
	{
		renderer.SetViewportHeight(frameBufferHeight);
		renderer.SetViewportWidth(frameBufferWidth);
		if (scene.GetCameraCount()) {
			scene.GetActiveCamera().SetWidth(frameBufferWidth);
			scene.GetActiveCamera().SetHeight(frameBufferHeight);
			scene.GetActiveCamera().SetAspectRatio(frameBufferWidth, frameBufferHeight);
		}
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
	static float OrthoWidth;
	/**
	 * MeshViewer menu
	 */
	ImGui::Begin("MeshViewer Menu");
	
	// Menu Bar
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Choose Model"))
		{
			if (ImGui::MenuItem("Banana")) {
				scene.AddModel(Utils::LoadMeshModel("C:\\Users\\most_\\OneDrive\\Documents\\GitHub\\computergraphics2021-f-r-i-e-n-d-s\\Data\\banana.obj"));
				OrthoWidth = (scene.GetActiveModel().GetMinOrtho() + scene.GetActiveModel().GetMaxOrtho()) / 3;
			}
			if (ImGui::MenuItem("Beethoven")) {
				scene.AddModel(Utils::LoadMeshModel("C:\\Users\\most_\\OneDrive\\Documents\\GitHub\\computergraphics2021-f-r-i-e-n-d-s\\Data\\beethoven.obj"));
				OrthoWidth = (scene.GetActiveModel().GetMinOrtho() + scene.GetActiveModel().GetMaxOrtho()) / 3;
			}
			if (ImGui::MenuItem("Bishop")) {
				scene.AddModel(Utils::LoadMeshModel("C:\\Users\\most_\\OneDrive\\Documents\\GitHub\\computergraphics2021-f-r-i-e-n-d-s\\Data\\bishop.obj"));
				OrthoWidth = (scene.GetActiveModel().GetMinOrtho() + scene.GetActiveModel().GetMaxOrtho()) / 3;
			}
			if (ImGui::MenuItem("Blob")) {
				scene.AddModel(Utils::LoadMeshModel("C:\\Users\\most_\\OneDrive\\Documents\\GitHub\\computergraphics2021-f-r-i-e-n-d-s\\Data\\blob.obj"));
				OrthoWidth = (scene.GetActiveModel().GetMinOrtho() + scene.GetActiveModel().GetMaxOrtho()) / 3;
			}
			if (ImGui::MenuItem("Bunny")) {
				scene.AddModel(Utils::LoadMeshModel("C:\\Users\\most_\\OneDrive\\Documents\\GitHub\\computergraphics2021-f-r-i-e-n-d-s\\Data\\bunny.obj"));
				OrthoWidth = (scene.GetActiveModel().GetMinOrtho() + scene.GetActiveModel().GetMaxOrtho()) / 3;
			}
			if (ImGui::MenuItem("Camera")) {
				scene.AddModel(Utils::LoadMeshModel("C:\\Users\\most_\\OneDrive\\Documents\\GitHub\\computergraphics2021-f-r-i-e-n-d-s\\Data\\camera.obj"));
				OrthoWidth = (scene.GetActiveModel().GetMinOrtho() + scene.GetActiveModel().GetMaxOrtho()) / 3;
			}
			if (ImGui::MenuItem("Chain")) {
				scene.AddModel(Utils::LoadMeshModel("C:\\Users\\most_\\OneDrive\\Documents\\GitHub\\computergraphics2021-f-r-i-e-n-d-s\\Data\\chain.obj"));
				OrthoWidth = (scene.GetActiveModel().GetMinOrtho() + scene.GetActiveModel().GetMaxOrtho()) / 3;
			}
			if (ImGui::MenuItem("Cow")) {
				scene.AddModel(Utils::LoadMeshModel("C:\\Users\\most_\\OneDrive\\Documents\\GitHub\\computergraphics2021-f-r-i-e-n-d-s\\Data\\cow.obj"));
				OrthoWidth = (scene.GetActiveModel().GetMinOrtho() + scene.GetActiveModel().GetMaxOrtho()) / 3;
			}
			if (ImGui::MenuItem("Demo")) {
				scene.AddModel(Utils::LoadMeshModel("C:\\Users\\most_\\OneDrive\\Documents\\GitHub\\computergraphics2021-f-r-i-e-n-d-s\\Data\\demo.obj"));
				OrthoWidth = (scene.GetActiveModel().GetMinOrtho() + scene.GetActiveModel().GetMaxOrtho()) / 3;
			}
			if (ImGui::MenuItem("Dolphin")) {
				scene.AddModel(Utils::LoadMeshModel("C:\\Users\\most_\\OneDrive\\Documents\\GitHub\\computergraphics2021-f-r-i-e-n-d-s\\Data\\dolphin.obj"));
				OrthoWidth = (scene.GetActiveModel().GetMinOrtho() + scene.GetActiveModel().GetMaxOrtho()) / 3;
			}
			if (ImGui::MenuItem("Feline")) {
				scene.AddModel(Utils::LoadMeshModel("C:\\Users\\most_\\OneDrive\\Documents\\GitHub\\computergraphics2021-f-r-i-e-n-d-s\\Data\\feline.obj"));
				OrthoWidth = (scene.GetActiveModel().GetMinOrtho() + scene.GetActiveModel().GetMaxOrtho()) / 3;
			}
			if (ImGui::MenuItem("Pawn")) {
				scene.AddModel(Utils::LoadMeshModel("C:\\Users\\most_\\OneDrive\\Documents\\GitHub\\computergraphics2021-f-r-i-e-n-d-s\\Data\\pawn.obj"));
				OrthoWidth = (scene.GetActiveModel().GetMinOrtho() + scene.GetActiveModel().GetMaxOrtho()) / 3;
			}
			if (ImGui::MenuItem("Teapot")) {
				scene.AddModel(Utils::LoadMeshModel("C:\\Users\\most_\\OneDrive\\Documents\\GitHub\\computergraphics2021-f-r-i-e-n-d-s\\Data\\teapot.obj"));
				OrthoWidth = (scene.GetActiveModel().GetMinOrtho() + scene.GetActiveModel().GetMaxOrtho()) / 3;
			}
				if (ImGui::MenuItem("Other", "CTRL+O"))
			{
				nfdchar_t* outPath = NULL;
				nfdresult_t result = NFD_OpenDialog("obj;", NULL, &outPath);
				if (result == NFD_OKAY)
				{
					scene.AddModel(Utils::LoadMeshModel(outPath));
					OrthoWidth = (scene.GetActiveModel().GetMinOrtho() + scene.GetActiveModel().GetMaxOrtho()) / 3;
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
		if (ImGui::Button("Clear Model"))
		{
			scene.ClearActiveModel();
		}
		// TODO: Add more menubar items (if you want to)
		ImGui::EndMainMenuBar();
	}

	// Controls
	ImGui::ColorEdit3("Background Color", (float*)&clear_color);
	ImGui::ColorEdit3("Bounding Box Color",BoundingBoxColor);
	ImGui::ColorEdit3("Vertices Normals Color", NormalsColor);
	ImGui::ColorEdit3("Faces Normals Color", FacesNormalsColor);
	ImGui::ColorEdit3("Model Color", ModelColor);
	if(scene.GetModelCount())
	scene.GetActiveModel().SetColors(BoundingBoxColor, FacesNormalsColor, NormalsColor, ModelColor);
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
	std::string name;
	//My Transformations Window
	if(scene.GetModelCount()>0)
	{
		scene.AddCamera(std::make_shared<Camera>(scene.GetActiveModel().GetPreffered_Eye()));
		auto model = scene.GetActiveModel();
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
		static float TranslateX = 0;
		static float TranslateY = 0;
		static float TranslateZ = 0;
		static float WTranslateX = 0;
		static float WTranslateY = 0;
		static float WTranslateZ = 0;
		static float AngleX=0;
		static float AngleY=0;
		static float AngleZ=0;
		static float WAngleX=0;
		static float WAngleY=0;
		static float WAngleZ=0;
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
				if (SelectedAxis == 0) {
					ImGui::SliderFloat("Rotation Angle", &AngleX, 0, 360);
					Transformation = Transformations::XRotationTransformation(AngleX);
				}
				if (SelectedAxis == 1)
				{
					ImGui::SliderFloat("Rotation Angle", &AngleY, 0, 360);
					Transformation = Transformations::YRotationTransformation(AngleY);
				}
				if (SelectedAxis == 2)
				{
					ImGui::SliderFloat("Rotation Angle", &AngleZ, 0, 360);
					Transformation = Transformations::ZRotationTransformation(AngleZ);
				}
				scene.GetActiveModel().SetRotationMatrix(Transformation, false, SelectedAxis + 1);
				scene.GetActiveModel().Set_R_m();
				break;
			case 2:
				ImGui::SliderFloat("Translate Factor X", &TranslateX, -220, 220);
				ImGui::SliderFloat("Translate Factor Y", &TranslateY, -220, 220);
				ImGui::SliderFloat("Translate Factor Z", &TranslateZ, -220, 220);
				Transformation = Transformations::TranslationTransformation(TranslateX/ scene.GetActiveModel().GetTranslateFactor(), TranslateY/ scene.GetActiveModel().GetTranslateFactor(), TranslateZ/ scene.GetActiveModel().GetTranslateFactor());
			    scene.GetActiveModel().Set_T_m(Transformation);
				break;
			default:
				break;
			}
			scene.GetActiveModel().SetModelTransformation(scene.GetActiveModel().Get_S_m()* scene.GetActiveModel().Get_R_m()* scene.GetActiveModel().Get_T_m());
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
				if (SelectedAxis == 0) {
					ImGui::SliderFloat("Rotation Angle", &WAngleX, 0, 360);
					Transformation = Transformations::XRotationTransformation(WAngleX);
				}
				if (SelectedAxis == 1)
				{
					ImGui::SliderFloat("Rotation Angle", &WAngleY, 0, 360);
					Transformation = Transformations::YRotationTransformation(WAngleY);
				}
				if (SelectedAxis == 2)
				{
					ImGui::SliderFloat("Rotation Angle", &WAngleZ, 0, 360);
					Transformation = Transformations::ZRotationTransformation(WAngleZ);
				}
				scene.GetActiveModel().SetRotationMatrix(Transformation, true, SelectedAxis + 1);
				scene.GetActiveModel().Set_R_w();
				break;
			case 2:
				ImGui::SliderFloat("World Translate Factor X", &WTranslateX, -220,220);
				ImGui::SliderFloat("World Translate Factor Y", &WTranslateY, -220,220);
				ImGui::SliderFloat("World Translate Factor Z", &WTranslateZ, -220,220);
				Transformation = Transformations::TranslationTransformation(WTranslateX/ scene.GetActiveModel().GetTranslateFactor(), WTranslateY/ scene.GetActiveModel().GetTranslateFactor(), WTranslateZ/ scene.GetActiveModel().GetTranslateFactor());
				scene.GetActiveModel().Set_T_w(Transformation);
				break;
			default:
				break;
			}
			scene.GetActiveModel().SetWorldTransformation(scene.GetActiveModel().Get_S_w() * scene.GetActiveModel().Get_R_w() * scene.GetActiveModel().Get_T_w());
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
			AngleX = 0;
			AngleY = 0;
			AngleZ = 0;
			WAngleX = 0;
			WAngleY = 0;
			WAngleZ = 0;
			scene.GetActiveModel().ResetModel();
		}
		// Reset The model to the center
		float color[3];
		RGBtoHSV(BoundingBoxColor[0]*255, BoundingBoxColor[1] * 255, BoundingBoxColor[2] * 255, color[0], color[1], color[2]);
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(color[0]/360, color[1], color[2]/255));
		if (ImGui::Button("Show/Hide Bounding Box"))
			scene.GetActiveModel().SetBoundingBoxFlag();
		ImGui::PopStyleColor();
		
		ImGui::SameLine();
		RGBtoHSV(FacesNormalsColor[0] * 255, FacesNormalsColor[1] * 255, FacesNormalsColor[2] * 255, color[0], color[1], color[2]);
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(color[0]/360, color[1], color[2]/255));
		if (ImGui::Button("Show/Hide Faces Normals"))
			scene.GetActiveModel().SetFacesNormalsFlag();
		ImGui::PopStyleColor();
		
		ImGui::SameLine();
		RGBtoHSV(NormalsColor[0] * 255, NormalsColor[1] * 255, NormalsColor[2] * 255, color[0], color[1], color[2]);
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(color[0]/360, color[1], color[2]/255));
		if (ImGui::Button("Show/Hide Normals"))
			scene.GetActiveModel().SetNormalsFlag();
		ImGui::PopStyleColor();
		ImGui::End();

		ImGui::Begin("Camera Window");
		static int Projection = 1;
		static float eye[3] = { scene.GetActiveModel().GetPreffered_Eye()[0],scene.GetActiveModel().GetPreffered_Eye()[1],scene.GetActiveModel().GetPreffered_Eye()[2] };
		static float at[3]={0,0,0};
		static float up[3]={0,1,0};
        static float cameraTranslationX, cameraTranslationY, cameraTranslationZ;
        static float cameraRotationX, cameraRotationY, cameraRotationZ;
        static float worldTranslationX, worldTranslationY, worldTranslationZ;
        static float worldRotationX, worldRotationY, worldRotationZ;
		static int IsWorld, TransformationType;
		static float fovy=(45.f);
		static float Near=-0.1;
		static float Far=1000;
		ImGui::Text("Choose Projection"); ImGui::SameLine();
		ImGui::RadioButton("Perspective", &Projection, 0); ImGui::SameLine();
		ImGui::RadioButton("Orthographic", &Projection, 1);
		if (Projection) 
		{
			scene.GetActiveCamera().SetIsOrthographic(true);
			ImGui::SliderFloat("Orho Width",&OrthoWidth, scene.GetActiveModel().GetMinOrtho(), scene.GetActiveModel().GetMaxOrtho());
			scene.GetActiveCamera().SetOrthographicWidth(OrthoWidth);
		}
		else
		{
			scene.GetActiveCamera().SetIsOrthographic(false);
			ImGui::SliderFloat("Fov", &fovy,20,120);
			scene.GetActiveCamera().SetFovy(fovy);
			//perspective
		}

		if (ImGui::TreeNode("LookAt Parameters")) {
			ImGui::InputFloat3("Eye", eye, 3);
			ImGui::InputFloat3("At", at, 3);
			ImGui::InputFloat3("Up", up, 3);
			scene.GetActiveCamera().SetAt(glm::vec3(at[0],at[1],at[2]));
			scene.GetActiveCamera().SetEye(glm::vec3(eye[0],eye[1],eye[2]));
			scene.GetActiveCamera().SetUp(glm::vec3(up[0],up[1],up[2]));
			if (ImGui::Button("Look At"))
			{
				scene.GetActiveCamera().SetCameraLookAt();
			}
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Camera Transformations")) {
			ImGui::RadioButton("Local", &IsWorld, 0);
			ImGui::SameLine();
			ImGui::RadioButton("World", &IsWorld, 1);
			ImGui::RadioButton("Translate", &TransformationType, 0);
			ImGui::SameLine();
			ImGui::RadioButton("Rotate", &TransformationType, 1);
			if (IsWorld)
			{
				if (TransformationType)
				{
					ImGui::SliderFloat("Rotate in X", &worldRotationX, 0, 360);
					ImGui::SliderFloat("Rotate in y", &worldRotationY, 0, 360);
					ImGui::SliderFloat("Rotate in Z", &worldRotationZ, 0, 360);
					scene.GetActiveCamera().SetRotationMatrix(Transformations::XRotationTransformation(worldRotationX), true, 1);
					scene.GetActiveCamera().SetRotationMatrix(Transformations::YRotationTransformation(worldRotationY), true, 2);
					scene.GetActiveCamera().SetRotationMatrix(Transformations::ZRotationTransformation(worldRotationZ), true, 3);
				}
				else
				{
					ImGui::SliderFloat("Translate in X", &worldTranslationX, -200, 200);
					ImGui::SliderFloat("Translate in y", &worldTranslationY, -200, 200);
					ImGui::SliderFloat("Translate in Z", &worldTranslationZ, -200, 200);
					scene.GetActiveCamera().SetWTranslate(Transformations::TranslationTransformation(worldTranslationX/1000, worldTranslationY/1000, worldTranslationZ/1000));
				}
				scene.GetActiveCamera().SetWTransformation();
			}
			else
			{
				if (TransformationType)
				{
					ImGui::SliderFloat("Rotate in X", &cameraRotationX, 0, 360);
					ImGui::SliderFloat("Rotate in y", &cameraRotationY, 0, 360);
					ImGui::SliderFloat("Rotate in Z", &cameraRotationZ, 0, 360);
					scene.GetActiveCamera().SetRotationMatrix(Transformations::XRotationTransformation(cameraRotationX), false, 1);
					scene.GetActiveCamera().SetRotationMatrix(Transformations::YRotationTransformation(cameraRotationY), false, 2);
					scene.GetActiveCamera().SetRotationMatrix(Transformations::ZRotationTransformation(cameraRotationZ), false, 3);
				}
				else
				{
					ImGui::SliderFloat("Translate in x", &cameraTranslationX, -200, 200);
					ImGui::SliderFloat("Translate in y", &cameraTranslationY, -200, 200);
					ImGui::SliderFloat("Translate in z", &cameraTranslationZ, -200, 200);
					scene.GetActiveCamera().SetCTranslate(Transformations::TranslationTransformation(cameraTranslationX/1000, cameraTranslationY/1000, cameraTranslationZ/1000));
				}
				scene.GetActiveCamera().SetCTransformation();
			}
			scene.GetActiveCamera().SetTransformations();
			if (ImGui::Button("Reset Transformations"))
			{
				scene.GetActiveCamera().ResetTransformations();
				cameraTranslationX=0;
				cameraTranslationY=0;
				cameraTranslationZ=0;
				cameraRotationX=0;
				cameraRotationY=0;
				cameraRotationZ=0;
				worldTranslationX=0;
				worldTranslationY=0;
				worldTranslationZ=0;
				worldRotationX=0;
				worldRotationY=0;
				worldRotationZ=0;
			}
			ImGui::TreePop();
		}

		ImGui::End();
	}
}