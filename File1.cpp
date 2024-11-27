#include "gif_lib.h"
#include <vector>
#include <iostream>

int main() {
    const int width = 100;    // Largeur de l'image
    const int height = 100;   // Hauteur de l'image
    const int numFrames = 10; // Nombre de frames dans l'animation
    const int delay = 10;     // Délai entre chaque image (en centièmes de seconde)

    // Créer un fichier GIF
	GifFileType* gifFile = EGifOpenFileName("fire.gif", false, nullptr);
    if (gifFile == nullptr) {
        std::cerr << "Erreur lors de l'ouverture du fichier GIF." << std::endl;
        return 1;
    }

	// Créer une palette de couleurs pour le GIF (256 couleurs)
    ColorMapObject* colorMap = GifMakeMapObject(256, nullptr);
    if (colorMap == nullptr) {
		std::cerr << "Erreur lors de la création de la palette." << std::endl;
        EGifCloseFile(gifFile, nullptr);
        return 1;
    }

    EGifSetGifVersion(gifFile, "89a");

    // Créer une image GIF pour chaque frame
    for (int frame = 0; frame < numFrames; ++frame) {
        // Créer une image avec des pixels fictifs (exemple)
        std::vector<unsigned char> pixels(width * height, 255);  // Tous les pixels sont blancs
        pixels[frame * 10] = 0;  // Un pixel devient noir à chaque frame

        // Créer une image GIF avec les pixels
        GifImage* gifImage = EGifMakeImageObject(width, height, pixels.data(), colorMap);
        if (gifImage == nullptr) {
            std::cerr << "Erreur lors de la création de l'image." << std::endl;
            EGifCloseFile(gifFile, nullptr);
            return 1;
        }

        // Ajouter cette image au fichier GIF
        if (EGifAddFrame(gifFile, gifImage, delay) != GIF_OK) {
            std::cerr << "Erreur lors de l'ajout de la frame au GIF." << std::endl;
            EGifCloseFile(gifFile, nullptr);
            return 1;
        }

		delete gifImage; // Libérer la mémoire de l'image
    }

    // Fermer le fichier GIF
    EGifCloseFile(gifFile, nullptr);

    std::cout << "GIF créé avec succès!" << std::endl;
    return 0;
}

