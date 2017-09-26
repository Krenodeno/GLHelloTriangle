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


	// Coordonn�es de textures temporaires

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

	// Destruction d'un �ventuel ancien VBO

	if (glIsBuffer(m_vboID) == GL_TRUE)
		glDeleteBuffers(1, &m_vboID);

	// G�n�ration de l'ID

	glGenBuffers(1, &m_vboID);

	// V�rrouillage du VBO

	glBindBuffer(GL_ARRAY_BUFFER, m_vboID);


	// Allocation de la m�moire

	glBufferData(GL_ARRAY_BUFFER, m_tailleVerticesBytes + m_tailleCoordTextureBytes, 0, GL_STATIC_DRAW);

	// Transfert des donn�es

	glBufferSubData(GL_ARRAY_BUFFER, 0, m_tailleVerticesBytes, m_vertices);
	glBufferSubData(GL_ARRAY_BUFFER, m_tailleVerticesBytes, m_tailleCoordTextureBytes, m_coordTexture);


	// D�verouillage du VBO

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	// Destruction d'un �ventuel ancien VAO

	if (glIsVertexArray(m_vaoID) == GL_TRUE)
		glDeleteVertexArrays(1, &m_vaoID);

	// G�n�ration de l'ID du VAO

	glGenVertexArrays(1, &m_vaoID);

	// Verrouillage du VAO

	glBindVertexArray(m_vaoID);

	// //
	// V�rrouillage du VBO

	glBindBuffer(GL_ARRAY_BUFFER, m_vboID);

	// // //
	// Acc�s des vertices depuis la VRAM

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);


	// Acc�s des coordonn�es de texture depuis la VRAM

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(m_tailleVerticesBytes));
	glEnableVertexAttribArray(2);

	// // //
	// D�verrouillage du VBO

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// //
	// D�verrouillage du VAO

	glBindVertexArray(0);
}

void Cristal::chargerVBO(void * donnees, int taillebytes, int decalage)
{

	// V�rrouillage du VBO

	glBindBuffer(GL_ARRAY_BUFFER, m_vboID);

	// R�cup�ration de l'adresse du VBO

	void * adresseVBO = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

	if (adresseVBO == NULL) {
		std::cout << "Erreur au niveau de la r�cup�ration du VBO" << std::endl;
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		return;
	}

	// Mise � jour des donn�es

	memcpy((char*)adresseVBO + decalage, donnees, taillebytes);

	// Annulation du pointeur

	glUnmapBuffer(GL_ARRAY_BUFFER);
	adresseVBO = 0;

	// D�verrouillage du VBO

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

	// D�verrouillage du VAO

	glBindVertexArray(0);

	// D�verrouillage de la texture

	glBindTexture(GL_TEXTURE_2D, 0);

	// D�sactivation du shader

	glUseProgram(0);

	// Restauration de la matrice

	m_model = sauvegardeModel;
}
