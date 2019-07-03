#include "../COMMON.H"


void initOpenGL(void)
{


    glClearColor(0.0f, 0.0f, 1.0f, 0.0f);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    float persp = 60.0f * 224.0f/352.0f;
    gluPerspective(persp, 352.0f/224.0f, 188.0f/((float)(1<<6)), 65536.0);
    glMatrixMode(GL_MODELVIEW);
    gluLookAt(0.0, 0.0, -0.1, 0.0, 0.0, 0.0, 0.0, -1.0, 0.0);
}

extern unsigned int texId[];
void setTextures(animated_model_t * aModel)
{
    for (unsigned int i=0; i<(unsigned int)aModel->nbTextures; i++)
    {
        glGenTextures(1, &texId[i]);                                // Generate OpenGL texture IDs
        glBindTexture(GL_TEXTURE_2D, texId[i]);
        texture_t * txt = &aModel->texture[i];

        GLubyte imgData[txt->width*txt->height*4]={0};
        unsigned int cnt=0;

        for (unsigned int ii=0; ii<txt->height; ii++)
        {
            for (unsigned int j=0; j<txt->width; j++)
            {
                imgData[cnt++] = ((txt->clut[txt->pixel[ii*txt->width + j].palette_idx])&0x1f)*8.25; //txt->pixel[ii*txt->width + j].b;
                imgData[cnt++] = ((txt->clut[txt->pixel[ii*txt->width + j].palette_idx]>>5)&0x1f)*8.25; //txt->pixel[ii*txt->width + j].g;
                imgData[cnt++] = ((txt->clut[txt->pixel[ii*txt->width + j].palette_idx]>>10)&0x1f)*8.25; //txt->pixel[ii*txt->width + j].r;
                imgData[cnt++] = txt->pixel[ii*txt->width + j].a;
            }
        }
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, txt->width, txt->height, GL_RGBA, GL_UNSIGNED_BYTE, (void*)imgData);
    }
}
