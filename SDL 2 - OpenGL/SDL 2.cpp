#include "SceneOpenGL.h"

int main(int argc, char **argv)
{
	//Création de la scène

	SceneOpenGL scene("Chap 11", 1024, 768);

	//Initialisation de la scène

	if (scene.initialiserFenetre() == false)
		return -1;

	if (scene.initGL() == false)
		return -1;

	//Boucle principale

	scene.bouclePrincipale();

	//Fin du programme

	return 0;
}