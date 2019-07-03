#include "../COMMON.H"


void setVector(VECTOR source, VECTOR dest)
{
    dest[X]=source[X];
    dest[Y]=source[Y];
    dest[Z]=source[Z];
}

void VecNormal(float * dest, float * a, float * b )
{
	/**Cross product**/
	float buf[3];
	double dist;

    buf[X]= (((a[Y])*(b[Z])) - ((a[Z])*(b[Y])));
    buf[Y]= (((a[Z])*(b[X])) - ((a[X])*(b[Z])));
    buf[Z]= (((a[X])*(b[Y])) - ((a[Y])*(b[X])));

    dist=sqrt((buf[X]*buf[X])+(buf[Y]*buf[Y])+(buf[Z]*buf[Z]));

    if (dist==0)
    {
        dest[X]=0; dest[Y]=0; dest[Z]=0;
        return;
    }
    else
    {
        dest[X]= (buf[X]/dist);
        dest[Y]= (buf[Y]/dist);
        dest[Z]= (buf[Z]/dist);
    }
}
/*
float v3Length(vector3 a)
{
    return sqrt(a.x*a.x + a.y*a.y + a.z*a.z);
}

void v3CrossAndNormalize(vector3 * dest, vector3 a, vector3 b )
{
	//Cross product
	vector3 buf;
	float dist;

    buf.x= (((a.y)*(b.z)) - ((a.z)*(b.y)));
    buf.y= (((a.z)*(b.x)) - ((a.x)*(b.z)));
    buf.z= (((a.x)*(b.y)) - ((a.y)*(b.x)));

    dist=sqrt((buf.x*buf.x)+(buf.y*buf.y)+(buf.z*buf.z));

    if (dist==0)
    {
        dest->x=0; dest->y=0; dest->z=0;
        return;
    }
    else
    {
        dest->x= (buf.x/dist);
        dest->y= (buf.y/dist);
        dest->z= (buf.z/dist);
    }
}*/

void VecEdge(VECTOR dest, VECTOR a, VECTOR b)
{
    /*float a[XYZ];    a[X] = toFLOAT(fa[X]);     a[Y] = toFLOAT(fa[Y]);  a[Z] = toFLOAT(fa[Z]);
    float b[XYZ];    b[X] = toFLOAT(fb[X]);     b[Y] = toFLOAT(fb[Y]);  b[Z] = toFLOAT(fb[Z]);*/

    dest[X] = (a[X]-b[X]);
    dest[Y] = (a[Y]-b[Y]);
    dest[Z] = (a[Z]-b[Z]);
}

void setNormals(polygon_t curPol)
{
   /* VECTOR v0, v1, v2, v3;
    VECTOR vN;
    VECTOR vU, vV;

    VecEdge(vU, curPol.vertIdx[0], curPol.vertIdx[3]);
    VecEdge(vV, curPol.vertIdx[1], curPol.vertIdx[0]);
    VecNormal(v0,vV,vU);

    VecEdge(vU, curPol.vertIdx[1], curPol.vertIdx[0]);
    VecEdge(vV, curPol.vertIdx[2], curPol.vertIdx[1]);
    VecNormal(v1,vV, vU);

    VecEdge(vU, curPol.vertIdx[2], curPol.vertIdx[1]);
    VecEdge(vV, curPol.vertIdx[3], curPol.vertIdx[2]);
    VecNormal(v2,vV,vU);

    VecEdge(vU, curPol.vertIdx[3], curPol.vertIdx[2]);
    VecEdge(vV, curPol.vertIdx[0], curPol.vertIdx[3]);
    VecNormal(v3,vV,vU);

    vN[X]=(v0[X]+v1[X]+v2[X]+v3[X]);
    vN[Y]=(v0[Y]+v1[Y]+v2[Y]+v3[Y]);
    vN[Z]=(v0[Z]+v1[Z]+v2[Z]+v3[Z]);

    float dist=sqrt((vN[X]*vN[X])+(vN[Y]*vN[Y])+(vN[Z]*vN[Z]));

    if (dist==0) {vN[X]=0; vN[Y]=0; vN[Z]=0;}
    else        {
        vN[X]= (vN[X]/dist);
        vN[Y]= (vN[Y]/dist);
        vN[Z]= (vN[Z]/dist);
    }

    setVector(vN, curPol.normal);*/
}

void recalculateNormals(model_t * pol)
{
    VECTOR v0, v1, v2, v3;
    VECTOR vN;
    VECTOR vU, vV;


    for (unsigned int i=0; i<pol->nbPolygon; i++)
    {
        VecEdge(vU, pol->pntbl[pol->pltbl[i].vertIdx[0]].point, pol->pntbl[pol->pltbl[i].vertIdx[3]].point);
        VecEdge(vV, pol->pntbl[pol->pltbl[i].vertIdx[1]].point, pol->pntbl[pol->pltbl[i].vertIdx[0]].point);
        VecNormal(v0,vV,vU);

        VecEdge(vU, pol->pntbl[pol->pltbl[i].vertIdx[1]].point, pol->pntbl[pol->pltbl[i].vertIdx[0]].point);
        VecEdge(vV, pol->pntbl[pol->pltbl[i].vertIdx[2]].point, pol->pntbl[pol->pltbl[i].vertIdx[1]].point);
        VecNormal(v1,vV, vU);

        VecEdge(vU, pol->pntbl[pol->pltbl[i].vertIdx[2]].point, pol->pntbl[pol->pltbl[i].vertIdx[1]].point);
        VecEdge(vV, pol->pntbl[pol->pltbl[i].vertIdx[3]].point, pol->pntbl[pol->pltbl[i].vertIdx[2]].point);
        VecNormal(v2,vV,vU);

        VecEdge(vU, pol->pntbl[pol->pltbl[i].vertIdx[3]].point, pol->pntbl[pol->pltbl[i].vertIdx[2]].point);
        VecEdge(vV, pol->pntbl[pol->pltbl[i].vertIdx[0]].point, pol->pntbl[pol->pltbl[i].vertIdx[3]].point);
        VecNormal(v3,vV,vU);

        vN[X]=(v0[X]+v1[X]+v2[X]+v3[X]);
        vN[Y]=(v0[Y]+v1[Y]+v2[Y]+v3[Y]);
        vN[Z]=(v0[Z]+v1[Z]+v2[Z]+v3[Z]);

        float dist=sqrt((vN[X]*vN[X])+(vN[Y]*vN[Y])+(vN[Z]*vN[Z]));

        if (dist==0) {vN[X]=0; vN[Y]=0; vN[Z]=0;}
        else        {
            vN[X]= (vN[X]/dist);
            vN[Y]= (vN[Y]/dist);
            vN[Z]= (vN[Z]/dist);
        }
        for (unsigned int ii=0; ii<3; ii++)
            pol->pltbl[i].normal[ii]= (vN[ii]);
    }
}




void pointNormals(model_t * pol)
{
    float vN[3]; //, vV;
    //vV[X]=0.0f; vV[Y]=-30.5f; vV[Z]=0.0f;

    for (unsigned int i=0; i<pol->nbPoint; i++)
    {
        float v_array[256][3];
        unsigned int nbVectors=0;

        for (unsigned int ii=0; ii<pol->nbPolygon; ii++)
        {
            for (unsigned int j=0; j<4; j++)
            {
                if (i == pol->pltbl[ii].vertIdx[j])
                {
                    v_array[nbVectors][X]=pol->pltbl[ii].normal[X];
                    v_array[nbVectors][Y]=pol->pltbl[ii].normal[Y];
                    v_array[nbVectors][Z]=pol->pltbl[ii].normal[Z];
                    nbVectors++;
                }
            }

        }

        VECTOR vnTot = {0};
        for (unsigned int ii=0; ii<nbVectors; ii++)
        {
            vnTot[X]+=(v_array[ii][X]);
            vnTot[Y]+=(v_array[ii][Y]);
            vnTot[Z]+=(v_array[ii][Z]);
        }

        if (nbVectors > 0)
        {
            vN[X]=(vnTot[X]/(float)nbVectors);
            vN[Y]=(vnTot[Y]/(float)nbVectors);
            vN[Z]=(vnTot[Z]/(float)nbVectors);
        }
        else
        {
            vN[X]=0; vN[Y]=0; vN[Z]=0;
        }



        for (unsigned int ii=0; ii<3; ii++)
            pol->pntbl[i].normal[ii]= (vN[ii]);
    }
}

