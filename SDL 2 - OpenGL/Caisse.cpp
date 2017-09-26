#include "Caisse.h"



Caisse::Caisse(float taille, std::string const vertexShader, std::string const fragmentShader, std::string const texture) 
	:Cube(taille, vertexShader, fragmentShader), m_texture(texture), m_tailleCoordTextureBytes(72 * sizeof(float))
{
	//Chargement de la texture

	m_texture.charger();

	// Coordonn�es de texture temporaires

	float coordTextureTmp[] = {
		0, 0,  1, 0,  1, 1,
		0, 0,  0, 1,  1, 1 };

	// Copie des valeurs dans le tableau final

	for (int i = 0; i < 72; i++)
		m_coordTexture[i] = coordTextureTmp[i % 12];
}

Caisse::~Caisse()
{
}

void Caisse::charger() {

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

void Caisse::afficher(glm::mat4 & projection, glm::mat4 & view)
{

	// Activation du shader

	glUseProgram(m_shader.getProgramID());

	// Envoi des matrices

	m_shader.envoyerMat4("modelviewProjection", projection * view * m_model);

	// Verrouillage de la texture

	glBindTexture(GL_TEXTURE_2D, m_texture.getID());

	// Verrouillage du VAO

	glBindVertexArray(m_vaoID);

	// Rendu

	glDrawArrays(GL_TRIANGLES, 0, 36);

	// D�verrouilage du VAO

	glBindVertexArray(0);

	// D�verrouillage de la texture

	glBindTexture(GL_TEXTURE_2D, 0);


	// D�sactivation du shader

	glUseProgram(0);

}
