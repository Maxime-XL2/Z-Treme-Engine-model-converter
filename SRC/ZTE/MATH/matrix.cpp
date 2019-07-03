#include "../../COMMON.H"

/**
WORK IN PROGRESS!
**/

MATRIX mtx[20];
int currentMatrix;

inline void unitMatrix(float * m)
{
     m[M00]=m[M11]=m[M22]=1.0f;
     m[M01]=m[M02]=m[M03]=
     m[M10]=m[M12]=m[M13]=
     m[M20]=m[M21]=m[M23]=
    // m[M30]=m[M31]=m[M32]=
    // m[M33]=
     0.0;
}

void ztUnitMatrix(unsigned int depth)
{
    currentMatrix= (int)depth;
    float * m = &mtx[currentMatrix][0];
    unitMatrix(m);
}




void ztPushMatrix(void)
{
    if (++currentMatrix > 19) currentMatrix=19;
    float * src = &mtx[currentMatrix-1][0];
    float * dst = &mtx[currentMatrix][0];
    for (unsigned int i=0; i<12; i++)
        dst[i]=src[i];
}

void ztPopMatrix(void)
{
    if (--currentMatrix < 0) currentMatrix=0;
}

void ztRotX(float angX)
{
    float * m = &mtx[currentMatrix][0];
    float cosV=std::cos(angX*conv);
    float sinV=std::sin(angX*conv);
cout << cosV << "  " << sinV << "\n";
    float m01 = m[M01];
    float m02 = m[M02];
    float m11 = m[M11];
    float m12 = m[M12];
    float m21 = m[M21];
    float m22 = m[M22];

    m[M01] = m01*cosV + m02*sinV;
    m[M02] = -(m01*sinV) + m02*cosV;
    m[M11] = m11*cosV+m12*sinV;
    m[M12] = -(m11*sinV)+m12*cosV;
    m[M21] = m21*cosV+m22*sinV;
    m[M22] = -(m21*sinV)+m22*cosV;

    /**
    	M01 = ctm[0][1];
	M02 = ctm[0][2];
	M11 = ctm[1][1];
	M12 = ctm[1][2];
	M21 = ctm[2][1];
	M22 = ctm[2][2];
	ctm[0][1] = MulFixed(M01, cosNum) + MulFixed(M02, sinNum);
	ctm[0][2] = -MulFixed(M01, sinNum) + MulFixed(M02, cosNum);
	ctm[1][1] = MulFixed(M11, cosNum) + MulFixed(M12, sinNum);
	ctm[1][2] = -MulFixed(M11, sinNum) + MulFixed(M12, cosNum);
	ctm[2][1] = MulFixed(M21, cosNum) + MulFixed(M22, sinNum);
	ctm[2][2] = -MulFixed(M21, sinNum) + MulFixed(M22, cosNum);
    **/
}

void ztRotY(float angY)
{

    float * m = &mtx[currentMatrix][0];

  //  ztPrintMatrix();
    float cosV=std::cos(angY*conv);
    float sinV=std::sin(angY*conv);
    float 	m00 = m[M00];
    float 	m02 = m[M02];
    float   m10 = m[M10];
    float   m12 = m[M12];
    float   m20 = m[M20];
    float   m22 = m[M22];

    m[M00]= m00 * cosV - m02 * sinV;
    m[M02]= m00 * sinV + m02 * cosV;
    m[M10]= m10 * cosV - m12 * sinV;
    m[M12]= m10 * sinV + m12 * cosV;
    m[M20]= m20 * cosV - m22 * sinV;
    m[M22]= m20 * sinV + m22 * cosV;

    ztPrintMatrix();
}


/**To be fixed...*/
void ztRotZ(float angZ)
{
    float * m = &mtx[currentMatrix][0];
    float cosV=std::cos(angZ*conv);
    float sinV=std::sin(angZ*conv);

	float m00 = m[M00];
	float m01 = m[M01];
	float m10 = m[M10];
	float m11 = m[M11];
	float m20 = m[M20];
	float m21 = m[M21];
	m[M00] = (m00 * cosV) + (m01 * sinV);
	m[M01] = -(m00 * sinV) + (m01 * cosV);
	m[M10] = (m10 * cosV) + (m11 * sinV);
	m[M11] = -(m10 * sinV) + (m11 * cosV);
	m[M20] = (m20 * cosV) + (m21 * sinV);
	m[M21] = -(m20 * sinV) + (m21 * cosV);

    /*m[M00] +=  std::cos(angZ*conv);
    m[M01] +=  std::sin(angZ*conv);
    m[M10] -=  std::sin(angZ*conv);
    m[M11] +=  std::cos(angZ*conv);*/
}

void ztTranslate(float tx, float ty, float tz)
{
    float * m = &mtx[currentMatrix][0];
    m[M03]+=tx;
    m[M13]+=ty;
    m[M23]+=tz;
}

void ztScale(float sx, float sy, float sz)
{
    float * m = &mtx[currentMatrix][0];
    m[M00] *= sx;
    m[M11] *= sy;
    m[M22] *= sz;
}


void ztPrintMatrix(void)
{
    cout << "\nPrint matrix stack no." << currentMatrix << "\n";
    float * m = &mtx[currentMatrix][0];
    for (unsigned int i=0; i<4; i++)
    {
        cout << (m[0]) << " " << (m[1]) << " " << (m[2]) << " " << m[3] << "\n";
        m+=4;
    }

}
