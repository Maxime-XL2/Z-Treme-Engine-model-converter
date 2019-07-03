#include "../COMMON.H"

bool USE_SGL = 1; //For now, default option?

#define Max_Normals (162)
VECTOR normLUT[] =
{
    #include "anorms.h"
};

void WRITE_MDATA(ofstream * file, animated_model_t * aModel)
{
    uint16_t    bUint16;
    uint32_t    bUint32;
    int32_t     bSint32;


    bUint16=swap_endian_ushort(aModel->nbModels);
        file->write((char*)&bUint16, sizeof(uint16_t));

    bUint16=swap_endian_sshort(aModel->nbTextures);
        file->write((char*)&bUint16, sizeof(uint16_t));

    uint32_t countSize = 0;
    for (uint32_t i = 0; i<aModel->nbTextures; i++)
    {
        countSize += (aModel->texture[i].height*aModel->texture[i].width)/2; //>>aModel->texture[i].colorDepth;
        countSize += 12+32;//*aModel->texture[i].colorDepth; //If colorDepth = 0, then there is no color look-up table.
    }
    bUint32=swap_endian_uint(countSize);
        file->write((char*)&bUint32, sizeof(uint32_t));

    countSize=0;
    for (uint32_t i = 0; i<aModel->nbModels; i++)
    {
        countSize += aModel->model[i].nbPolygon * 20 ; //Each polygon = 12 bytes
        countSize += aModel->model[i].nbPoint * 32;
    }
    countSize += (aModel->nbFrames * aModel->model[0].nbPoint * 8); //Each compressed vertex is a sint16_t[3]
    bUint32=swap_endian_uint(countSize);
        file->write((char*)&bUint32, sizeof(uint32_t));


    bSint32=swap_endian_sint(toFIXED(aModel->BoxMin[X]));
        file->write((char*)&bSint32, sizeof(int32_t));
    bSint32=swap_endian_sint(toFIXED(aModel->BoxMin[Y]));
        file->write((char*)&bSint32, sizeof(int32_t));
    bSint32=swap_endian_sint(toFIXED(aModel->BoxMin[Z]));
        file->write((char*)&bSint32, sizeof(int32_t));

    bSint32=swap_endian_sint(toFIXED(aModel->BoxMax[X]));
        file->write((char*)&bSint32, sizeof(int32_t));
    bSint32=swap_endian_sint(toFIXED(aModel->BoxMax[Y]));
        file->write((char*)&bSint32, sizeof(int));
    bSint32=swap_endian_sint(toFIXED(aModel->BoxMax[Z]));
        file->write((char*)&bSint32, sizeof(int));

    //Animation : Updated 2018/05/31
    bUint16=swap_endian_sshort(aModel->nbFrames);
        file->write((char*)&bUint16, sizeof(unsigned short));
    bUint16=swap_endian_sshort(aModel->framerate);
        file->write((char*)&bUint16, sizeof(unsigned short));

    bSint32=swap_endian_sint(0x00000000);
        file->write((char*)&bSint32, sizeof(int));
}


/*****
FOR SGL (mainly for you Ponut64!) : This writes all the PDATA in a sequential order
*****/
void WRITE_SGL_PDATA(ofstream * file, animated_model_t * aModel)
{
    unsigned short bUint16;
    unsigned int bUint32;
    int bSint32;

    for (unsigned int i=0; i<aModel->nbModels; i++){
        //PDATA, including buffers for the pointers
        bUint32 = 0;
            file->write((char*)&bUint32, sizeof(uint32_t));
        bUint32 = swap_endian_uint(aModel->model[i].nbPoint);
            file->write((char*)&bUint32, sizeof(uint32_t));
        bUint32 = 0; //Pointer
            file->write((char*)&bUint32, sizeof(uint32_t));
        bUint32 = swap_endian_uint(aModel->model[i].nbPolygon);
            file->write((char*)&bUint32, sizeof(uint32_t));
        bUint32 = 0; //Pointer
            file->write((char*)&bUint32, sizeof(uint32_t));
        bUint32 = 0; //VECTOR TABLE POINTER
            file->write((char*)&bUint32, sizeof(uint32_t));

        //POINT, 12 bytes each
        for (unsigned int ii=0; ii<aModel->model[i].nbPoint; ii++) {
            for (unsigned int j=0; j<3; j++) {
                bSint32 = swap_endian_sint(toFIXED(aModel->model[i].pntbl[ii].point[j]));
                file->write((char*)&bSint32, sizeof(int32_t));
            }
        }
        //POLYGON, 12 bytes for normals and 8 bytes for vertices
        for (unsigned int ii=0; ii< aModel->model[i].nbPolygon; ii++) {
            //Normals
            for (unsigned int j=0; j<3; j++) {
                bSint32 = swap_endian_sint(toFIXED(aModel->model[i].pltbl[ii].normal[j]));
                file->write((char*)&bSint32, sizeof(int32_t));
            }
            //Vertices
            for (unsigned int j=0; j<4; j++) {
                bUint16 = swap_endian_ushort(aModel->model[i].pltbl[ii].vertIdx[j]);
                file->write((char*)&bUint16, sizeof(uint16_t));
            }
        }
        //ATTR, 12 bytes each
        for (unsigned int ii=0; ii< aModel->model[i].nbPolygon; ii++)
        {
            file->write((char*)&aModel->texture[aModel->model[i].pltbl[ii].texture].SGL_ATTR.flag, sizeof(uint8_t));
            file->write((char*)&aModel->texture[aModel->model[i].pltbl[ii].texture].SGL_ATTR.sorting, sizeof(uint8_t));
            bUint16=swap_endian_ushort(aModel->texture[aModel->model[i].pltbl[ii].texture].SGL_ATTR.texno);
                file->write((char*)&bUint16, sizeof(uint16_t));
            bUint16=swap_endian_ushort(aModel->texture[aModel->model[i].pltbl[ii].texture].SGL_ATTR.atrb);
                file->write((char*)&bUint16, sizeof(uint16_t));
            bUint16=swap_endian_ushort(aModel->texture[aModel->model[i].pltbl[ii].texture].SGL_ATTR.colno);
                file->write((char*)&bUint16, sizeof(uint16_t));
            bUint16=swap_endian_ushort(aModel->texture[aModel->model[i].pltbl[ii].texture].SGL_ATTR.gstb);
                file->write((char*)&bUint16, sizeof(uint16_t));
            bUint16=swap_endian_ushort(aModel->texture[aModel->model[i].pltbl[ii].texture].SGL_ATTR.dir);
                file->write((char*)&bUint16, sizeof(uint16_t));
        }
        //VECTOR NORMALS
        for (unsigned int ii=0; ii<aModel->model[i].nbPoint; ii++) //VECTOR : 12 bytes each
        {
            for (unsigned int j=0; j<3; j++)
            {
                bSint32 = swap_endian_sint(toFIXED(aModel->model[i].pntbl[ii].normal[j]));
                file->write((char*)&bSint32, sizeof(int32_t));
            }
        }
    }
}

unsigned short convert_to_4bpp(unsigned short a, unsigned short b){    return (((a&0xff)<<4) | (b));}
void WRITE_TEXTURES(ofstream * file, animated_model_t * aModel)
{
    cout << "Writing the textures to binary file...\n\n";
    uint16_t buf16;
    uint32_t bufPtr=0;
    texture_t * t;
    for (unsigned short i=0; i<aModel->nbTextures; i++)
    {
        t=&aModel->texture[i];
        uint16_t w = swap_endian_ushort(t->width);
        uint16_t h = swap_endian_ushort(t->height);
        file->write((char*)&w, sizeof(uint16_t));
        file->write((char*)&h, sizeof(uint16_t));
        buf16 = swap_endian_ushort(COL_16);
        file->write((char*)(&buf16), sizeof(uint16_t));
        buf16 = swap_endian_ushort(16);
        file->write((char*)(&buf16), sizeof(uint16_t));
        file->write((char*)&bufPtr, sizeof(int32_t)); //Just a pointer

        for (unsigned short ii=0; ii< (t->width * t->height);)
        {
            uint8_t buf = 0;
            buf =  (uint8_t)(convert_to_4bpp(t->pixel[ii].palette_idx, t->pixel[ii+1].palette_idx)); //(((t->pixel[ii].palette_idx &0x0F)<<4) |  (t->pixel[ii+1].palette_idx&0x0F));
            file->write((char*)&buf, sizeof(uint8_t));
            ii+=2;
        }

    }
    for (unsigned short i=0; i<aModel->nbTextures; i++)
    {
        t=&aModel->texture[i];
        for (unsigned int ii=0; ii<16; ii++)
        {
           buf16 = swap_endian_ushort(t->clut[ii]);
           file->write((char*)(&buf16), sizeof(uint16_t));
        }
    }
}


uint8_t normalLookUp(float * curNormal)
{
    uint32_t  bestRes = 0;
    float   bestDist = 65535.0;

    for (uint32_t i=0; i<Max_Normals; i++)
    {
        float dist = sqrtf(pow(curNormal[X]-normLUT[i][X], 2) + pow(curNormal[Y]-normLUT[i][Y], 2) + pow(curNormal[Z]-normLUT[i][Z], 2));
        if ((dist) < (bestDist))
        {
            bestDist=dist;
            bestRes=i;
        }
    }
    return bestRes;
}

extern void recalculateNormals(model_t *);
/*void fixAnimationData(animated_model_t * aModel, animated_model_t * tmpModel)
{

    for (unsigned int i=0; i<aModel->nbModels; i++)
    {
        for (unsigned int ii=0; ii<aModel->model[i].nbPolygon; ii++)
        {
            for (unsigned int j=0; j<4; j++)
                tmpModel->model[i].pltbl[ii].vertIdx[j]=aModel->model[i].pltbl[ii].vertIdx[j];
        }
        recalculateNormals(&tmpModel->model[i]);
    }
}
*/
/*
void compareAnimationData(animated_model_t * baseModel, animated_model_t * tmpModel)
{
    fixAnimationData(baseModel, tmpModel);
    unsigned int tot_vertices=0; unsigned int tot_normals=0;
    for (unsigned int i=0; i<baseModel->nbModels; i++)
    {
        for (unsigned int ii=0; ii<baseModel->model[i].nbPoint; ii++)
        {
            for (unsigned int j=0; j<3; j++)
            {
                baseModel->animation[FrameID].cVert[tot_vertices][j]=
                (Sint16)(kfModel->pol[i].pntbl[ii][j]/256);
            }
                tot_vertices++;
        }
    }
    for (unsigned int i=0; i<kfModel->TOTAL_MESH; i++)
    {
        for (unsigned int ii=0; ii<kfModel->pol[i].nbPolygon; ii++)
        {
            baseModel->animation[FrameID].cNorm[tot_normals]=
            (uint8_t)(normalLookUp(kfModel->pol[i].pltbl[ii].norm));
            tot_normals++;
        }
    }
}*/



void writeAnim(ofstream * file, animated_model_t * aModel, unsigned int FrameID)
{
    int32_t bSint32 = swap_endian_sint(FrameID);
    uint8_t bUint8 = 0;
    file->write((char*)&bSint32, sizeof(int32_t));
    file->write((char*)&bSint32, sizeof(int32_t));
    unsigned int tot_vertices=0, tot_normals=0;

    for (unsigned int i=0; i< aModel->nbModels; i++ )
    {
        for (unsigned int ii=0; ii<(aModel->model[i].nbPoint); ii++)
        {
            for (unsigned int j=0; j<3; j++)
            {
                int16_t bSint16 = swap_endian_sshort(toFIXED8(aModel->keyFrames[FrameID].cVert[tot_vertices].point[j]));
                file->write((char*)&bSint16, sizeof(int16_t));
            }tot_vertices++;
        }
    }
    while (tot_vertices%2 !=0)
    {
        file->write((char*)(&bSint32), sizeof(uint16_t));
        tot_vertices++;
    }

    for (unsigned int i=0; i< aModel->nbModels; i++ )
    {
        for (unsigned int ii=0; ii<(aModel->model[i].nbPolygon); ii++)
        {
            bUint8 = (uint8_t)normalLookUp((float*)&aModel->model[i].pltbl[ii].normal[X]); //baseModel->animation[FrameID].cNorm[tot_normals]=(uint8_t)(normalLookUp(kfModel->pol[i].pltbl[ii].norm));
            file->write((char*)&bUint8, sizeof(uint8_t));
            tot_normals++;
        }
    }
    while (tot_normals%4 !=0)
    {
        file->write((char*)(&bSint32), sizeof(int8_t));
        tot_normals++;
    }

}

/********************************
* WIP : Writes data to custom binary format
*********************************/
void write_model_binary(ofstream * file, animated_model_t * aModel)
{


    WRITE_MDATA(file, aModel);
    WRITE_TEXTURES(file, aModel);
    WRITE_SGL_PDATA(file, aModel);

    vertex_t * v[aModel->nbModels];
    for (unsigned int i=0; i<aModel->nbModels; i++)
        v[i] = aModel->model[i].pntbl;


    for (unsigned int i=0; i<aModel->nbFrames; i++)
    {
        uint32_t v_count = 0;
        for (unsigned int ii=0; ii<aModel->nbModels; ii++)
        {
            aModel->model[ii].pntbl = &aModel->keyFrames[i].cVert[v_count];
            v_count += aModel->model[ii].nbPoint;
            recalculateNormals(&aModel->model[ii]);
        }

        //compareAnimationData(model, &animModel, i);
        writeAnim(file, aModel, i);
        for (unsigned int i=0; i<aModel->nbModels; i++)
        aModel->model[i].pntbl = v[i];
    }
}

