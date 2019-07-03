#ifndef COMMON_H
#define COMMON_H


using namespace std;
#include <windows.h>
#include <gl/gl.h>
#include <GL/glu.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <math.h>

#include "ZTE/saturnDef.h"

#define     LUTaddr             (0x25C7A960)
#define     returnLUTaddr(n)    (LUTaddr + (0x20 * n))
#define     LUTidx(n)           (uint16_t)(62764 + (4 * n))
#define     CRAM_Base           (0x25f00200)
#define     returnCRAMaddr(n)   (CRAM_Base + (0x20 * n))
#define     LUTcramIdx(n)       ((256 + (16 * n)))

#define     toFIXED16(n)          (int32_t)(n*65536.0)
#define     toFIXED8(n)         (int16_t)(n*256.0)
#define MAX(x, y)	((x) > (y) ? (x) : (y))
#define MIN(x, y)	((x) < (y) ? (x) : (y))

#define CAN_JUMP (1)
#define MAX_SPRITES             (2000)

const float conv              = M_PI/180.0f;

enum
{
 X=0,
 Y=1,
 Z=2,
 XY=2,
 XYZ=3,
 XYZS=4
};

typedef     float   MATRIX[16];
typedef     float	VECTOR[XYZ] ;

/*
enum
{
    SORT_MIN = 0,
    SORT_CEN = 1,
    SORT_MAX = 2,
    SORT_BFR = 3
};*/

enum
{
    IDLE_ANIM       = 0,
    WALK_ANIM       = 1,
    RUN_ANIM        = 2,
    ATTACK_ANIM     = 3,
    ALT_ATT_ANIM    = 4,
    MELEE_ANIM      = 5,
    JUMP_ANIM       = 6,
    FALL_ANIM       = 7,
    CRAWL_ANIM      = 8,
    HURT_ANIM       = 9,
    DYING_ANIM      = 10,
    TAUNT_ANIM      = 11,
};

typedef struct
{
	bool bPressed;
	bool bReleased;
	bool bHeld;
} sKeyState;

typedef struct
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
    uint16_t rgb; //Saturn 16 bits format
    uint16_t palette_idx; //Palette/Color bank index
} pixel_t;

typedef struct
{
    float point[3];
    float color[3];
    float normal[3];
    unsigned int normalArray;
} vertex_t;

typedef struct
{
    unsigned int vertIdx[4];
    int texture;
    bool dualPlane;
    int sorting;
    pixel_t color; //If it's a single color
    unsigned int vdp2_effect; //Bump mapping?
    float normal[3];
} polygon_t;

typedef struct
{
    unsigned int nbPoint;
    unsigned int nbPolygon;
    vertex_t * pntbl;
    polygon_t * pltbl;
    float           BoxMin[3]; //Bounding box min point for the submodel, can be useful if you are a maniac who wants to add things such as headshots, dismemberment and pretty kid-friendly good stuff
    float           BoxMax[3]; //Bounding box max point for the submodel, can be useful if you are a maniac who wants to add things such as headshots, dismemberment and pretty kid-friendly good stuff
} model_t;

typedef struct
{
    vertex_t * cVert;  //Uncompressed for now
    float * cNorm[3]; //Will probably stay unused
}key_frame_t;

enum
{
    TEX_IS_2D = (1<<0),
    TEX_NO_GOURAUD = (1<<1),
    TEX_USE_TRANSPARENCY = (1<<2),
};


typedef struct {
    uint8_t	     flag ;
    uint8_t	     sorting ;
    uint16_t	 texno ;
    uint16_t	 atrb ;
    uint16_t	 colno ;
    uint16_t	 gstb ;
    uint16_t	 dir ;
} ATTR ;

typedef struct
{
    unsigned int    width;
    unsigned int    height;
    unsigned int    colorDepth;
    string          name;
    unsigned int    textureId;
    int             paletteIdx;
    pixel_t *       pixel;
    uint16_t        clut[16];
    uint8_t         nbFrames;
    uint8_t         attributes;
    ATTR            SGL_ATTR; //For SGL mainly, can be converted for ZTE format
} texture_t;


typedef struct
{
    unsigned int    framerate;
    unsigned int    nbFrames;
    unsigned int    nbTextures;
    unsigned int    nbModels;
    string          name;
    uint8_t         keyFramesIdx[12][3]; //Sets the ranges for different animations : Start, end, and frames per seconds
    key_frame_t *   keyFrames;
    model_t *       model;
    texture_t *     texture;
    float           BoxMin[3]; //Bounding box min point
    float           BoxMax[3]; //Bounding box max point
} animated_model_t;

typedef struct
{
    uint16_t animID;
    uint16_t currentFrm;
    uint16_t currentKeyFrm;
    uint16_t fps;
    uint16_t startFrm;
    uint16_t endFrm;
} animationControl_t;

struct player_t
{
    float               POSITION[3];
    float               NEXT_POSITION[3];
    float               ROTATION[3]; //16 bits on Saturn, so aligned with the next 2 chars
    int8_t              PLAYER_ID;
    int8_t              OWNED_WEAPONS;
    float               SPEED[3];
    float               MOVEMENT_SPEED;
    float               LATERAL_SPEED;
    int32_t             LEAF;  //Short is way enough...
    uint16_t            ANIM[2];
    int8_t              AMMO[8];
    uint16_t            STATUS;
    uint16_t            LAST_INPUT;
    int8_t              IS_FIRING;
    int8_t              WEAPON;
    animationControl_t  ANIMCTRL;
};

uint16_t        swap_endian_ushort(uint16_t value);
int16_t         swap_endian_sshort( int16_t value);
uint32_t        swap_endian_uint(uint32_t value);
int32_t         swap_endian_sint( int32_t value);


#include "ZTE/MATH/matrix.h"

/**Functions**/
float dotProduct(float * vct1, float * vct2);

#endif // COMMON_H
