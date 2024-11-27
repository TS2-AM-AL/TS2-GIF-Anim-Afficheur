#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "gif.h"
#include <math.h>

const int width = 628;
const int height = 469;
static uint8_t image[width * height * 4];  // Image pour chaque frame

// Fonction pour charger une image dans le tableau `image[]`
bool LoadImage(const char* filename) {
    int imgWidth, imgHeight, channels;
    uint8_t* imgData = stbi_load(filename, &imgWidth, &imgHeight, &channels, 4); // Convertir en RGBA

    if (imgData == nullptr) {
        printf("Erreur lors du chargement de l'image %s\n", filename);
        return false;
    }

    // Redimensionner l'image pour qu'elle tienne dans la taille du GIF
    if (imgWidth != width || imgHeight != height) {
        // Vous pouvez utiliser une bibliothèque de redimensionnement comme stb_image_resize si nécessaire
        printf("L'image a une taille différente de celle du GIF (%dx%d vs %dx%d)\n", imgWidth, imgHeight, width, height);
        stbi_image_free(imgData);
        return false;
    }

    // Copier l'image dans le tableau `image[]`
    memcpy(image, imgData, width * height * 4);  // Copier les données RGBA

    stbi_image_free(imgData);
    return true;
}

int main(int argc, const char* argv[]) {
    const char* filename = "./MyGif.gif";
    if (argc > 1) {
        filename = argv[1];
    }

    // Créer un fichier gif
    GifWriter writer = {};
    GifBegin(&writer, filename, width, height, 2, 8, true);

    // Liste de vos images à insérer dans le GIF
    const char* imageFiles[] = {
        "image0.png",
        "image1.png",
        "image2.png",
        "image3.png"
      

    };
    int numFrames = sizeof(imageFiles) / sizeof(imageFiles[0]);

    for (int frame = 0; frame < numFrames; ++frame) {
        // Charger l'image pour le frame actuel
        if (!LoadImage(imageFiles[frame])) {
            printf("Erreur lors du chargement de l'image pour le frame %d.\n", frame);
            continue;
        }

        // Écrire le frame dans le gif
        printf("Écriture du frame %d...\n", frame);
        GifWriteFrame(&writer, image, width, height, 2, 8, true);
    }

    // Terminer l'écriture du gif
    GifEnd(&writer);
    printf("GIF créé avec succès !\n");

    return 0;
}








/////////////////////////////////////////////////////////////////////////////////////////
/* #include "gif.h"

#include <math.h>

const int width = 256;
const int height = 256;
static uint8_t image[ width * height * 4 ];

void SetPixel( int xx, int yy, uint8_t red, uint8_t grn, uint8_t blu )
{
    uint8_t* pixel = &image[(yy*width+xx)*4];
    pixel[0] = red;
    pixel[1] = grn;
    pixel[2] = blu;
    pixel[3] = 255;  // no alpha for this demo
}

void SetPixelFloat( int xx, int yy, float fred, float fgrn, float fblu )
{
    // convert float to unorm
    uint8_t red = (uint8_t)roundf( 255.0f * fred );
    uint8_t grn = (uint8_t)roundf( 255.0f * fgrn );
    uint8_t blu = (uint8_t)roundf( 255.0f * fblu );
    
    SetPixel( xx, yy, red, grn, blu );
}

int main(int argc, const char * argv[])
{
    const char* filename = "./MyGif.gif";
    if( argc > 1 )
    {
        filename = argv[1];
    }
    
    // Create a gif
    GifWriter writer = {};
    GifBegin( &writer, filename, width, height, 2, 8, true );
    
    for( int frame=0; frame<256; ++frame )
    {
        
        // Make an image, somehow
        // this is the default shadertoy - credit to shadertoy.com
        float tt = frame * 3.14159f * 2 / 255.0f;
        for( int yy=0; yy<height; ++yy )
        {
            for( int xx=0; xx<width; ++xx )
            {
                float fx = xx / (float)width;
                float fy = yy / (float)height;
                
                float red = 0.5f + 0.5f * cosf(tt+fx);
                float grn = 0.5f + 0.5f * cosf(tt+fy+2.f);
                float blu = 0.5f + 0.5f * cosf(tt+fx+4.f);
                
                SetPixelFloat( xx, yy, red, grn, blu );
            }
        }
        
        
        // Write the frame to the gif
        printf( "Writing frame %d...\n", frame );
        GifWriteFrame( &writer, image, width, height, 2, 8, true );
    }
    
    // Write EOF
    GifEnd( &writer );
    
    return 0;
}
*/