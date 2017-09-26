#pragma once

// Includes OpenGL

#ifdef WIN32
#include <GL/glew.h>

#else
#define GL3_PROTOTYPES 1
#include <GL3/gl3.h>

#endif

// Macro utile au VBO

#ifndef BUFFER_OFFSET

#define BUFFER_OFFSET(offset) ((char*)NULL + (offset))

#endif

// Includes GLM

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>


// Includes

#include "Shader.h"
#include "Texture.h"

// Classe Cristal

class Cristal
{
public:
	Cristal(std::string const vertexShader, std::string const fragmentShader, std::string const texture);
	~Cristal();

	void charger();
	void chargerVBO(void* donnees, int taillebytes, int decalage);

	void afficher(glm::mat4 &projection, glm::mat4 &view);

private:
	glm::mat4 m_model;
	Shader m_shader;
	float m_vertices[72];
	float m_coordTexture[48];

	Texture m_texture;
	float angle;

	GLuint m_vboID;
	int m_tailleVerticesBytes;
	int m_tailleCoordTextureBytes;

	GLuint m_vaoID;
};

