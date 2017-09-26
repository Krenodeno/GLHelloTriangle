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
#include <vector>
#include "Shader.h"
#include "Texture.h"

class Model
{
public:
	Model(float taille, std::string const vertexShader, std::string const fragmentShader, std::string const texture);
	~Model();

	void charger();
	void chargerVBO(void* donnees, int taillebytes, int decalage);
	void afficher(glm::mat4&projection, glm::mat4 &modelview);

protected:
	Shader m_shader;
	Texture m_texture;
	std::vector<float> m_vertices;
	std::vector<float> m_coordtexture;

	GLuint m_vboID;
	int m_tailleVerticesBytes;
	int m_tailleCoordTextureBytes;

	GLuint m_vaoID;
};

