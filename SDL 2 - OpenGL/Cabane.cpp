#include "Cabane.h"


Cabane::Cabane(std::string const vertexShader, std::string const fragmentShader) : m_shader(vertexShader, fragmentShader), m_textureMur("Textures/mur.jpg"), m_textureToit("Textures/toit.jpg")
{
	// Chargement du shader

	m_shader.charger();

	// Chargement des textures

	m_textureMur.charger();
	m_textureToit.charger();

	// Vertices temporaires

	float verticesTmp[] = {
		-5.0, 5.0, 5.0,   -5.0, 5.0, -5.0,   -5.0, 0.0, -5.0,	// Mur gauche
		-5.0, 5.0, 5.0,   -5.0, 0.0, 5.0,   -5.0, 0.0, -5.0,	// Mur gauche

		-5.0, 5.0, -5.0,   5.0, 5.0, -5.0,   5.0, 0.0, -5.0,	// Mur fond
		-5.0, 5.0, -5.0,   -5.0, 0.0, -5.0,   5.0, 0.0, -5.0,	// Mur fond

		5.0, 5.0, -5.0,   5.0, 5.0, 5.0,   5.0, 0.0, 5.0,		// Mur droit
		5.0, 5.0, -5.0,   5.0, 0.0, -5.0,   5.0, 0.0, 5.0,		// Mur droit

		-5.0, 5.0, -5.0,   0.0, 6.0, -5.0,   5.0, 5.0, -5.0,	// Comble

		-6.0, 4.8, -6.0,   -6.0, 4.8, 6.0,   0.0, 6.0, 6.0,		// Toit pan 1
		-6.0, 4.8, -6.0,   0.0, 6.0, -6.0,   0.0, 6.0, 6.0,		// Toit pan 1

		6.0, 4.8, -6.0,   6.0, 4.8, 6.0,   0.0, 6.0, 6.0,		// Toit pan 2
		6.0, 4.8, -6.0,   0.0, 6.0, -6.0,   0.0, 6.0, 6.0, };	// Toit pan 2


	// Coordonnées de textures temporaires

	float texturesTmp[] = {
		0, 0,  1, 0,  1, 1,		// Mur gauche
		0, 0,  0, 1,  1, 1,		// Mur gauche

		0, 0,  1, 0,  1, 1,           // Mur fond
		0, 0,  0, 1,  1, 1,           // Mur fond

		0, 0,  1, 0,  1, 1,           // Mur droit
		0, 0,  0, 1,  1, 1,           // Mur droit

		0, 0,  0.5, 0.5,  1, 0,           // Comble

		0, 0,  1, 0,  1, 1,           // Toit pan 1
		0, 0,  0, 1,  1, 1,           // Toit pan 1

		0, 0,  1, 0,  1, 1,           // Toit pan 2
		0, 0,  0, 1,  1, 1, };          // Toit pan 2


	// Copie des valeurs dans les tableaux finaux

	for (int i(0); i < 99; i++)
	{
		m_vertices[i] = verticesTmp[i];
	}
	for (int i(0); i < 66; i++)
	{
		m_coordTextures[i] = texturesTmp[i];
	}
}

Cabane::~Cabane()
{
}

void Cabane::afficher(glm::mat4 & projection, glm::mat4 & modelview)
{
	// Activation du shader

	glUseProgram(m_shader.getProgramID());


	// Envoi des vertices

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, m_vertices);
	glEnableVertexAttribArray(0);


	// Envoi des coordonnées de texture

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, m_coordTextures);
	glEnableVertexAttribArray(2);


	// Envoi des matrices

	glUniformMatrix4fv(glGetUniformLocation(m_shader.getProgramID(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(m_shader.getProgramID(), "modelview"), 1, GL_FALSE, glm::value_ptr(modelview));


	// Verrouillage de la texture

	glBindTexture(GL_TEXTURE_2D, m_textureMur.getID());


	// Rendu

	glDrawArrays(GL_TRIANGLES, 0, 21);


	// Déverrouillage de la texture

	glBindTexture(GL_TEXTURE_2D, 0);

	
	// Verrouillage de la texture

	glBindTexture(GL_TEXTURE_2D, m_textureToit.getID());


	// Rendu

	glDrawArrays(GL_TRIANGLES, 21, 12);


	// Déverrouillage de la texture

	glBindTexture(GL_TEXTURE_2D, 0);

	
	// Désactivation des tableaux

	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(0);


	// Désactivation du shader

	glUseProgram(0);
}
