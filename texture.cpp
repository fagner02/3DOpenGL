#include "texture.h"
Texture::Texture(const char* filename) {
    unsigned int image;
    ilGenImages(1, &image);
    ilBindImage(image);
    if (!ilLoadImage(filename)) {
        std::cout << "Error loading texture: " << std::string(filename) << "\n";
    }

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, GL_RGBA, GL_UNSIGNED_BYTE, ilGetData());
    glGenerateMipmap(GL_TEXTURE_2D);

    ilDeleteImage(image);
}

void Texture::bindTexture() {
    glBindTexture(GL_TEXTURE_2D, texture);
}