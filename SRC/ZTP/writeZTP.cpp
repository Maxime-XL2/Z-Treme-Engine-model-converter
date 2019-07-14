#include "../COMMON.H"

int USE_SGL = 1; //For now, default option?


void writeUint16(ofstream * file, uint16_t data);
void writeSint16(ofstream * file, int16_t data);
void writeUint32(ofstream * file, uint32_t data);
void writeSint32(ofstream * file, int32_t data);


#define Max_Normals (162)
VECTOR normLUT[] =
{
    #include "anorms.h"
};



void WRITE_MDATA(ofstream * file, animated_model_t * aModel)
{
    writeUint16(file, aModel->nbModels);
    writeUint16(file, aModel->nbTextures);

    uint32_t countSize = 0;
    for (uint32_t i = 0; i<aModel->nbTextures; i++) {
        countSize += (aModel->texture[i].height*aModel->texture[i].width)/2; //>>aModel->texture[i].colorDepth;
        countSize += 12+32;//*aModel->texture[i].colorDepth; //If colorDepth = 0, then there is no color look-up table.
    }
    writeUint32(file, countSize);

    countSize=0;
    for (uint32_t i = 0; i<aModel->nbModels; i++) {
        if (USE_SGL)
        {
            countSize += aModel->model[i].nbPolygon * 20 ; //Each polygon = 12 bytes
            countSize += aModel->model[i].nbPoint * 32;
        }
        else
            countSize += aModel->model[i].nbPoint;

    }
    if (USE_SGL)
    {
        countSize += (aModel->nbFrames * aModel->model[0].nbPoint * 8); //Each compressed vertex is a sint16_t[3]
        writeUint32(file, countSize);
    }
    else
        writeUint16(file, countSize);

    countSize=0;
    if (!USE_SGL)
    {
        for (uint32_t i=0; i<aModel->nbModels; i++)
        {
            countSize+=aModel->model[i].nbPolygon;
        }
        writeUint16(file, countSize);
    }

    for (uint32_t i=0; i<3; i++) {
            writeSint32(file, toFIXED(aModel->BoxMin[i]));
            cout << (int)(aModel->BoxMin[i]) << ", ";
    }
    for (uint32_t i=0; i<3; i++) {
            writeSint32(file, toFIXED(aModel->BoxMax[i]));
            cout << (int)(aModel->BoxMax[i]) << ", ";
    }
    cout << "\n";

    //Animation : Updated 2018/05/31
    writeUint16(file, aModel->nbFrames);
    writeUint16(file, aModel->framerate);

    writeUint32(file, 0x00000000);

    if (!USE_SGL) {
        for (uint32_t i = 0; i<9; i++) writeUint32(file, 0);
    }

}

/**
New Z-Treme engine format (no more SGL)

typedef int16_t ZPOINT[4]; //vertex data = 3 Points, 1 color data. Use int8_t instead?

typedef struct {
    Uint8   vertices[4];
    Uint16  texno;
    Uint16  cmdctrl;
    Uint16  cmdpmod;
    Uint16  cmdcolr;
} ZPOLYGON; //12 bytes each

typedef struct {
    Uint16 StartPoint;
    Uint16 EndPoint;
    Uint16 StartPol;
    Uint16 EndPol;
    Uint16 LightID; //A copy to transfer to the slave
    Sint16 LightDistance;
} ZPDATA;
**/
#define	    GRTBL(r,g,b)	        (((b&0x1f)<<10) | ((g&0x1f)<<5) | (r&0x1f) )
uint16_t getColor(float * color)
{
    uint16_t rgb[XYZ]={0};
    const uint16_t maxColorBrightness=16;
    rgb[0]=MIN((uint16_t)((color[0]+1.0f)*16), maxColorBrightness);
    rgb[1]=MIN((uint16_t)((color[1]+1.0f)*16), maxColorBrightness);
    rgb[2]=MIN((uint16_t)((color[2]+1.0f)*16), maxColorBrightness);

    return GRTBL(rgb[0],rgb[1],rgb[2]);
}

void WRITE_ZPDATA(ofstream * file, animated_model_t * aModel)
{


    uint16_t startPt=0;
    uint16_t startPol=0;
    //ZPDATA
    for (unsigned int i=0; i<aModel->nbModels; i++){
        model_t * m = &aModel->model[i];

        //points
        writeUint16(file, startPt);
        startPt += m->nbPoint;
        writeUint16(file, startPt);

        //polygon
        writeUint16(file, startPol);
        startPol += m->nbPolygon;
        writeUint16(file, startPol);

        //Light data, not precalculated, might not even be used
        writeUint16(file, 0);
        writeUint16(file, 0);
    }

    //Zpoints
    for (unsigned int i=0; i<aModel->nbModels; i++){
        model_t * m = &aModel->model[i];

        //points
        for (unsigned int ii=0; ii<m->nbPoint; ii++) {
            for (unsigned int j=0; j<3; j++) {
                writeSint16(file, toFIXED8(m->pntbl[ii].point[j]));
            }
            //Get color for now, to be changed with a cool precalculated table instead
            writeUint16(file, getColor(m->pntbl[ii].color));
        }
    }

    startPt=0;
    for (unsigned int i=0; i<aModel->nbModels; i++) {
        model_t * m = &aModel->model[i];
        for (unsigned int ii=0; ii<m->nbPolygon; ii++){
                polygon_t * pol = &m->pltbl[ii];
            for (unsigned int j=0; j<4; j++){
                uint16_t buf = pol->vertIdx[j]+startPt;
                file->write((char*)&buf, sizeof(uint8_t));
            }
            texture_t * t = &aModel->texture[pol->texture];
            writeUint16(file, t->textureId);
            writeUint16(file, (t->SGL_ATTR.dir&0x0FFF)|((t->SGL_ATTR.sorting&3)<<12)
                        | ((t->SGL_ATTR.flag&1)<<14));
            writeUint16(file, t->SGL_ATTR.atrb|CL_Gouraud);
            writeUint16(file, t->SGL_ATTR.colno);
        }
        startPt+=m->nbPoint;

    }


}

/*****
FOR SGL (mainly for you Ponut64!) : This writes all the PDATA in a sequential order
*****/
void WRITE_SGL_PDATA(ofstream * file, animated_model_t * aModel)
{
    for (unsigned int i=0; i<aModel->nbModels; i++){
        //PDATA, including buffers for the pointers
        writeUint32(file, 0);
        writeUint32(file, aModel->model[i].nbPoint);
        writeUint32(file, 0);
        writeUint32(file, aModel->model[i].nbPolygon);
        writeUint32(file, 0);
        writeUint32(file, 0);//VECTOR TABLE POINTER

        //POINT, 12 bytes each
        for (unsigned int ii=0; ii<aModel->model[i].nbPoint; ii++) {
            for (unsigned int j=0; j<3; j++) {
                writeSint32(file,toFIXED(aModel->model[i].pntbl[ii].point[j]));
            }
        }
        //POLYGON, 12 bytes for normals and 8 bytes for vertices
        for (unsigned int ii=0; ii< aModel->model[i].nbPolygon; ii++) {
            //Normals
            for (unsigned int j=0; j<3; j++) {
                writeSint32(file, toFIXED(aModel->model[i].pltbl[ii].normal[j]));
            }
            //Vertices
            for (unsigned int j=0; j<4; j++) {
                writeUint16(file, aModel->model[i].pltbl[ii].vertIdx[j]);
            }
        }
        //ATTR, 12 bytes each
        for (unsigned int ii=0; ii< aModel->model[i].nbPolygon; ii++)
        {
            file->write((char*)&aModel->texture[aModel->model[i].pltbl[ii].texture].SGL_ATTR.flag, sizeof(uint8_t));
            file->write((char*)&aModel->texture[aModel->model[i].pltbl[ii].texture].SGL_ATTR.sorting, sizeof(uint8_t));
            writeUint16(file, aModel->texture[aModel->model[i].pltbl[ii].texture].SGL_ATTR.texno);
            writeUint16(file, aModel->texture[aModel->model[i].pltbl[ii].texture].SGL_ATTR.atrb);
            writeUint16(file, aModel->texture[aModel->model[i].pltbl[ii].texture].SGL_ATTR.colno);
            writeUint16(file, aModel->texture[aModel->model[i].pltbl[ii].texture].SGL_ATTR.gstb);
            writeUint16(file, aModel->texture[aModel->model[i].pltbl[ii].texture].SGL_ATTR.dir);
        }
        //VECTOR NORMALS
        for (unsigned int ii=0; ii<aModel->model[i].nbPoint; ii++) //VECTOR : 12 bytes each
        {
            for (unsigned int j=0; j<3; j++)
            {
                writeSint32(file, toFIXED(aModel->model[i].pntbl[ii].normal[j]));
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
        writeUint16(file, t->width);
        writeUint16(file, t->height);
        writeUint16(file, COL_16);
        writeUint16(file, 16);
        writeUint32(file, 0); //Just a pointer

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
           writeUint16(file, t->clut[ii]);
        }
    }



    cout << "Writing the textures to text file (for debugging)...\n\n";
    ofstream tFile("OUT/TEXTURES.TXT", ios::out);
    if (!tFile.is_open()) {cout << "ERROR WRITING TEXT FILE TEXTURE DATA...\n"; return;}

    tFile << "//Text version of the texture data. FOR TESTING/DEBUGGING ONLY! USE THE BINARY FORMAT INSTEAD!\n\n";
    tFile << "//Uint32 nbTextures = " << aModel->nbTextures << ";\n";
    tFile << "\nstruct {\n Uint16 w;\nUint16 h;\nUint16 * pixData;\n} textData;\n\n";

    tFile << "textData testTextures[" << aModel->nbTextures << "];\n\n";


    for (unsigned short i=0; i<aModel->nbTextures; i++)
    {
        t=&aModel->texture[i];
        tFile << "\n//Texture " << i << "\n";
        tFile << "//Width = " << t->width << ", height = " << t->height << "\n";
        tFile << "Uint16 textData TEXTURE" << i << " = {\n";
        tFile << t->width << ", " << t->height << ", \n{\n";
        for (unsigned int ii=0; ii<t->height*t->width; ii+=8)
        {
            for (unsigned int j=0; j<8; j++)
            {
                tFile << "C_RGB(" << (int)(t->pixel[ii+j].rgb&0x1F) << ", " <<
                (int)((t->pixel[ii+j].rgb>>5)&0x1F)  << ", " <<
                (int)((t->pixel[ii+j].rgb>>10)&0x1F) << "), ";
            }
            tFile << "\n";
        }
        tFile << "}\n};\n\n";
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



/**
Writes the vertex data and compressed normals (to keep SGL happy)
**/
void writeAnim(ofstream * file, animated_model_t * aModel, unsigned int FrameID)
{
    int32_t bSint32 = swap_endian_sint(FrameID);
    uint8_t bUint8 = 0;
    file->write((char*)&bSint32, sizeof(int32_t));
    file->write((char*)&bSint32, sizeof(int32_t));
    unsigned int tot_vertices=0, tot_normals=0;

    for (unsigned int i=0; i< aModel->nbModels; i++ ) {
        for (unsigned int ii=0; ii<(aModel->model[i].nbPoint); ii++) {
            for (unsigned int j=0; j<3; j++) {
                writeSint16(file, toFIXED8(aModel->keyFrames[FrameID].cVert[tot_vertices].point[j]));
            }
            tot_vertices++;
        }
    }
    while (tot_vertices%2 !=0) {
        writeSint16(file, 0);
        tot_vertices++;
    }

    if (!USE_SGL) return;

    for (unsigned int i=0; i< aModel->nbModels; i++ ) {
        for (unsigned int ii=0; ii<(aModel->model[i].nbPolygon); ii++) {
            bUint8 = (uint8_t)normalLookUp((float*)&aModel->model[i].pltbl[ii].normal[X]); //baseModel->animation[FrameID].cNorm[tot_normals]=(uint8_t)(normalLookUp(kfModel->pol[i].pltbl[ii].norm));
            file->write((char*)&bUint8, sizeof(uint8_t));
            tot_normals++;
        }
    }
    while (tot_normals%4 !=0) {
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
    if (USE_SGL)
        WRITE_SGL_PDATA(file, aModel);
    else
        WRITE_ZPDATA(file, aModel);

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


