#include "SceneOpenGL.h"
#include "Shader.h"
#include "Texture.h"
#include "FrameBuffer.h"
#include "Origine.h"
#include "Cube.h"
#include "Caisse.h"
#include "Camera.h"
#include "Cabane.h"
#include "Sol.h"
#include "Cristal.h"

using namespace glm;

SceneOpenGL::SceneOpenGL(std::string titreFenetre, int largeurFenetre, int hauteurFenetre):
	m_titreFenetre(titreFenetre), m_largeurFenetre(largeurFenetre), 
	m_hauteurFenetre(hauteurFenetre), m_fenetre(0), m_contexteOpenGL(0), m_input()
{

}

SceneOpenGL::~SceneOpenGL()
{
	SDL_GL_DeleteContext(m_contexteOpenGL);
	SDL_DestroyWindow(m_fenetre);
	SDL_Quit();
}

bool SceneOpenGL::initialiserFenetre()
{
	//Initialisation de la SDL

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "Erreur lors de l'initialisation de la SDL : " << SDL_GetError() << std::endl;
		SDL_Quit();

		return false;
	}

#ifdef __APPLE__

	// Version d'OpenGL

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	// Récupération du Bundle

	CFURLRef URLBundle = CFBundleCopyResourcesDirectoryURL(CFBundleGetMainBundle());
	char *cheminResources = new char[PATH_MAX];


	// Changement du 'Working Directory'

	if (CFURLGetFileSystemRepresentation(URLBundle, 1, (UInt8*)cheminResources, PATH_MAX))
		chdir(cheminResources);


	// Libération de la mémoire

	delete[] cheminResources;
	CFRelease(URLBundle);

#else

	// Version d'OpenGL

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

#endif

	//Double Buffer

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	//Création de la fenêtre

	m_fenetre = SDL_CreateWindow(m_titreFenetre.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		m_largeurFenetre, m_hauteurFenetre, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

	if (m_fenetre == 0)
	{
		std::cout << "Erreur lors de la création de la fenetre : " << SDL_GetError() << std::endl;
		SDL_Quit();

		return false;
	}

	//Création du contexte OpenGL

	m_contexteOpenGL = SDL_GL_CreateContext(m_fenetre);

	if (m_contexteOpenGL == 0)
	{
		std::cout << SDL_GetError() << std::endl;
		SDL_DestroyWindow(m_fenetre);
		SDL_Quit();

		return false;
	}

	return true;
}

bool SceneOpenGL::initGL()
{
#ifdef WIN32

	//On initialise GLEW
	
	GLenum initisialisationGLEW(glewInit());

	//Si l'initialisation a échouée

	if (initisialisationGLEW != GLEW_OK)
	{
		//On affiche l'erreur grace à la fontion : glewGetErrorString(GLenum code)

		std::cout << "Erreur d'initialisation de GLEW : " << glewGetErrorString(initisialisationGLEW) << std::endl;

		//On quitte la SDL

		SDL_GL_DeleteContext(m_contexteOpenGL);
		SDL_DestroyWindow(m_fenetre);
		SDL_Quit();

		return false;
	}

#endif

	// Activation du Depth Buffer

	glEnable(GL_DEPTH_TEST);

	// Tout s'est bien passé, on retourne true

	return true;
}

void SceneOpenGL::bouclePrincipale()
{
	// Variables

	float frameRate(1000 / 60);
	Uint32 debutBoucle(0), finBoucle(0), tempsEcoule(0);

	double fov = 70.0;

	// Framebuffer

	FrameBuffer frameBuffer(512, 512);
	frameBuffer.charger();

	// Matrices

	mat4 projectionFBO, viewFBO;
	mat4 projection, view;

	projectionFBO = perspective(fov, (double)frameBuffer.getLargeur() / frameBuffer.getHauteur(), 1.0, 100.0);
	viewFBO = mat4(1.0);

	projection = perspective(fov, (double)m_largeurFenetre / m_hauteurFenetre, 1.0, 100.0);
	view = mat4(1.0);

	// Caméra mobile

	Camera camera(vec3(3, 3, 3), vec3(0, 0, 0), vec3(0, 1, 0), 0.5, 0.5);
	m_input.afficherPointeur(false);
	m_input.capturerPointeur(true);

	// Objet origine

	Origine origine;
	origine.charger();
	
	// Objet Caisse

	Caisse caisse(2.0, "Shaders/textureMVP.vert", "Shaders/texture.frag", "Textures/Caisse00.jpg");
	caisse.charger();

	float angle = 0.0;

	// Vertices

	float vertices[] = { 0, 0, 0,   4, 0, 0,   4, 4, 0,     // Triangle 1
		0, 0, 0,   0, 4, 0,   4, 4, 0 };    // Triangle 2

	float coordTexture[] = { 0, 0,   1, 0,   1, 1,          // Triangle 1
		0, 0,   0, 1,   1, 1 };         // Triangle 2

	mat4 model(1.0);

	/* ***** Gestion du VBO ***** */

	GLuint vbo;
	int tailleVerticesBytes = 18 * sizeof(float);
	int tailleCoordTextureBytes = 12 * sizeof(float);


	// Génération du VBO

	glGenBuffers(1, &vbo);


	// Verrouillage

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// Remplissage

	glBufferData(GL_ARRAY_BUFFER, tailleVerticesBytes + tailleCoordTextureBytes, 0, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, tailleVerticesBytes, vertices);
	glBufferSubData(GL_ARRAY_BUFFER, tailleVerticesBytes, tailleCoordTextureBytes, coordTexture);


	// Déverrouillage

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	/* ***** Gestion du VAO ***** */

	GLuint vao;


	// Génération du VAO

	glGenVertexArrays(1, &vao);


	// Verrouillage du VAO

	glBindVertexArray(vao);

	// Verrouillage du VBO

	glBindBuffer(GL_ARRAY_BUFFER, vbo);


	// Vertex Attrib 0 (Vertices)

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);


	// Vertex Attrib 2 (Texture)

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(tailleVerticesBytes));
	glEnableVertexAttribArray(2);


	// Déverrouillage du VBO

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	// Déverrouillage du VAO

	glBindVertexArray(0);


	// Shader

	Shader shaderTexture("Shaders/textureMVP.vert", "Shaders/texture.frag");
	shaderTexture.charger();

	// Boucle principale

	while (!m_input.terminer())
	{
		// On définit le temps de début de boucle

		debutBoucle = SDL_GetTicks();


		// Gestion des évènements

		m_input.updateEvenements();

		if (m_input.getTouche(SDL_SCANCODE_ESCAPE))
			break;

		if (m_input.getTouche(SDL_SCANCODE_KP_MINUS)) {
			fov--;
			projection = perspective(fov, (double)m_largeurFenetre / m_hauteurFenetre, 1.0, 100.0);
		}

		if (m_input.getTouche(SDL_SCANCODE_KP_PLUS)) {
			fov++;
			projection = perspective(fov, (double)m_largeurFenetre / m_hauteurFenetre, 1.0, 100.0);
		}


		camera.deplacer(m_input);

		//////////////////////////////////////////////////////////////////////
		// Verrouillage du Framebuffer
		
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer.getID());

		// Nettoyage de l'écran

		glClearColor(0.5, 0.5, 0.5, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Redimentssionnement de la zone d'affichage

		glViewport(0, 0, frameBuffer.getLargeur(), frameBuffer.getHauteur());

		// Placement de la caméra

		viewFBO = lookAt(vec3(3, 0, 3), vec3(0, 0, 0), vec3(0, 1, 0));
		
		// Rotation de la caisse

		angle += 2;
		if (angle > 360)
			angle -= 360;

		// Sauvegarde de la matrice modelview

		mat4 sauvegardeModelviewFBO = viewFBO;

		// Translation pour positionner la caisse

		viewFBO = rotate(viewFBO, angle, vec3(0, 1, 0));

		// Affichage de la caisse

		caisse.afficher(projectionFBO, viewFBO);

		// Restauration de la matrice

		viewFBO = sauvegardeModelviewFBO;

		// Déverrouillage du Framebuffer

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		
		//////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////
		// Nettoyage de l'écran
		
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Redimenssionnement de la zone d'affichage

		glViewport(0, 0, m_largeurFenetre, m_hauteurFenetre);

		// Gestion de la caméra

		camera.lookAt(view);

		// Affichage de l'origine

		origine.afficher(projection, view);

		// Activation du shader

		glUseProgram(shaderTexture.getProgramID());


		// Verrouillage du VAO

		glBindVertexArray(vao);


		// Envoi des matrices

		shaderTexture.envoyerMat4("modelviewProjection", projection * view * model);


		// Verrouillage de la texture

		glBindTexture(GL_TEXTURE_2D, frameBuffer.getColorBufferID(0));


		// Rendu

		glDrawArrays(GL_TRIANGLES, 0, 6);


		// Déverrouillage de la texture

		glBindTexture(GL_TEXTURE_2D, 0);


		// Verrouillage du VAO

		glBindVertexArray(0);


		// Désactivation du shader

		glUseProgram(0);
		
		//////////////////////////////////////////////////////////////////////
		// Actualisation de la fenêtre

		SDL_GL_SwapWindow(m_fenetre);


		// Calcul du temps écoulé

		finBoucle = SDL_GetTicks();
		tempsEcoule = finBoucle - debutBoucle;
		//std::cout << "frametime :\t" << tempsEcoule << "ms\n";


		// Si nécessaire, on met en pause le programme

		if (tempsEcoule < frameRate)
			SDL_Delay(frameRate - tempsEcoule);
	}
}
