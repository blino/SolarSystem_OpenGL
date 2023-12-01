﻿#define STB_IMAGE_IMPLEMENTATION

#include <assimp/Importer.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <spdlog/spdlog.h>


#include <memory>
#include <utility>
#include <fstream>

#include "include/Common.h"
#include "include/Camera.h"
#include "include/Shader.h"
#include "include/Window.h"
#include "include/SolarSystem.h"
#include "include/Model.h"

using namespace std;

static GLfloat deltaTimeMs = 0.0f;
static GLfloat lastTimeMs = 0.0f;

// vertex shader
static const char* vShader = "../../Shaders/shader.vert";

// fragment shader
static const char* fShader = "../../Shaders/shader.frag";

int main(int argc, char** argv)
{
	// Window
	constexpr int WIDTH = 1152;
	constexpr int HEIGHT = 864;

	SPDLOG_INFO("Create GLFW window");
	unique_ptr<Window> mainWindow = make_unique<Window>(WIDTH, HEIGHT);
	mainWindow->Initialise();

	// Camera
	unique_ptr<Camera> camera = make_unique<Camera>(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.0f, 1.0f, 0.0f), 0.f, 0.0f, 5.0f, 0.5f);

	// Solar system
	unique_ptr<SolarSystem> solarSystem;
	solarSystem = SolarSystem::GetInstance();


	unique_ptr<Shader> shader = make_unique<Shader>();
	shader->CreateFromFiles(vShader, fShader);
	
	unique_ptr<Model> xWing = make_unique<Model>();
	xWing->LoadModel("../../Models/Xwing/x-wing.obj");

	unique_ptr<Model> earth = make_unique<Model>();
	earth->LoadModel("../../Models/Earth/Earth.obj");

	GLuint uniformProjection = 0, uniformView = 0, uniformWorld = 0, uniformCameraPosition = 0;
	glm::mat4 projection = glm::perspective(glm::radians(45.f), mainWindow->GetBufferWidth() / static_cast<GLfloat>(mainWindow->GetBufferHeight()), 0.1f, 100.f);

	// Loop until window closed
	while (!mainWindow->GetShouldClose()) {
		GLfloat currentMs = glfwGetTime(); // SDL_GetPerformanceCounter();
		deltaTimeMs = currentMs - lastTimeMs; // (current - lastTime)*1000/SDL_GetPerformanceFrequency();
		lastTimeMs = currentMs;

		// Get + Handle User Input
		glfwPollEvents();

		camera->KeyControl(mainWindow->GetKeys(), deltaTimeMs);
		camera->MouseControl(mainWindow->GetXChange(), mainWindow->GetYChange());

		// Clear the window
		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader->UseShader();
		uniformWorld = shader->GetWorldLocation();
		uniformView = shader->GetViewLocation();
		uniformProjection = shader->GetProjectionLocation();
		uniformCameraPosition = shader->GetCameraPositionLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera->CalcViewMatrix()));
		glUniform3f(uniformCameraPosition, camera->GetCameraPosition().x, camera->GetCameraPosition().y, camera->GetCameraPosition().z);

		glm::mat4 model(1.f);
		model = glm::translate(model, glm::vec3(0.f, 0.f, 0.f));
		model = glm::scale(model, glm::vec3(0.006f, 0.006f, 0.006f));
		glUniformMatrix4fv(uniformWorld, 1, GL_FALSE, glm::value_ptr(model));
		xWing->RenderModel();

		model = glm::mat4(1.f);
		model = glm::translate(model, glm::vec3(10.f, 0.f, 0.f));
		model = glm::scale(model, glm::vec3(1.f, 1.f, 1.f));
		glUniformMatrix4fv(uniformWorld, 1, GL_FALSE, glm::value_ptr(model));
		earth->RenderModel();

		glUseProgram(0);
		mainWindow->SwapBuffers();
	}

	xWing->ClearModel();

	return 0;
}