#include "Origine.h"



Origine::Origine() : m_shader("Shaders/couleur3D.vert", "Shaders/couleur3D.frag"), m_vboID(0), m_tailleVerticesBytes(18 * sizeof(float)), m_tailleCouleursBytes(24 * sizeof(float)), m_vaoID(0)
{
	// Chargement du shader

	m_shader.charger();

	// Vertices temporaires

	float verticesTmp[] = {
		0, 0, 0,  1, 0, 0,
		0, 0, 0,  0, 1, 0,
		0, 0, 0,  0, 0, 1,
	};

	// Couleurs temporaires

	float couleursTmp[] = {
		1, 0, 0,  1, 0, 0,
		0, 1, 0,  0, 1, 0,
		0, 0, 1,  0, 0, 1,
	};

	// Copie des valeurs

	for (int i(0); i < 18; i++)
	{
		m_vertices[i] = verticesTmp[i];
		m_couleurs[i] = couleursTmp[i];
	}
}


Origine::~Origine()
{
	// Destruction du VBO

	glDeleteBuffers(1, &m_vboID);

	// Destruction du VAO

	glDeleteVertexArrays(1, &m_vaoID);
}

void Origine::charger()
{
	// Destruction d'un �ventuel ancien VBO

	if (glIsBuffer(m_vboID) == GL_TRUE)
		glDeleteBuffers(1, &m_vboID);

	// G�n�ration de l'ID du VBO

	glGenBuffers(1, &m_vboID);

	// V�rrouillage du VBO

	glBindBuffer(GL_ARRAY_BUFFER, m_vboID);


	// Allocation de la m�moire

	glBufferData(GL_ARRAY_BUFFER, m_tailleCouleursBytes + m_tailleVerticesBytes, 0, GL_STATIC_DRAW);

	// Transfert des donn�es

	glBufferSubData(GL_ARRAY_BUFFER, 0, m_tailleVerticesBytes, m_vertices);
	glBufferSubData(GL_ARRAY_BUFFER, m_tailleVerticesBytes, m_tailleCouleursBytes, m_couleurs);


	// D�verouillage du VBO

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	// Destruction d'un �ventuel ancien VAO

	if (glIsVertexArray(m_vaoID) == GL_TRUE)
		glDeleteVertexArrays(1, &m_vaoID);

	// G�n�ration de l'ID du VAO

	glGenVertexArrays(1, &m_vaoID);

	// V�rrouillage du VAO

	glBindVertexArray(m_vaoID);

	// // On enregistre ce bout de code dans la carte graphique pour am�liorer les performances
	// Verrouillage du VBO

	glBindBuffer(GL_ARRAY_BUFFER, m_vboID);


	// Envoi des vertices dans la VRAM

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);


	// Envoi de la couleur dans la VRAM

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(m_tailleVerticesBytes));
	glEnableVertexAttribArray(1);

	// D�verrouillage du VBO

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// //
	// D�verrouillage du VAO

	glBindVertexArray(0);
}

void Origine::chargerVBO(void * donnees, int taillebytes, int decalage)
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

void Origine::afficher(glm::mat4 & projection, glm::mat4 & modelview)
{
	// Activation du shader

	glUseProgram(m_shader.getProgramID());


	// Envoi des matrices

	glUniformMatrix4fv(glGetUniformLocation(m_shader.getProgramID(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(m_shader.getProgramID(), "modelview"), 1, GL_FALSE, glm::value_ptr(modelview));


	// Verrouillage du VAO

	glBindVertexArray(m_vaoID);

	// Rendu

	glDrawArrays(GL_LINES, 0, 6);

	// D�verrouillage du VAO

	glBindVertexArray(0);


	// D�sactivation du shader

	glUseProgram(0);
}
