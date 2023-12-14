#pragma once

#include <gl/glew.h>
#include <spdlog/spdlog.h>

#include <string>
#include <fstream>

#include "Common.h"
#include "PointLight.h"

class Shader {
public:
	Shader();
	virtual ~Shader();

	void CreateFromString(const char* vertexCode, const char* fragmentCode);
	void CreateFromFiles(const char* vertexPath, const char* fragmentPath);
	std::string ReadFile(const char* filePath);

	void UseShader();
	void ClearShader();

	GLuint GetWorldLocation() const;
	GLuint GetViewLocation() const;
	GLuint GetProjectionLocation() const;
	GLuint GetCameraPositionLocation() const;

	GLuint GetLightCoulourLocation() const;

	GLuint GetAmbientIntensityLocation() const;
	GLuint GetDiffuseIntensityLocation() const;

	GLuint GetLightPositionLocation() const;


	GLuint GetConstantLocation() const;
	GLuint GetLinearLocation() const;
	GLuint GetExponentLocation() const;

	void SetPointLight(PointLight* light);

private:
	GLuint mShaderID, mUniformProjection, mUniformWorld, mUniformView, mUniformCameraPosition,
			mUniformLightColour,
			mUniformAmbientIntensity, mUniformDiffuseIntensity, 
			mUniformLightPosition, 
			mUniformConstant, mUniformLinear, mUniformExponent;

	void CompileShader(const char* vertexCode, const char* fragmentCode);
	void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
};
