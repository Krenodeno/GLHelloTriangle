#include "Model.h"



Model::Model(float taille, std::string const vertexShader, std::string const fragmentShader, std::string const texture)
	:m_shader(vertexShader, fragmentShader), m_texture(texture), m_vboID(0), m_vaoID(0)
{
	// Chargement de la texture

	m_texture.charger();
}

Model::~Model()
{
	// Destruction du VBO

	glDeleteBuffers(1, &m_vboID);

	// Destruction du VAO

	glDeleteVertexArrays(1, &m_vaoID);

}

void Model::charger()
{
}

void Model::chargerVBO(void * donnees, int taillebytes, int decalage)
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

void Model::afficher(glm::mat4 & projection, glm::mat4 & modelview)
{
	// Activation du shader

	glUseProgram(m_shader.getProgramID());


	// Envoi des matrices

	glUniformMatrix4fv(glGetUniformLocation(m_shader.getProgramID(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(m_shader.getProgramID(), "modelview"), 1, GL_FALSE, glm::value_ptr(modelview));


	// Verrouillage du VAO

	glBindVertexArray(m_vaoID);

	// Rendu

	glDrawArrays(GL_TRIANGLES, 0, m_vertices.size() /3);

	// D�verrouillage du VAO

	glBindVertexArray(0);


	// D�sactivation du shader

	glUseProgram(0);
}
