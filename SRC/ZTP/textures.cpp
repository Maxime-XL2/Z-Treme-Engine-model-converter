#include "../COMMON.H"
/*****
CREDIT FOR TGA LOADING FUNCTIONS : JOHANNES FETZ, JO ENGINE
DIRTY ADAPTATION BY XL2
******/








#define			CONVERT_COLOR(TGA, IDX)		        (((*((unsigned char *)(TGA + IDX))))/8 & 0x1f)

#define R_COLOR_COMPRESSION (1)
#define G_COLOR_COMPRESSION (1)
#define B_COLOR_COMPRESSION (1)

#define         TGA_RGB(R, G, B)		                    (0x8000 | ((((B/B_COLOR_COMPRESSION)*B_COLOR_COMPRESSION)<<10) | (((G/G_COLOR_COMPRESSION)*G_COLOR_COMPRESSION) << 5) | (R/R_COLOR_COMPRESSION)*R_COLOR_COMPRESSION))

#define			TGA_24BITS_GET_PIXEL(TGA, X, Y, WIDTH)	TGA_RGB(CONVERT_COLOR(TGA, (((X) * 3) + 2) + ((Y) * (WIDTH * 3))), \
                                                                CONVERT_COLOR(TGA, (((X) * 3) + 1) + ((Y) * (WIDTH * 3))), \
                                                                CONVERT_COLOR(TGA, (((X) * 3)) + ((Y) * (WIDTH * 3))))

#define			TGA_32BITS_GET_PIXEL(TGA, X, Y, WIDTH)	TGA_RGB(CONVERT_COLOR(TGA, ((4*X) + 2) + ((Y) * (4*WIDTH))), \
                                                                CONVERT_COLOR(TGA, ((4*X) + 1) + ((Y) * (4*WIDTH))), \
                                                                CONVERT_COLOR(TGA, ((4*X) + 0) + ((Y) * (4*WIDTH))))




/**** Bytes alignment can be problematic, and since I'm only trying to read the file format, I just went all-short to save time  ****/
typedef struct
{
   unsigned short  idlength;
   unsigned short  colourmaptype;
   unsigned short  datatypecode;
   short colourmaporigin;
   short colourmaplength;
   unsigned short  colourmapdepth;
   short x_origin;
   short y_origin;
   short width;
   short height;
   unsigned short  bitsperpixel;
   unsigned short  imagedescriptor;
} TGA_HEADER;





/****/
uint16_t CompressColor(uint16_t color, int compression)
{
    if (color == 0)
        return 0;
    else
        return color & (64478<<(compression));
}

bool checkSimilarColor(uint16_t color1, uint16_t color2, int maxDistance)
{
    float c1[XYZ] = {(float)(color1&0x1F), (float)((color1>>5)&0x1F), (float)((color1>>10)&0x1F)};
    float c2[XYZ] = {(float)(color2&0x1F), (float)((color2>>5)&0x1F), (float)((color2>>10) & 0x1F)};

    float dif[XYZ] = {c1[X]-c2[X], c1[Y]-c2[Y], c1[Z]-c2[Z]};
    float dist = sqrtf(dif[X]*dif[X]+dif[Y]*dif[Y]+dif[Z]*dif[Z]);
    //cout << "Dist= " << dist << "\n";
    if (dist <= (float)maxDistance)
        return true;
    return false;
}

/**This function fills the current image's palette in a 16 colors palette**/
void FillPalette(texture_t * texture, int compression)
{
if (compression > 20) {cout << "Texture no. " << texture->textureId <<" has more than 16 colors...can't compress more\n"; return;}
    for (unsigned int i=0; i<16; i++) texture->clut[i]=0x00;
    unsigned short total_palette = 0;

   // uint16_t tmpPalette[texture->width*texture->height]={0};
    //First pass to reserve the first palette entry for transparency, only if transparency is used
    for (unsigned int i=0; i< (unsigned int)(texture->width*texture->height); i++)
    {
        if (texture->pixel[i].rgb==0)
        {
            total_palette=1;
            texture->clut[0]=0;
            break;
        }
    }

    for (unsigned int i=0; i< (unsigned int)(texture->width*texture->height); i++)
    {
        bool duplicate=false;
        for (unsigned short ii=0; ii<total_palette; ii++){
            if (checkSimilarColor(texture->pixel[i].rgb, texture->clut[ii], compression+1))
            {
                duplicate = true;
                texture->pixel[i].palette_idx=ii;
                break;
            }
        }
        if (duplicate == false){
            if (total_palette >= 16)
                {texture->pixel[i].palette_idx=15; total_palette++;}
            else{
                texture->pixel[i].palette_idx=total_palette;
                texture->clut[total_palette]=texture->pixel[i].rgb;
                total_palette++;
            }
        }
    }
    if (total_palette > 16)
    {
        int compr=compression + 1;
        cout << "More than 16 colors...reducing the amount of colors...\n";
        FillPalette(texture, compr);
    }
    else
    {
        texture->colorDepth=1; //4 bpp
        if (compression >= 0)
            cout << "The image contains a total of " << total_palette << " colors after compression.\n";
        else
            cout << "The image contains a total of " << total_palette << " colors.\n";
    }
}


int ReadTGAHeader(ifstream *ibinfile, TGA_HEADER * header)
{
    ibinfile->read((char*)&header->idlength, sizeof(uint8_t));
    ibinfile->read((char*)&header->colourmaptype, sizeof(uint8_t));
    ibinfile->read((char*)&header->datatypecode, sizeof(uint8_t));
    ibinfile->read((char*)&header->colourmaporigin, sizeof(uint16_t));
    ibinfile->read((char*)&header->colourmaplength, sizeof(uint16_t));
    ibinfile->read((char*)&header->colourmapdepth, sizeof(uint8_t));
    ibinfile->read((char*)&header->x_origin, sizeof(uint16_t));
    ibinfile->read((char*)&header->y_origin, sizeof(uint16_t));
    ibinfile->read((char*)&header->width, sizeof(uint16_t));
    ibinfile->read((char*)&header->height, sizeof(uint16_t));
    ibinfile->read((char*)&header->bitsperpixel, sizeof(uint8_t));
    ibinfile->read((char*)&header->imagedescriptor, sizeof(uint8_t));
    return 1;
}


uint16_t        tga_get_pixel(const char * const stream, const int x, const int y, const int width, uint16_t bits)
{
    switch (bits)
    {
        case 32:
            if (CONVERT_COLOR(stream, (4*(x) + 3) + ((y) * 4*(width))) <= 0)
                return (0);
            return (TGA_32BITS_GET_PIXEL(stream, x, y, width));
        default:
            return (TGA_24BITS_GET_PIXEL(stream, x, y, width));
    }
}


int ReadTGAData(ifstream * file, texture_t * img, unsigned short bits)
{

    for (int y=img->height-1; y>=0 ; y--)  //This is actually the right order
    //for (int y=0; y<img->height ; y++)
    {
         for (int x=0; x< (int)img->width; x++)
        {
            if (bits==32)
            {
                char stream[4] = {0};
                file->read((char*)&stream, (4));
                if (CONVERT_COLOR(stream, 3) <= 0) img->pixel[(y*img->width)+x].rgb=0;
                else
                    img->pixel[(y*img->width)+x].rgb = TGA_32BITS_GET_PIXEL(stream,0,0, 4);

                img->pixel[(y*img->width)+x].r = (uint8_t)stream[0];
                img->pixel[(y*img->width)+x].g = (uint8_t)stream[1];
                img->pixel[(y*img->width)+x].b = (uint8_t)stream[2];
                if (img->pixel[(y*img->width)+x].rgb != 0)
                    img->pixel[(y*img->width)+x].a = 0xFF;
                else img->pixel[(y*img->width)+x].a=0;
            }
            else if (bits==24)
            {
                char stream[3] = {0};
                file->read((char*)&stream, (3));
                img->pixel[(y*img->width)+x].rgb = TGA_24BITS_GET_PIXEL(stream, 0, 0, 3);
                img->pixel[(y*img->width)+x].r = (uint8_t)stream[0];
                img->pixel[(y*img->width)+x].g = (uint8_t)stream[1];
                img->pixel[(y*img->width)+x].b = (uint8_t)stream[2];
                if (img->pixel[(y*img->width)+x].rgb != 0)
                    img->pixel[(y*img->width)+x].a = 0xFF;
                else img->pixel[(y*img->width)+x].a=0;
            }
        }
    }
    return 1;
}

void textureSpecialAttributes(texture_t * texture)
{
        stringstream classType(texture->name);            string bufferStr;
        getline(classType, bufferStr, '_');

        int tot=1;
        string c[20]; //Stupidly high for no real reasons, could be smarter
        c[0]=bufferStr;
        texture->nbFrames=1;
        texture->attributes=0;
        while (classType || (tot<20))
        {  //cout << c[tot];
            getline(classType, bufferStr, '_');
            if (c[tot-1] == bufferStr) break;
            c[tot]=bufferStr;
            //if (c[tot]=="ANIM") cout << "anim!!!!!!!!!!!!!!!!!\n\n\n!!!!!!\n";
            tot++;
        }
        for (int i=0; i<20; i++)//Stupidly high for no real reasons, could be smarter
        {
            if (c[i]=="ANIM")
            {
                if (c[i+1]!= "")
                texture->nbFrames=std::atoi(c[i+1].c_str());
            }
            if (c[i]=="2D")
                texture->attributes|=TEX_IS_2D|TEX_NO_GOURAUD;

        }
       // cout << "TOTAL TESTING ATTRIBUTES : " << tot << ", total frames : " << texture->nbFrames << "\n";
}

/**If the loading of the texture fails, at least you have a sad face to look at!**/
void createFakeTexture(texture_t * t)
{
    t->width = 8;
    t->height = 8;
    t->pixel = new pixel_t[64];
    for (uint16_t i=0; i<64; i++)
    {
        t->pixel[i].a=t->pixel[i].r=t->pixel[i].g=t->pixel[i].b=255;
        t->pixel[i].rgb = 0xFFFF;
    }
    t->pixel[2*8+2].rgb = C_RGB(1, 1, 1);
    t->pixel[2*8+5].rgb = C_RGB(1, 1, 1);
    t->pixel[3*8+6].rgb = C_RGB(1, 1, 31);

    t->pixel[5*8+2].rgb = C_RGB(1, 1, 1);
    t->pixel[5*8+3].rgb = C_RGB(1, 1, 1);
    t->pixel[5*8+4].rgb = C_RGB(1, 1, 1);
    t->pixel[5*8+5].rgb = C_RGB(1, 1, 1);
    t->pixel[6*8+1].rgb = C_RGB(1, 1, 1);
    t->pixel[6*8+6].rgb = C_RGB(1, 1, 1);
    FillPalette(t, -1);
    textureSpecialAttributes(t);

}

/**TO DO : ADD RLE SUPPORT */
int ReadTGAFile (string folder, texture_t * texture)
{

    //cout << "\nAttempting to read TGA texture files...\n";
    string name;
    name =  folder + texture->name + ".TGA"; //Temporary : Will break under several conditions
    TGA_HEADER header;
    ifstream ibinfile(name.c_str(), ios::in | ios::binary);

    if (!ibinfile.is_open())
        {
            cout << "ERROR : COULDN'T LOAD FILE " << texture->name.c_str() << ".TGA, creating a sad face texture\n";
            createFakeTexture(texture);
            return -1;
        }

    ReadTGAHeader(&ibinfile, &header);
        cout << "Texture " << texture->name << ", size : " << header.width << "x" << header.height << ", pixel depth : " << header.bitsperpixel << ", RLE = " << header.datatypecode << "\n";

    if (header.datatypecode!=2) {cout << "RLE currently not supported\n"; createFakeTexture(texture); return -1;}
    texture->width = header.width;
    texture->height = header.height;
    texture->pixel = new pixel_t[header.height*header.width];

    ReadTGAData(&ibinfile, texture, header.bitsperpixel);
    ibinfile.close();

    FillPalette(texture, -1);
    textureSpecialAttributes(texture);

    return 7777;
}
