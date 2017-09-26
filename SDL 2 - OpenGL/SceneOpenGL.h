#ifndef DEF_SCENEOPENGL
#define DEF_SCENEOPENGL

// Includes OpenGL Windows

#ifdef WIN32
#include <GL/glew.h>

// Include OpenGL Apple

#elif __APPLE__
#define GL3_PROTOTYPES 1
#include <OpenGL/gl3.h>
#include <CoreFoundation/CoreFoundation.h>

// Include OpenGL Linux

#else
#define GL3_PROTOTYPES 1
#include <GL3/gl3.h>

#endif

//Entêtes GLM

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Autres includes

#include <SDL2/SDL.h>
#include "Input.h"
#include <iostream>
#include <string>

// Classe

class SceneOpenGL
{
public:

	SceneOpenGL(std::string titreFenetre, int largeurFenetre, int hauteurFenetre);
	~SceneOpenGL();

	bool initialiserFenetre();
	bool initGL();
	void bouclePrincipale();

private:

	std::string m_titreFenetre;
	int m_largeurFenetre;
	int m_hauteurFenetre;

	SDL_Window* m_fenetre;
	SDL_GLContext m_contexteOpenGL;
	SDL_Event m_evenements;

	Input m_input;
};

#endif