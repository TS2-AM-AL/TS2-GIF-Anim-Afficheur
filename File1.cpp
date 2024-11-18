#pragma hdrstop
#pragma argsused

#ifdef _WIN32
#include <tchar.h>
#else
  typedef char _TCHAR;
  #define _tmain main
#endif

#include <stdio.h>
#include <gif_lib.h>
#include <iostream>
#include <vector>
#include <cstdlib>

// Définir GIF89a si nécessaire (pour compatibilité)
#ifndef GIF89a
#define GIF89a 0xF9
#endif

// Fonction pour créer une image simple (une couleur unie)
void create_image(std::vector<unsigned char>& buffer, int width, int height, unsigned char r, unsigned char g, unsigned char b) {
    for (int i = 0; i < width * height; i++) {
        buffer[i * 3 + 0] = r; // Composant Rouge
        buffer[i * 3 + 1] = g; // Composant Vert
        buffer[i * 3 + 2] = b; // Composant Bleu
    }
}

int main() {
    // Dimensions de l'image GIF
    int width = 100;
    int height = 100;

    // Créer un buffer pour l'image
    std::vector<unsigned char> buffer(width * height * 3);

    // Créer un fichier GIF
    const char* filename = "output.gif";
    int error;
    GifFileType* gif = EGifOpenFileName(filename, false, &error);
    if (!gif) {
        std::cerr << "Erreur lors de l'ouverture du fichier GIF." << std::endl;
        return 1;
    }

    // Créer la palette de couleurs (256 couleurs en niveaux de gris)
	GifColorType* colorMap = (GifColorType*)malloc(256 * sizeof(GifColorType));
    if (!colorMap) {
        std::cerr << "Erreur d'allocation de mémoire pour la palette." << std::endl;
        return 1;
    }

    // Initialiser la palette avec des couleurs en niveaux de gris
    for (int i = 0; i < 256; i++) {
        colorMap[i].Red = i;   // Rouge
        colorMap[i].Green = i; // Vert
        colorMap[i].Blue = i;  // Bleu
    }

    // Définir la palette et les autres propriétés du GIF
	GifScreenDesc screenDesc = { width, height, 256, false, nullptr };
    gif->SWidth = screenDesc.SWidth;
    gif->SHeight = screenDesc.SHeight;
	gif->SColorMap = GifMakeMapObject(256, colorMap); // Ajouter la palette au GIF

    // Boucle pour ajouter quelques images au GIF
    for (int frame = 0; frame < 5; ++frame) {
        // Créer une nouvelle image avec une couleur différente pour chaque frame
        create_image(buffer, width, height, frame * 50, 0, 255 - frame * 50);

        // Créer un ImageDescriptor et ajouter l'image
        GifImageDesc imgDesc = {0, 0, width, height, false, gif->SColorMap};
        EGifPutImageDesc(gif, imgDesc.Left, imgDesc.Top, imgDesc.Width, imgDesc.Height, imgDesc.Interlace);

        // Remplir la palette avec les indices des couleurs (en niveaux de gris ici)
        for (int i = 0; i < width * height; i++) {
            // Remplacer chaque pixel avec un indice de la palette
            buffer[i] = static_cast<unsigned char>(buffer[i * 3 + 0]); // Utiliser la composante rouge comme index
        }

        // Ajouter les pixels au GIF (en utilisant la palette)
        EGifPutLine(gif, &buffer[0], width * height);

        // Ajouter une pause entre les images (10 centièmes de seconde)
        int delay = 10;
        EGifPutExtension(gif, 0xF9, 4, reinterpret_cast<GifByteType*>(&delay));
    }

    // Terminer le fichier GIF
    EGifCloseFile(gif, &error);

    std::cout << "GIF créé avec succès : " << filename << std::endl;

    // Libérer la mémoire allouée à la palette
	free(colorMap);

    return 0;
}

