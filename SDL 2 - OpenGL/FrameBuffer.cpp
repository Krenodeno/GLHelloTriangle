#include "FrameBuffer.h"



FrameBuffer::FrameBuffer() : m_id(0), m_largeur(0), m_hauteur(0), m_colorBuffers(0), m_depthBufferID(0), m_utiliserStencilBuffer(false)
{

}

FrameBuffer::FrameBuffer(int largeur, int hauteur, bool utiliserStencilBuffer) : m_id(0), m_largeur(largeur), m_hauteur(hauteur),
m_colorBuffers(0), m_depthBufferID(0), m_utiliserStencilBuffer(utiliserStencilBuffer)
{

}

FrameBuffer::FrameBuffer(FrameBuffer const & frameBufferACopier)
{
	// Copie de la largeur, de la hauteur et du booléen

	m_largeur = frameBufferACopier.m_largeur;
	m_hauteur = frameBufferACopier.m_hauteur;
	m_utiliserStencilBuffer = frameBufferACopier.m_utiliserStencilBuffer;

	charger();
}


FrameBuffer::~FrameBuffer()
{
	// Destruction des buffers

	glDeleteFramebuffers(1, &m_id);
	glDeleteRenderbuffers(1, &m_depthBufferID);

	m_colorBuffers.clear();
}

bool FrameBuffer::charger()
{
	// Destruction d'un éventuel ancien Buffer

	if (glIsFramebuffer(m_id) == GL_TRUE)
	{
		// Destruction Frame buffer

		glDeleteFramebuffers(1, &m_id);

		// Libération des Color Buffers

		m_colorBuffers.clear();
	}

	// Génération de l'identifiant

	glGenFramebuffers(1, &m_id);

	// Verrouillage du Frame Buffer

	glBindFramebuffer(GL_FRAMEBUFFER, m_id);

	// Création du Color Buffer

	Texture colorBuffer(m_largeur, m_hauteur, GL_RGBA, GL_RGBA, true);
	colorBuffer.chargerTextureVide();

	// Ajout au tableau

	m_colorBuffers.push_back(colorBuffer);

	// Création du Depth et du Stencil Buffer (si besoin)

	if (m_utiliserStencilBuffer)
		creerRenderBuffer(m_depthBufferID, GL_DEPTH24_STENCIL8);
	else
		creerRenderBuffer(m_depthBufferID, GL_DEPTH_COMPONENT24);

	// Association du Color Buffer

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorBuffers[0].getID(), 0);

	// Association du Depth et du Stencil Buffer (si besoin)

	if (m_utiliserStencilBuffer)
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depthBufferID);
	else
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthBufferID);


	// Vérification de l'intégrité du FBO

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		// Libération des buffers

		glDeleteFramebuffers(1, &m_id);
		glDeleteRenderbuffers(1, &m_depthBufferID);

		m_colorBuffers.clear();

		// Affichage d'un message d'erreur et retour de la valeur false

		std::cout << "Erreur : le FBO est mal construit" << std::endl;

		return false;
	}

	// Déverrouillage du Frame Buffer
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Si tout s'est bient passé, on renvoie la valeur true

	return true;
}

void FrameBuffer::creerRenderBuffer(GLuint & id, GLenum formatInterne)
{
	// Destruction d'un éventuel ancien Render Buffer

	if (glIsRenderbuffer(id) == GL_TRUE)
		glDeleteRenderbuffers(1, &id);

	// Génération de l'identifiant

	glGenRenderbuffers(1, &id);

	// Verrouillage

	glBindRenderbuffer(GL_RENDERBUFFER, id);

	// Configuration du Render Buffer

	glRenderbufferStorage(GL_RENDERBUFFER, formatInterne, m_largeur, m_hauteur);


	// Déverrouillage

	glBindRenderbuffer(GL_RENDERBUFFER, 0);

}

GLuint FrameBuffer::getID() const
{
	return m_id;
}

GLuint FrameBuffer::getColorBufferID(unsigned int index) const
{
	return m_colorBuffers[index].getID();
}

int FrameBuffer::getLargeur() const
{
	return m_largeur;
}

int FrameBuffer::getHauteur() const
{
	return m_hauteur;
}
