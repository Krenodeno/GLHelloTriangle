#ifndef DEF_FRAMEBUFFER
#define DEF_FRAMEBUFFER

// Include Windows

#ifdef WIN32
#include <GL/glew.h>


// Include Mac

#elif __APPLE__
#define GL3_PROTOTYPES 1
#include <OpenGL/gl3.h>


// Include UNIX/Linux

#else
#define GL3_PROTOTYPES 1
#include <GL3/gl3.h>

#endif

// Includes

#include <vector>
#include "Texture.h"


// Classe FrameBuffer

class FrameBuffer
{
public:
	FrameBuffer();
	FrameBuffer(int largeur, int hauteur, bool utiliserStencilBuffer = false);
	FrameBuffer(FrameBuffer const & frameBufferACopier);
	~FrameBuffer();

	bool charger();
	void creerRenderBuffer(GLuint &id, GLenum formatInterne);

	GLuint getID() const;
	GLuint getColorBufferID(unsigned int index) const;

	int getLargeur() const;
	int getHauteur() const;

private:
	GLuint m_id;
	float m_largeur;
	float m_hauteur;
	std::vector<Texture> m_colorBuffers;
	GLuint m_depthBufferID;

	bool m_utiliserStencilBuffer;
};

#endif