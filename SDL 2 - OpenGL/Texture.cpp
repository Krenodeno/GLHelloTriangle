#include "Texture.h"


// Constructeurs

Texture::Texture() : m_id(0), m_fichierImage("") 
{
}

Texture::Texture(std::string fichierImage) : m_id(0), m_fichierImage(fichierImage), m_largeur(0), m_hauteur(0), m_format(0), m_formatInterne(0), m_textureVide(false)
{
}

Texture::Texture(int largeur, int hauteur, GLenum format, GLenum formatInterne, bool textureVide) : m_id(0), m_fichierImage(""), m_largeur(largeur), 
m_hauteur(hauteur), m_format(format), m_formatInterne(formatInterne), m_textureVide(textureVide)
{

}

Texture::Texture(Texture const & textureACopier)
{
	// Copie des attributs

	m_fichierImage = textureACopier.m_fichierImage;

	m_largeur = textureACopier.m_largeur;
	m_hauteur = textureACopier.m_hauteur;
	m_format = textureACopier.m_format;
	m_formatInterne = textureACopier.m_formatInterne;
	m_textureVide = textureACopier.m_textureVide;

	// Si la texture est vide, alors on appelle la méthode chargerTextureVide()

	if (m_textureVide && glIsTexture(textureACopier.m_id) == GL_TRUE)
		chargerTextureVide();


	// Sinon, on appelle la méthode charger() par défaut

	else if(glIsTexture(textureACopier.m_id) == GL_TRUE)
		charger();
}


// Destructeur

Texture::~Texture()
{
	// Destruction de la texture

	glDeleteTextures(1, &m_id);
}


// Méthodes

bool Texture::charger()
{
	// Chargement de l'image dans une surface SDL

	SDL_Surface *imageSDL = IMG_Load(m_fichierImage.c_str());

	if (imageSDL == 0)
	{
		std::cout << "Erreur : " << SDL_GetError() << std::endl;
		return false;
	}

	// Inversion de l'image

	SDL_Surface *imageInversee = inverserPixels(imageSDL);
	SDL_FreeSurface(imageSDL);

	// Destruction d'une éventuelle ancienne texture

	if (glIsTexture(m_id) == GL_TRUE)
		glDeleteTextures(1, &m_id);

	// Génération de l'ID

	glGenTextures(1, &m_id);

	// Verrouillage

	glBindTexture(GL_TEXTURE_2D, m_id);

	// Format de l'image

	GLenum formatInterne(0);
	GLenum format(0);

	// Détermination du format et du format interne pour les images é 3 composantes

	if (imageInversee->format->BytesPerPixel == 3)
	{
		// Format interne

		formatInterne = GL_RGB;

		// Format

		if (imageInversee->format->Rmask == 0xff)
			format = GL_RGB;
		else
			format = GL_BGR;
	}

	// Détermination du format et du format interne pour les images é 4 composantes

	else if (imageInversee->format->BytesPerPixel == 4)
	{
		// Format interne

		formatInterne = GL_RGBA;

		// Format

		if (imageInversee->format->Rmask == 0xff)
			format = GL_RGBA;
		else
			format = GL_BGRA;
	}

	// Dans les autres cas, on arréte le chargement

	else
	{
		std::cout << "Erreur, format interne de l'image inconnu" << std::endl;
		SDL_FreeSurface(imageInversee);

		return false;
	}

	// Copie des pixels

	glTexImage2D(GL_TEXTURE_2D, 0, formatInterne, imageInversee->w, imageInversee->h, 0, format, GL_UNSIGNED_BYTE, imageInversee->pixels);

	// Application des filtres

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// Proche -> beau mais demandeur en ressources
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);	// Loin -> moche mais léger en ressources

	// Déverrouillage

	glBindTexture(GL_TEXTURE_2D, 0);

	// Fin de la méthode

	SDL_FreeSurface(imageInversee);
	return true;
}

void Texture::chargerTextureVide()
{
	// Destruction d'une éventuelle ancienne texture

	if (glIsTexture(m_id) == GL_TRUE)
		glDeleteTextures(1, &m_id);


	// Génération de l'ID

	glGenTextures(1, &m_id);

	// Verrouillage de la texture

	glBindTexture(GL_TEXTURE_2D, m_id);

	// Définition des caractéristiques de la texture

	glTexImage2D(GL_TEXTURE_2D, 0, m_formatInterne, m_largeur, m_hauteur, 0, m_format, GL_UNSIGNED_BYTE, 0);

	// Application des filtres

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


	// Déverrouillage de la texture

	glBindTexture(GL_TEXTURE_2D, 0);
}


GLuint Texture::getID() const
{
	return m_id;
}


void Texture::setFichierImage(const std::string &fichierImage)
{
	m_fichierImage = fichierImage;
}


SDL_Surface* Texture::inverserPixels(SDL_Surface *imageSource) const
{
	// Copie conforme de l'image source sans les pixels

	SDL_Surface *imageInversee = SDL_CreateRGBSurface(0, imageSource->w, imageSource->h, imageSource->format->BitsPerPixel, imageSource->format->Rmask,
		imageSource->format->Gmask, imageSource->format->Bmask, imageSource->format->Amask);


	// Tableau intermédiaires permettant de manipuler les pixels

	unsigned char* pixelsSources = (unsigned char*)imageSource->pixels;
	unsigned char* pixelsInverses = (unsigned char*)imageInversee->pixels;


	// Inversion des pixels

	for (int i = 0; i < imageSource->h; i++)
	{
		for (int j = 0; j < imageSource->w * imageSource->format->BytesPerPixel; j++)
			pixelsInverses[(imageSource->w * imageSource->format->BytesPerPixel * (imageSource->h - 1 - i)) + j] = pixelsSources[(imageSource->w * imageSource->format->BytesPerPixel * i) + j];
	}


	// Retour de l'image inversée

	return imageInversee;
}


Texture & Texture::operator=(Texture const & textureACopier)
{
	// Copie des attributs

	m_fichierImage = textureACopier.m_fichierImage;

	m_largeur = textureACopier.m_largeur;
	m_hauteur = textureACopier.m_hauteur;
	m_format = textureACopier.m_format;
	m_formatInterne = textureACopier.m_formatInterne;
	m_textureVide = textureACopier.m_textureVide;

	// Si la texture est vide, alors on appelle la méthode chargerTextureVide()

	if (m_textureVide && glIsTexture(textureACopier.m_id) == GL_TRUE)
		chargerTextureVide();


	// Sinon, on appelle la méthode charger() par défaut

	else if (glIsTexture(textureACopier.m_id) == GL_TRUE)
		charger();

	// Retour du pointeur *this

	return *this;
}
