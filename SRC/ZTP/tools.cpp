#include "../COMMON.H"


uint16_t         swap_endian_ushort(uint16_t value)   { return ((((value) >> 8) & 0xff) | (((value) & 0xff) << 8));}
int16_t          swap_endian_sshort( int16_t value)   { return (((value >> 8)&0xFF) | ((value & 0xFF) << 8));}
uint32_t         swap_endian_uint(  uint32_t value)   { return ((((value) & 0xff000000) >> 24) | (((value) & 0x00ff0000) >>  8) | (((value) & 0x0000ff00) <<  8) | (((value) & 0x000000ff) << 24)); }
int32_t          swap_endian_sint(   int32_t value)   { return ((((value) & 0xff000000) >> 24) | (((value) & 0x00ff0000) >>  8) | (((value) & 0x0000ff00) <<  8) | (((value) & 0x000000ff) << 24)); }

float dotProduct(float * vct1, float * vct2){return ((vct1[X]*vct2[X])+(vct1[Y]*vct2[Y])+(vct1[Z]*vct2[Z]));}

/*
int AddPoint(vPDATA * newMesh, POINT CurrentPoint, VECTOR CurNormVect)
{
    for (unsigned int i=0; i< newMesh->nbPoint; i++)
    {
        if ((newMesh->pntbl[i][0] == CurrentPoint[0])&& (newMesh->pntbl[i][1]== CurrentPoint[1]) && (newMesh->pntbl[i][2]== CurrentPoint[2]))
            {
                return i;
            }
    }

    newMesh->pntbl[newMesh->nbPoint][X] = CurrentPoint[0];
    newMesh->pntbl[newMesh->nbPoint][Y] = CurrentPoint[1];
    newMesh->pntbl[newMesh->nbPoint][Z] = CurrentPoint[2];
    newMesh->vntbl[newMesh->nbPoint][X] = CurNormVect[X];
    newMesh->vntbl[newMesh->nbPoint][Y] = CurNormVect[Y];
    newMesh->vntbl[newMesh->nbPoint][Z] = CurNormVect[Z];
    newMesh->nbPoint ++;
    return (newMesh->nbPoint - 1);
}*/


void calculateBoundingBox(float * boxMin, float * boxMax, vertex_t * vertexArray, unsigned int totalVertices)
{
    vertex_t * v = vertexArray;
    for (unsigned int i=0; i<totalVertices; i++)
    {
        for (unsigned int ii = 0; ii<3; ii++)
        {
            if (v->point[ii] < boxMin[ii])
                boxMin[ii]=v->point[ii];
            else if (v->point[ii] > boxMax[ii])
                boxMax[ii]=v->point[ii];
        }
        v++;
    }
}


