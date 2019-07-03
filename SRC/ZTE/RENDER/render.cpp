#include "../../COMMON.H"

#define BACKFACE_CULLING_OFF (0x4000)
#define ZSORT_BFR (0)
#define ZSORT_MIN (0x1000)
#define ZSORT_MAX (0x2000)
#define ZSORT_CEN (0x3000)

#define VDP1_MAX (1023)
#define VDP1_MIN (-1024)

#define DRAW_DISTANCE (8192<<16)
#define PRECLIPPING_DISABLE (1<<11)
extern int currentMatrix;

typedef enum
{
	CLIP_LEFT = 1<<0,
	CLIP_RIGHT = 1<<1,
	CLIP_TOP = 1<<2,
	CLIP_BOTTOM = 1<<3,
	CLIP_FAR = 1<<4,
	CLIP_NEAR = 1<<5,
} CLIP_PLANE;

typedef struct{
    float           pnt[3];
    float           color;
    unsigned int    clipFlag;
    float           inverseZ;
} vertex2D_t;

/**INCOMPLETE**/
void ztDrawModel(model_t * model)
{

    float ScreenDist = 188.0;
    float ScreenDistClip = 188.0 / 32.0;

    vertex2D_t vBuf[model->nbPoint];
    vertex2D_t * v = &vBuf[0];
    vertex_t * pArray=model->pntbl ;
    float * m = &mtx[currentMatrix][0];
    float * p;

    for (unsigned int i=0; i<model->nbPoint; i++)
    {
        p=&pArray->point[X];

        v->pnt[Z] = m[8] * p[X] + m[9] * p[Y] + m[10] * p[Z] + m[11];

        if (v->pnt[Z] > ScreenDistClip ) v->clipFlag = 0;
        else
            {v->pnt[Z] = ScreenDistClip; v->clipFlag = CLIP_NEAR;}

        v->inverseZ = ScreenDist/v->pnt[Z];

        v->pnt[X] = ((m[0]*p[X] + m[1]*p[Y] + m[2]*p[Z] + m[3])*v->inverseZ) * (1.0/704.0);
        v->pnt[Y] = ((m[4]*p[X] + m[5]*p[Y] + m[6]*p[Z] + m[7])*v->inverseZ) * (1.0/448.0);

        v->color=pArray->color[0]+0.3;
        pArray++;
        v++;
    }
}
/*
void ztDrawWall(ZPDATA * pol)
{

    Uint32 cnt;// = pol->StartPoint;
    Uint32 nbPol=pol->EndPol-pol->StartPol;

    if (ztDispPolygons+nbPol >= MAX_POLYGONS-30) return;

    FIXED z, nearP=ScreenDistClip;
    Uint32 PreClippingflag;
    vertex_t * v= &vertex[0];
    FIXED * m = (FIXED*)mtx;
    //ZPOINT * zp = &pol->pntbl[0];
    Sint16 * p;
    Uint16 ctrl;


    for (cnt=pol->StartPoint; cnt < pol->EndPoint; cnt++)
    {
        //Convert vertices from 16 bits to 32 bits
        p = VertexArray[cnt];

        v->pnt[Z] = (m[M02] * p[X]) + (m[M12] * p[Y]) + (m[M22] * p[Z]) + m[M32];
        if (v->pnt[Z] > nearP ) v->clipFlag = 0;
        else
            {v->pnt[Z] = nearP; v->clipFlag = CLIP_NEAR;}

        //Starts the division
        DivFixed(ScreenDist, v->pnt[Z]);

        //Calculates X and Y while waiting for screenDist/z
        v->pnt[X] = (m[M00] * p[X]) + (m[M10] * p[Y]) + (m[M20] * p[Z]) + m[M30];
        v->pnt[Y] = (m[M01] * p[X]) + (m[M11] * p[Y]) + (m[M21] * p[Z]) + m[M31];

        // Retrieves the result of the division
        v->inverseZ =   *(FIXED*)0xFFFFFF04;

        //Transform X and Y to screen space
        v->pnt[X]= slMulFX(v->pnt[X], v->inverseZ) >> 16;
        v->pnt[Y]= slMulFX(v->pnt[Y], v->inverseZ) >> 16;

        //Clipping flags for fast culling/preclipping disable/enable
        v->clipFlag |= v->pnt[X] < W_L_CLIP ? CLIP_LEFT : (v->pnt[X] > W_R_CLIP ? CLIP_RIGHT : 0);
        v->clipFlag |= v->pnt[Y] < W_T_CLIP ? CLIP_TOP : (v->pnt[Y] > W_B_CLIP ? CLIP_BOTTOM : 0);

        //Vertex color processing
        v->color= (p[S]&0x1F)-(v->pnt[Z]>>24);
        if (v->color < 0) v->color=0;
        v->color=GourDef[v->color | (p[S]&0xFFE0) ];

        //Next vertex
        v++;
    }
    ztTotVertices+=pol->EndPoint-pol->StartPoint;

    Uint32 ptv[5];  //4 indices, the 5th is a buffer to swap the values
//return;
ZPOLYGON * pltbl;
    //POLYGON PROCESSING
    for (cnt=pol->StartPol; cnt<pol->EndPol; cnt++)
    {
        pltbl= &PolygonArray[cnt];
        //"Load" the values
        ptv[0]=pltbl->vertices[0];
        ptv[1]=pltbl->vertices[1];
        ptv[2]=pltbl->vertices[2];
        ptv[3]=pltbl->vertices[3];

        //If it's fully out, just cull it out
        if (vertex[ptv[0]].clipFlag & vertex[ptv[1]].clipFlag & vertex[ptv[2]].clipFlag & vertex[ptv[3]].clipFlag)
            continue;

        //Do backface culling using a cross product. Checks the orientation, but should also flip the h value if it's a backfaced dual plane polygon
        //Causes problems with triangles
        if (!(pltbl->cmdctrl&BACKFACE_CULLING_OFF))
        {
            if ((vertex[ptv[0]].pnt[X]-vertex[ptv[3]].pnt[X]) * (vertex[ptv[0]].pnt[Y] - vertex[ptv[2]].pnt[Y]) >=
                (vertex[ptv[0]].pnt[Y]-vertex[ptv[3]].pnt[Y]) * (vertex[ptv[0]].pnt[X] - vertex[ptv[2]].pnt[X])) continue;
        }

        //Test if it's in the z display area. You need it for the z sort, but you could skip the switch for a single rule only (like sort max only)
        z=vertex[ptv[0]].pnt[Z];
        switch (pltbl->cmdctrl&0x3000)
        {
            case ZSORT_MIN:
                if (vertex[ptv[1]].pnt[Z] < z) z= vertex[ptv[1]].pnt[Z];
                if (vertex[ptv[2]].pnt[Z] < z) z= vertex[ptv[2]].pnt[Z];
                if (vertex[ptv[3]].pnt[Z] < z) z= vertex[ptv[3]].pnt[Z];
                break;
            //case SORT_BFR: //Not supported at the moment
                //break;
            case ZSORT_CEN:
                z+= (vertex[ptv[1]].pnt[Z] + vertex[ptv[2]].pnt[Z] + vertex[ptv[3]].pnt[Z]);
                z=z>>2;
                break;
            default:
                if (vertex[ptv[1]].pnt[Z] > z) z= vertex[ptv[1]].pnt[Z];
                if (vertex[ptv[2]].pnt[Z] > z) z= vertex[ptv[2]].pnt[Z];
                if (vertex[ptv[3]].pnt[Z] > z) z= vertex[ptv[3]].pnt[Z];
                //break;
        }

        //Checks if the z distance is valid
        //if ( (z <= nearP) || (z >= farP) ) continue;
       // if (z <= nearP) continue;

        PreClippingflag=(vertex[ptv[0]].clipFlag | vertex[ptv[1]].clipFlag | vertex[ptv[2]].clipFlag | vertex[ptv[3]].clipFlag);
        PreClippingflag=(PreClippingflag&0x0F);
        ctrl=pltbl->cmdctrl&0x8FFF;

        if (!PreClippingflag)
           PreClippingflag= PRECLIPPING_DISABLE;
        else
            PreClippingflag=0;




        //Sets up the gouraud shading table to be transfered after vblank
        gourRealMax[ztDispPolygons][0]= vertex[ptv[0]].color; //GRTBL(vertex[ptv[0]].color[0], vertex[ptv[0]].color[1], vertex[ptv[0]].color[2]);
        gourRealMax[ztDispPolygons][1]= vertex[ptv[1]].color; //GRTBL(vertex[ptv[1]].color[0], vertex[ptv[1]].color[1], vertex[ptv[1]].color[2]);
        gourRealMax[ztDispPolygons][2]= vertex[ptv[2]].color; //GRTBL(vertex[ptv[2]].color[0], vertex[ptv[2]].color[1], vertex[ptv[2]].color[2]);
        gourRealMax[ztDispPolygons][3]= vertex[ptv[3]].color; //GRTBL(vertex[ptv[3]].color[0], vertex[ptv[3]].color[1], vertex[ptv[3]].color[2]);

        //Display the polygon if it's all good...it aint pretty
        ztSetCommand(vertex[ptv[0]].pnt, vertex[ptv[1]].pnt, vertex[ptv[2]].pnt, vertex[ptv[3]].pnt,
                     ctrl, (pltbl->cmdpmod&0xF6FF) | (PreClippingflag),
                     tex_def[pltbl->texno].CGadr, pltbl->cmdcolr, tex_def[pltbl->texno].HVsize,
                     GRreal(ztDispPolygons),z);
    }
    ztTotPolygons += nbPol;
}*/

