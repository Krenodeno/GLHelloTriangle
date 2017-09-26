#include "Sol.h"


Sol::Sol(std::string const vertexShader, std::string const fragmentShader, std::string texture, int taille = 1) : m_shader(vertexShader, fragmentShader), m_texture(texture)
{
	// Chargement du shader

	m_shader.charger();

	// Chargement de la texture

	m_texture.charger();
	
	// Division de la taille

	taille /= 2;

	// Vertices temporaires

	float verticesTmp[] = {
		-taille, 0, -taille,   taille, 0, -taille,   taille, 0, taille,
		-taille, 0, -taille,   -taille, 0, taille,   taille, 0, taille,
	};


	// Coordonnées de textures temporaires

	float texturesTmp[] = {
		0, 0,  taille, 0,  taille, taille,
		0, 0,  0, taille,  taille, taille,
	};


	// Copie des valeurs dans les tableaux finaux

	for (int i(0); i < 18; i++)
	{
		m_vertices[i] = verticesTmp[i];
	}
	for (int i(0); i < 12; i++)
	{
		m_coordTexture[i] = texturesTmp[i];
	}
}

Sol::~Sol()
{
}

void Sol::afficher(glm::mat4 & projection, glm::mat4 & modelview)
{
	// Activation du shader

	glUseProgram(m_shader.getProgramID());


	// Envoi des vertices

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, m_vertices);
	glEnableVertexAttribArray(0);


	// Envoi des coordonnées de texture

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, m_coordTexture);
	glEnableVertexAttribArray(2);


	// Envoi des matrices

	glUniformMatrix4fv(glGetUniformLocation(m_shader.getProgramID(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(m_shader.getProgramID(), "modelview"), 1, GL_FALSE, glm::value_ptr(modelview));


	// Verrouillage de la texture

	glBindTexture(GL_TEXTURE_2D, m_texture.getID());


	// Rendu

	glDrawArrays(GL_TRIANGLES, 0, 6);


	// Déverrouillage de la texture

	glBindTexture(GL_TEXTURE_2D, 0);


	// Désactivation des tableaux

	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(0);


	// Désactivation du shader

	glUseProgram(0);
}
