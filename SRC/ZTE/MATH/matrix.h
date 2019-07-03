extern MATRIX mtx[];

enum
{
    M00, M01, M02, M03,
    M10, M11, M12, M13,
    M20, M21, M22, M23,
   // M30, M31, M32, M33
};

void ztUnitMatrix(unsigned int depth);
void ztPushMatrix(void);
void ztPopMatrix(void);
void ztRotX(float angX);
void ztRotY(float angY);
void ztRotZ(float angZ);
void ztTranslate(float tx, float ty, float tz);
void ztScale(float sx, float sy, float sz);
void ztPrintMatrix(void);
