#pragma once

// Includes OpenGL

#ifdef WIN32
#include <GL/glew.h>

#else
#define GL3_PROTOTYPES 1
#include <GL3/gl3.h>

#endif


// Includes GLM

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>


// Includes

#include "Shader.h"
#include "Texture.h"

// Classe Cabane

class Cabane
{
public:
	Cabane(std::string const vertexShader, std::string const fragmentShader);
	~Cabane();

	void afficher(glm::mat4 &projection, glm::mat4 &modelview);

private:
	Shader m_shader;
	float m_vertices[99];	// 33 * 3 = 99
	float m_coordTextures[66];	// 33 * 2 = 66

	Texture m_textureMur;
	Texture m_textureToit;
};

