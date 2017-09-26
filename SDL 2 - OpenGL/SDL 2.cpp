#include "SceneOpenGL.h"

int main(int argc, char **argv)
{
	//Cr�ation de la sc�ne

	SceneOpenGL scene("Chap 11", 1024, 768);

	//Initialisation de la sc�ne

	if (scene.initialiserFenetre() == false)
		return -1;

	if (scene.initGL() == false)
		return -1;

	//Boucle principale

	scene.bouclePrincipale();

	//Fin du programme

	return 0;
}