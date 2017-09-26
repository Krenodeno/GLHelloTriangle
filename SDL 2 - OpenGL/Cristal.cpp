#include "Cristal.h"


Cristal::Cristal(std::string const vertexShader, std::string const fragmentShader, std::string const texture) : m_shader(vertexShader, fragmentShader), m_texture("Textures/cristal.jpg"), angle(0.0)
{
	// Chargement du shader

	m_shader.charger();

	// Chargement de la texture

	m_texture.charger();

	// Vertices temporaires

	float verticesTmp[] = {
		-0.5, 0, 0.5,   0, 1, 0,   0.5, 0, 0.5,		// Partie haute
		0.5, 0, 0.5,   0, 1, 0,   0.5, 0, -0.5,
		0.5, 0, -0.5,  0, 1, 0,  -0.5, 0, -0.5,
		-0.5, 0, -0.5,  0, 1, 0,  -0.5, 0, 0.5,

		-0.5, 0, 0.5,   0, -1, 0,   0.5, 0, 0.5,	// Partie basse
		0.5, 0, 0.5,   0, -1, 0,   0.5, 0, -0.5,
		0.5, 0, -0.5,  0, -1, 0,  -0.5, 0, -0.5,
		-0.5, 0, -0.5,  0, -1, 0,  -0.5, 0, 0.5,
	};


	// Coordonnées de textures temporaires

	float texturesTmp[] = {
		0, 0,  0.5, 0.5,  0.5, 0,
	};


	// Copie des valeurs dans les tableaux finaux

	for (int i(0); i < 72; i++)
	{
		m_vertices[i] = verticesTmp[i];
	}
	for (int i(0); i < 48; i++)
	{
		m_coordTexture[i] = texturesTmp[i % 6];
	}
}

Cristal::~Cristal()
{
}

void Cristal::charger()
{

	// Destruction d'un éventuel ancien VBO

	if (glIsBuffer(m_vboID) == GL_TRUE)
		glDeleteBuffers(1, &m_vboID);

	// Génération de l'ID

	glGenBuffers(1, &m_vboID);

	// Vérrouillage du VBO

	glBindBuffer(GL_ARRAY_BUFFER, m_vboID);


	// Allocation de la mémoire

	glBufferData(GL_ARRAY_BUFFER, m_tailleVerticesBytes + m_tailleCoordTextureBytes, 0, GL_STATIC_DRAW);

	// Transfert des données

	glBufferSubData(GL_ARRAY_BUFFER, 0, m_tailleVerticesBytes, m_vertices);
	glBufferSubData(GL_ARRAY_BUFFER, m_tailleVerticesBytes, m_tailleCoordTextureBytes, m_coordTexture);


	// Déverouillage du VBO

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	// Destruction d'un éventuel ancien VAO

	if (glIsVertexArray(m_vaoID) == GL_TRUE)
		glDeleteVertexArrays(1, &m_vaoID);

	// Génération de l'ID du VAO

	glGenVertexArrays(1, &m_vaoID);

	// Verrouillage du VAO

	glBindVertexArray(m_vaoID);

	// //
	// Vérrouillage du VBO

	glBindBuffer(GL_ARRAY_BUFFER, m_vboID);

	// // //
	// Accès des vertices depuis la VRAM

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);


	// Accès des coordonnées de texture depuis la VRAM

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(m_tailleVerticesBytes));
	glEnableVertexAttribArray(2);

	// // //
	// Déverrouillage du VBO

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// //
	// Déverrouillage du VAO

	glBindVertexArray(0);
}

void Cristal::chargerVBO(void * donnees, int taillebytes, int decalage)
{

	// Vérrouillage du VBO

	glBindBuffer(GL_ARRAY_BUFFER, m_vboID);

	// Récupération de l'adresse du VBO

	void * adresseVBO = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

	if (adresseVBO == NULL) {
		std::cout << "Erreur au niveau de la récupération du VBO" << std::endl;
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		return;
	}

	// Mise à jour des données

	memcpy((char*)adresseVBO + decalage, donnees, taillebytes);

	// Annulation du pointeur

	glUnmapBuffer(GL_ARRAY_BUFFER);
	adresseVBO = 0;

	// Déverrouillage du VBO

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Cristal::afficher(glm::mat4 & projection, glm::mat4 & view)
{
	// Sauvegarde de la matrice modelview

	glm::mat4 sauvegardeModel = m_model;

	// Rotation pour positionner le cristal

	angle += 1.0;
	if (angle >= 360.0)
		angle -= 360.0;

	m_model = glm::rotate(m_model, angle, glm::vec3(0, 1, 0));

	// Activation du shader

	glUseProgram(m_shader.getProgramID());

	// Envoi des matrices

	m_shader.envoyerMat4("modelviewProjection", projection * view * m_model);

	// Verrouillage de la texture

	glBindTexture(GL_TEXTURE_2D, m_texture.getID());

	// Verrouillage du VAO

	glBindVertexArray(m_vaoID);

	// Rendu

	glDrawArrays(GL_TRIANGLES, 0, 24);

	// Déverrouillage du VAO

	glBindVertexArray(0);

	// Déverrouillage de la texture

	glBindTexture(GL_TEXTURE_2D, 0);

	// Désactivation du shader

	glUseProgram(0);

	// Restauration de la matrice

	m_model = sauvegardeModel;
}
