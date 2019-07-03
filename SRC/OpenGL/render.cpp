#include "../COMMON.H"

extern player_t PLAYER[];
extern int ZT_FRAMERATE;


int testAnim = 0;
int playSpeed = 32;




void ztUpdateAnimation(animationControl_t * animCtrl, animated_model_t * currentModel)
{
    if (currentModel->nbFrames==0)return;
    //XPDATA * currentPDATA = currentModel->pol[0];
    //model_t * currentPDATA = currentModel->model[0];

    /**Sets the animation data**/
    animCtrl->currentFrm+=ZT_FRAMERATE;
    uint32_t FPS = animCtrl->fps;
    float percent = 1.0f/(float)(1<<FPS);
    if (animCtrl->currentFrm>=animCtrl->endFrm<<FPS)
        animCtrl->currentFrm-=(animCtrl->endFrm-animCtrl->startFrm)<<FPS;

    /**Safety measure**/
    if (animCtrl->currentFrm < animCtrl->startFrm<<FPS || animCtrl->currentFrm >= animCtrl->endFrm<<FPS)
        animCtrl->currentFrm=animCtrl->startFrm<<FPS;

    animCtrl->currentKeyFrm=animCtrl->currentFrm>>FPS;  //should be >>currentModel->AnimInterpolation; but I forgot to output 60 fps animations... No time to fix it...

    uint16_t nextKeyFrm=animCtrl->currentKeyFrm+1;
    if (nextKeyFrm>=animCtrl->endFrm)
        nextKeyFrm=animCtrl->startFrm;

   // compVert * curKeyFrame = (compVert*)currentModel->animation[animCtrl->currentKeyFrm]->cVert;
   // compVert * nextKeyFrame = (compVert*)currentModel->animation[nextKeyFrm]->cVert;
    vertex_t * curKeyFrame = (vertex_t*)currentModel->keyFrames[animCtrl->currentKeyFrm].cVert;

    vertex_t * nextKeyFrame = (vertex_t*)currentModel->keyFrames[nextKeyFrm].cVert;

    uint16_t interpFctr = animCtrl->currentFrm-(animCtrl->currentKeyFrm<<FPS);

    /**Uncompress the vertices and apply linear interpolation**/

    /*vertex_t *dst=currentPDATA->pntbl[0];
    vertex_t *src=curKeyFrame[0];
    vertex_t *nxt=nextKeyFrame[0];*/
    /*for (i = 0; i < currentPDATA->nbPoint*sizeof(vertex_t); i+= sizeof(vertex_t)) {
		*dst++=(*src+(((*nxt-*src)*interpFctr)>>FPS));  //Here is a "mistake" : Using <<8 would be faster since the SH2 supports a shift left 8 instruction, but I didn't know it when I made this animation tool. To be changed to 8 instead
		*src++; *nxt++;
    }*/


    extern int enableInterpolation;
    if (!enableInterpolation) {interpFctr=0;}
    unsigned int j=0;
    for (unsigned int i=0; i<currentModel->nbModels; i++)
        {
            model_t * currentPDATA = &currentModel->model[i];
            for (unsigned int ii=0; ii<currentModel->model[i].nbPoint; ii++)
            {
                vertex_t *dst=(vertex_t*)&currentPDATA->pntbl[ii];
                vertex_t *src=(vertex_t*)&curKeyFrame[j];
                vertex_t *nxt=(vertex_t*)&nextKeyFrame[j];

                for (uint32_t v=0; v<3; v++) {
                   dst->point[v]=(src->point[v]) + (((nxt->point[v]-src->point[v])*interpFctr)*percent);
                }


                /*(aModel->keyFrames[curFrame].cVert[j].point[X]) + (percentage2 * aModel->keyFrames[nextFrame].cVert[j].point[X]);

                aModel->model[i].pntbl[ii].point[Y]=
                (percentage * aModel->keyFrames[curFrame].cVert[j].point[Y]) + (percentage2 * aModel->keyFrames[nextFrame].cVert[j].point[Y]);

                aModel->model[i].pntbl[ii].point[Z]=
                (percentage * aModel->keyFrames[curFrame].cVert[j].point[Z]) + (percentage2 * aModel->keyFrames[nextFrame].cVert[j].point[Z]);*/
                j++;
            }
        }

    /*  *dst=currentPDATA->pltbl[0].norm[0];
    Uint8 *src2=currentModel->animation[animCtrl->currentKeyFrm]->cNorm;
    for (i = 0; i < currentPDATA->nbPolygon; i++)    {
        *dst++=anorms[*src2][X];
        *dst++=anorms[*src2][Y];
        *dst=anorms[*src2++][Z];
        dst=dst+3 ; //++; *dst++;
    }*/
}

int render(animated_model_t * aModel)
{

           /* ztUnitMatrix(0);
            //ztRotY(52.0);
            //ztRotX(23.0);
            //ztRotZ(-12.5);
            ztPushMatrix();
            {
                ztTranslate((25), (12), (100));
                ztPrintMatrix();
                glBegin(GL_QUADS);
                    {
                        glColor4f(1.0,0.5,1.0,1.0f);
                        glVertex2f(-0.5, -0.5);
                        glColor4f(0.0,0.0,1.0,1.0f);
                        glVertex2f(0.5, -0.5);
                        glColor4f(0.0,1.0,0.0,1.0f);
                        glVertex2f(0.5, 0.5);
                        glColor4f(1.0,0,0,1.0f);
                        glVertex2f(-0.5, 0.5);
                    }
                    glEnd();
            }
            ztPopMatrix();*/

            glPushMatrix();

            glRotatef(PLAYER[0].ROTATION[X], 1.0f, 0.0f, 0.0f);
            glRotatef(PLAYER[0].ROTATION[Y], 0.0f, 1.0f, 0.0f);
            glTranslatef(-PLAYER[0].POSITION[X], -PLAYER[0].POSITION[Y], -PLAYER[0].POSITION[Z]);

            extern int DisplayTEST;
            if (DisplayTEST==0)glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            else  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

            glEnable(GL_CULL_FACE);
            glEnable(GL_BLEND);
            glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

            if (aModel->nbFrames>0)
            {

                ztUpdateAnimation(&PLAYER[0].ANIMCTRL, aModel);
                //testAnim+=ZT_FRAMERATE;
                //if (testAnim >= aModel->nbFrames*playSpeed) testAnim=0;
            }

            const float cOffset = 0.3;
            extern unsigned int texId[];
            for (unsigned int m=0; m<aModel->nbModels; m++)
            {
                model_t * curModel = &aModel->model[m];
                vertex_t * curVert;
                for (unsigned int i=0; i<curModel->nbPolygon; i++)
                {
                    glBindTexture(GL_TEXTURE_2D, texId[curModel->pltbl[i].texture]);
                    if (aModel->texture[curModel->pltbl[i].texture].attributes & TEX_NO_GOURAUD) glColor4f(1,1,1,1);
                    glBegin(GL_QUADS);
                    {
                        glTexCoord2f(0.0f, 1.0f/ aModel->texture[curModel->pltbl[i].texture].nbFrames);
                        curVert = &curModel->pntbl[curModel->pltbl[i].vertIdx[3]];
                        if ((aModel->texture[curModel->pltbl[i].texture].attributes & TEX_NO_GOURAUD) == 0)
                            glColor4f(curVert->color[0]+cOffset,curVert->color[1]+cOffset,curVert->color[2]+cOffset,1.0f);
                        glVertex3f(curVert->point[X], curVert->point[Y], curVert->point[Z]);

                        glTexCoord2f(1.0f, 1.0f/ aModel->texture[curModel->pltbl[i].texture].nbFrames);
                        curVert = &curModel->pntbl[curModel->pltbl[i].vertIdx[2]];
                        if ((aModel->texture[curModel->pltbl[i].texture].attributes & TEX_NO_GOURAUD) == 0)
                            glColor4f(curVert->color[0]+cOffset,curVert->color[1]+cOffset,curVert->color[2]+cOffset,1.0f);
                        glVertex3f(curVert->point[X], curVert->point[Y], curVert->point[Z]);

                        glTexCoord2f(1.0f, 0.0f);
                        curVert = &curModel->pntbl[curModel->pltbl[i].vertIdx[1]];
                        if ((aModel->texture[curModel->pltbl[i].texture].attributes & TEX_NO_GOURAUD) == 0)
                            glColor4f(curVert->color[0]+cOffset,curVert->color[1]+cOffset,curVert->color[2]+cOffset,1.0f);
                        glVertex3f(curVert->point[X], curVert->point[Y], curVert->point[Z]);

                        glTexCoord2f(0.0f, 0.0f);
                        curVert = &curModel->pntbl[curModel->pltbl[i].vertIdx[0]];
                        if ((aModel->texture[curModel->pltbl[i].texture].attributes & TEX_NO_GOURAUD) == 0)
                            glColor4f(curVert->color[0]+cOffset,curVert->color[1]+cOffset,curVert->color[2]+cOffset,1.0f);
                        glVertex3f(curVert->point[X], curVert->point[Y], curVert->point[Z]);
                    }
                    glEnd();
                }
            }

            glPopMatrix();

            return 1;
}
