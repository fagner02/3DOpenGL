#include <stdio.h>

#include "texture_picking.h"

Picking::~Picking() {
    if (fbo != 0) {
        glDeleteFramebuffers(1, &fbo);
    }

    if (pickingTexture != 0) {
        glDeleteTextures(1, &pickingTexture);
    }

    if (depthTexture != 0) {
        glDeleteTextures(1, &depthTexture);
    }
}

void Picking::init(unsigned int width, unsigned int height) {
    // Create the FBO
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    // Create the texture object for the primitive information buffer
    // glGenTextures(1, &m_pickingTexture);
    // glBindTexture(GL_TEXTURE_2D, m_pickingTexture);
    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_pickingTexture, 0);

    // // Create the texture object for the depth buffer
    glGenTextures(1, &depthTexture);
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);

    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA32UI, width, height);

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
    glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, rbo);

    // Verify that the FBO is correct
    GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (Status != GL_FRAMEBUFFER_COMPLETE) {
        printf("FB error, status: 0x%x\n", Status);
        exit(1);
    }

    // Restore the default framebuffer
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void Picking::enableWriting() {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
}


void Picking::disableWriting() {
    // Bind back the default framebuffer
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}


PixelInfo Picking::ReadPixel(unsigned int x, unsigned int y, unsigned int width, unsigned int height) {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);

    glReadBuffer(GL_COLOR_ATTACHMENT0);

    // unsigned int pixel[4];
    PixelInfo pixel;
    glReadPixels(x, y, 1, 1, GL_RGBA_INTEGER, GL_INT, &pixel);

    glReadBuffer(GL_NONE);

    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

    return pixel;
}

void Picking::applyIndex(int shaderProgram, int index) {
    int loc = glGetUniformLocation(shaderProgram, "gObjectIndex");
    glUniform1ui(loc, 1);
}