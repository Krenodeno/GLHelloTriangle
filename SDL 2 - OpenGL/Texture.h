#ifndef DEF_TEXTURE
#define DEF_TEXTURE


// Include Windows

#ifdef WIN32
#include <GL/glew.h>
#include <SDL2/SDL_image.h>


// Include Mac

#elif __APPLE__
#define GL3_PROTOTYPES 1
#include <OpenGL/gl3.h>
#include <SDL2_image/SDL_image.h>


// Include UNIX/Linux

#else
#define GL3_PROTOTYPES 1
#include <GL3/gl3.h>
#include <SDL2/SDL_image.h>

#endif

#include <SDL2/SDL.h>
#include <iostream>
#include <string>


// Classe Texture

class Texture
{
public:

	Texture();
	Texture(std::string fichierImage);
	Texture(int largeur, int hauteur, GLenum format, GLenum formatInterne, bool textureVide);
	Texture(Texture const &textureACopier);
	~Texture();

	bool charger();
	void chargerTextureVide();
	GLuint getID() const;
	void setFichierImage(const std::string &fichierImage);
	SDL_Surface* inverserPixels(SDL_Surface *imageSource) const;

	Texture& operator=(Texture const &textureACopier);

private:

	GLuint m_id;
	std::string m_fichierImage;

	int m_largeur;
	int m_hauteur;
	GLenum m_format;
	GLenum m_formatInterne;
	bool m_textureVide;
};

#endif