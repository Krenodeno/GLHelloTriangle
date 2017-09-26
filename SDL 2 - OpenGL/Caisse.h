#ifndef DEF_CAISSE
#define DEF_CAISSE

#include "Cube.h"
#include "Texture.h"

#include <string>

class Caisse :
	public Cube
{
public:
	Caisse(float taille, std::string const vertexShader, std::string const fragmentShader, std::string const texture);
	~Caisse();

	void charger();
	void afficher(glm::mat4 &projection, glm::mat4 &view);

private:
	Texture m_texture;
	float m_coordTexture[72];
	int m_tailleCoordTextureBytes;
};

#endif