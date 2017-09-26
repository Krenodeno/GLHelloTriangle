#ifndef DEF_ORIGINE
#define DEF_ORIGINE

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

// Classe Origine

class Origine
{
public:
	Origine();
	~Origine();

	void charger();
	void chargerVBO(void* donnees, int taillebytes, int decalage);
	void afficher(glm::mat4 &projection, glm::mat4 &modelview);

private:
	Shader m_shader;
	float m_vertices[18];
	float m_couleurs[18];

	GLuint m_vboID;;
	int m_tailleVerticesBytes;
	int m_tailleCouleursBytes;

	GLuint m_vaoID;
};

#endif