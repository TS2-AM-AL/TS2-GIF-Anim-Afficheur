                                                      extern "C" {
	#include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libavutil/avutil.h>
    #include <libavutil/imgutils.h>
}

#include <iostream>
#include <string>

void convertVideoToGif(const std::string &inputFile, const std::string &outputFile) {
    av_register_all();
    avformat_network_init();

    AVFormatContext* formatContext = nullptr;
    if (avformat_open_input(&formatContext, inputFile.c_str(), nullptr, nullptr) < 0) {
        std::cerr << "Erreur d'ouverture du fichier vidéo!" << std::endl;
        return;
	}

    if (avformat_find_stream_info(formatContext, nullptr) < 0) {
        std::cerr << "Erreur lors de la récupération des informations du flux!" << std::endl;
		return;
    }

	AVCodec* codec = nullptr;
    AVCodecContext* codecContext = nullptr;
	int videoStreamIndex = -1;

    for (int i = 0; i < formatContext->nb_streams; i++) {
        if (formatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoStreamIndex = i;
            codec = avcodec_find_decoder(formatContext->streams[i]->codecpar->codec_id);
            codecContext = avcodec_alloc_context3(codec);
            avcodec_parameters_to_context(codecContext, formatContext->streams[i]->codecpar);
            break;
        }
    }

    if (videoStreamIndex == -1 || codec == nullptr) {
        std::cerr << "Vidéo introuvable dans le fichier!" << std::endl;
        return;
    }

    if (avcodec_open2(codecContext, codec, nullptr) < 0) {
        std::cerr << "Erreur d'ouverture du codec!" << std::endl;
        return;
    }

    // Prépare les structures nécessaires pour l'encodage GIF
    AVPacket packet;
    av_init_packet(&packet);

    AVFrame* frame = av_frame_alloc();
    AVFrame* rgbFrame = av_frame_alloc();
    int width = codecContext->width;
    int height = codecContext->height;

    // Allocation d'un tampon pour l'image RGB
    int numBytes = av_image_get_buffer_size(AV_PIX_FMT_RGB24, width, height, 1);
    uint8_t* buffer = (uint8_t*) av_malloc(numBytes * sizeof(uint8_t));
    av_image_fill_arrays(rgbFrame->data, rgbFrame->linesize, buffer, AV_PIX_FMT_RGB24, width, height, 1);

    // Crée un fichier GIF
    // (Tu devras utiliser une bibliothèque comme GIFLIB ou écrire une fonction pour encoder des GIFs à partir de frames ici)
    // Tu peux aussi utiliser une solution comme ffmpeg en ligne de commande pour cela

    int frameCount = 0;
    while (av_read_frame(formatContext, &packet) >= 0) {
        if (packet.stream_index == videoStreamIndex) {
            int response = avcodec_receive_frame(codecContext, frame);
            if (response == 0) {
                // Convertir la frame en RGB
                sws_scale(
                    sws_getContext(codecContext->width, codecContext->height, codecContext->pix_fmt,
                                   width, height, AV_PIX_FMT_RGB24, SWS_BICUBIC, nullptr, nullptr, nullptr),
                    frame->data, frame->linesize, 0, height, rgbFrame->data, rgbFrame->linesize
                );

                // Sauvegarde chaque image RGB en GIF (utilise GIFLIB ou autre méthode pour l'écriture du GIF)
                // Ajoute ici la logique pour ajouter chaque frame à un fichier GIF.
                std::cout << "Frame " << frameCount++ << " ajoutée." << std::endl;
            }
        }
        av_packet_unref(&packet);
    }

    // Libération de la mémoire
    av_frame_free(&frame);
    av_frame_free(&rgbFrame);
    avcodec_free_context(&codecContext);
    avformat_close_input(&formatContext);
}

int main() {
    std::string inputFile = "input_video.mp4";
    std::string outputFile = "output.gif";
    convertVideoToGif(inputFile, outputFile);
    return 0;
}
