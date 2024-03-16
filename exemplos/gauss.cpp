void GUI::drawQuad(float width, float height, float discrWidth, float discrHeight, float texWidth, float texHeight, bool inverted)
{
    int nWidth = width/discrWidth;
    int nHeight = height/discrHeight;
    discrWidth = width/nWidth; //correcao necessaria, pois, caso width/discrWidth nao seja inteiro, nWidth*discrWidth (feito pelo for) nao completara exatamente a width
    discrHeight = height/nHeight; //correcao necessaria, pois, caso height/discrHeight nao seja inteiro, nHeight*discrHeight (feito pelo for) nao completara exatamente a height
    float discrTexWidth = texWidth*(discrWidth/width);
    float discrTexHeight = texHeight*(discrHeight/height);
    for(float i=-0.5*(width/discrWidth);i<0.5*(width/discrWidth);i++) {
        for(float j=-0.5*(height/discrHeight);j<0.5*(height/discrHeight);j++) {
            glPushMatrix();
                if (inverted) glRotatef(180,1,0,0);
                glTranslatef(i*discrWidth,0.0,j*discrHeight);
                glBegin( GL_QUADS );
                    glNormal3f(0.,1.,0.);
                        glTexCoord2f(     i*discrTexWidth, (j+1)*discrTexHeight); glVertex3f(        0.0,1*pow(M_E,pow(i*discrWidth + 0,2)/pow(2,2)+pow(j*discrHeight+discrHeight + 0,2)/pow(2,2)),+discrHeight);
                        glTexCoord2f( (i+1)*discrTexWidth, (j+1)*discrTexHeight); glVertex3f(+discrWidth,1*pow(M_E,pow(i*discrWidth+discrWidth + 0,2)/pow(2,2)+pow(j*discrHeight+discrHeight + 0,2)/pow(2,2)),+discrHeight);
                        glTexCoord2f( (i+1)*discrTexWidth,     j*discrTexHeight); glVertex3f(+discrWidth,1*pow(M_E,pow(i*discrWidth+discrWidth + 0,2)/pow(2,2)+pow(j*discrHeight + 0,2)/pow(2,2)),         0.0);
                        glTexCoord2f(     i*discrTexWidth,     j*discrTexHeight); glVertex3f(        0.0,1*pow(M_E,pow(i*discrWidth + 0,2)/pow(2,2)+pow(j*discrHeight + 0,2)/pow(2,2)),         0.0);
                glEnd();
            glPopMatrix();
        }
    }
}

