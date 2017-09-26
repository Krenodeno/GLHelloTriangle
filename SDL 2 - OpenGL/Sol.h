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

// Classe Sol

class Sol
{
public:
	Sol(std::string const vertexShader, std::string const fragmentShader, std::string texture, int taille);
	~Sol();

	void afficher(glm::mat4 &projection, glm::mat4 &view);

private:
	glm::mat4 m_model;
	Shader m_shader;
	float m_vertices[18];
	float m_coordTexture[12];

	Texture m_texture;

	int taille;
};

