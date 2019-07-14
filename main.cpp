
#include "SRC/COMMON.H"

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);
void DisableOpenGL(HWND, HDC, HGLRC);


/***************************************
* UGLY UGLY GLOBALS... TO BE REMOVED SOMEDAY...
***********************************/
unsigned int texId[1280];
string PdataName[256];
int DisplayTEST=0;
int ZT_FRAMERATE=1;
player_t PLAYER[4];
int enableInterpolation=1;

/*
//TO BE MOVED ELSEWHERE (ZTM_DEF)


void specialConditions(unsigned short startPtr, unsigned short endPtr, _MOD_DATA * model)
{
    if (model->TOT_TEXT <= 0) return;


    for (unsigned short i=startPtr; i<endPtr; i++)
    {
        std::size_t findDual = TextureArray[i].find("DUAL_");
        std::size_t findWhite = TextureArray[i].find("C_WHITE");
        if ((findDual==0) && (pimg[i].palette[1]==0 && pimg[i].palette[0]!=0))
        {
            cout << "Found a dual plane attribute texture (no backface culling)\n";
            TextureAttributeArray[i]=ATTRIBUTE(Dual_Plane, SORT_MIN, No_Texture, pimg[i].palette[0], CL32KRGB|No_Gouraud, CL32KRGB| MESHoff | Window_In, sprPolygon, UseLight|UseNearClip);
        }
        else if ((findWhite == 0) && (pimg[i].palette[1]==0 && pimg[i].palette[0]!=0))
        {
            cout << "Found a quad with SORT_MIN flag.\n";
            TextureAttributeArray[i]=ATTRIBUTE(Single_Plane, SORT_MIN, No_Texture, pimg[i].palette[0], CL32KRGB|No_Gouraud, CL32KRGB| MESHoff | Window_In, sprPolygon, UseLight|UseNearClip);
        }
        else if (pimg[i].palette[0]==0)
        {
           TextureAttributeArray[i] = ATTRIBUTE(Dual_Plane,SORT_CEN, i, LUTidx(i), No_Gouraud,Window_In|MESHoff|HSSon|ECdis|SPenb|CL16Look,sprNoflip,UseLight);
        }
        else
        {
            if (pimg[i].palette[1]==0 && pimg[i].palette[0]!=0)
                TextureAttributeArray[i]=ATTRIBUTE(Single_Plane, SORT_CEN, No_Texture, pimg[i].palette[0], CL32KRGB|No_Gouraud, CL32KRGB| MESHoff | Window_In, sprPolygon, UseLight|UseNearClip);
                //TextureAttributeArray[i]=ATTRIBUTE(Single_Plane, SORT_CEN, No_Texture, pimg[i].palette[0], CL16Bnk|No_Gouraud, CL16Bnk| MESHoff | Window_In, sprPolygon, UseLight|UseNearClip);
              //  TextureAttributeArray[i] = ATTRIBUTE(Single_Plane, SORT_CEN, No_Texture, pimg[i].palette[0], CL32KRGB | No_Gouraud, CL32KRGB|Window_In|MESHoff , sprPolygon, UseLight);
            else
                TextureAttributeArray[i] = ATTRIBUTE(Single_Plane,SORT_CEN, i, LUTidx(i), No_Gouraud,Window_In|MESHoff|HSSon|ECdis|SPdis|CL16Look,sprNoflip,UseLight|UseNearClip);
        }


    }
}
*/

/*
void setTextures(bool useMipmap)
{
  for (unsigned int i=0; i<(unsigned int)MAP_DATA.TOT_TEXT; i++)
    {
        glGenTextures(1, &texId[i]);                                // Generate OpenGL texture IDs
        glBindTexture(GL_TEXTURE_2D, texId[i]);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        GLubyte imgData[img[i].width*img[i].height*4]={0};

        int j=0;
        for (int ii=0; ii<img[i].width*img[i].height*4; ii+=4)
        {
            if (img[i].data[j]==0)
            {
                imgData[ii]= 0;
                imgData[ii+1]= 0;
                imgData[ii+2]= 0;
                imgData[ii+3]= 0;
            }
            else
            {
                imgData[ii]= R_to_24(img[i].data[j]);
                imgData[ii+1]= G_to_24(img[i].data[j]);
                imgData[ii+2]= B_to_24(img[i].data[j]);
                imgData[ii+3]= 255;
            }

            j++;

        }

        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        if (useMipmap)
            gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, img[i].width, img[i].height, GL_RGBA, GL_UNSIGNED_BYTE, (void*)imgData);
        else
            glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, img[i].width, img[i].height, 0, GL_ALPHA, GL_UNSIGNED_BYTE, (void*)imgData);
    }
}*/

extern void                     initOpenGL();
extern animated_model_t *       loading();
extern void                     setTextures(animated_model_t * model);
extern int                      render(animated_model_t * aModel);
extern void                     calcLight(animated_model_t*);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wcex;
    HWND hwnd;
    HDC hDC;
    HGLRC hRC;
    MSG msg;
    BOOL bQuit = FALSE;

    /* register window class */
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_OWNDC;
    wcex.lpfnWndProc = WindowProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = "ZTP_CONVERTER";
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);;


    if (!RegisterClassEx(&wcex))
        return 0;

    /* create main window */
    hwnd = CreateWindowEx(0,
                          "ZTP_CONVERTER",
                          "ZTP CONVERTER", WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT, CW_USEDEFAULT,
                          352*2, 224*2, NULL, NULL, hInstance, NULL);

    animated_model_t * model = loading();

    ShowWindow(hwnd, nCmdShow);
    /* enable OpenGL for the window */
    EnableOpenGL(hwnd, &hDC, &hRC);

    initOpenGL();

    cout << "Setting textures...\n";
    setTextures(model);

    cout << "Preview...\n";
    extern void resetPlayer(player_t *);
    for (int pl=0; pl<4; pl++) {resetPlayer(&PLAYER[pl]);}
            PLAYER[0].ANIMCTRL.fps=4;
            PLAYER[0].ANIMCTRL.startFrm= 0;
            PLAYER[0].ANIMCTRL.endFrm= model->nbFrames; //The end frame is EXCLUDED. So 1 to 3 is really just 2 frames

    /* program main loop */
    while (!bQuit)
    {
        /* check for messages */
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            /* handle or dispatch messages */
            if (msg.message == WM_QUIT)
            {
                bQuit = TRUE;
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        extern int myControls(player_t*);
        bQuit = myControls(&PLAYER[0]);
        PLAYER[0].POSITION[X] = PLAYER[0].NEXT_POSITION[X];
        PLAYER[0].POSITION[Y] = PLAYER[0].NEXT_POSITION[Y];
        PLAYER[0].POSITION[Z] = PLAYER[0].NEXT_POSITION[Z];

        glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glMatrixMode(GL_MODELVIEW);
        glDisable(GL_CULL_FACE);
        glColor4f(1.0f,1.0f,1.0f,1.0f);
        if (DisplayTEST==0)
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
         else
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        /**DEMO DISPLAY*/
            /*glPushMatrix();
            glBegin(GL_QUADS);
            {
                glColor4f(1.0f,1.0f,0.0f,1.0f); glVertex2f(-1.0, -1.0);
                glColor4f(1.0f,0.0f,0.0f,1.0f); glVertex2f(1, -1);
                glColor4f(0.0f,1.0f,1.0f,1.0f); glVertex2f(1, 1);
                glColor4f(1.0f,1.0f,0.5f,1.0f); glVertex2f(-1, 1);
            }
            glEnd();
            glPopMatrix();*/
        /**END DEMO DISPLAY*/
        render(model);
            SwapBuffers(hDC);
            Sleep (ZT_FRAMERATE);
            glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    }

    /**
        TODO : Delete the arrays here
    **/

    /* shutdown OpenGL */
    DisableOpenGL(hwnd, hDC, hRC);

    /* destroy the window explicitly */
    DestroyWindow(hwnd);

    return msg.wParam;
}



LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CLOSE:
            PostQuitMessage(0);
        break;

        case WM_DESTROY:
            return 0;

        case WM_KEYDOWN:
        {
            switch (wParam)
            {
                case VK_ESCAPE:
                    PostQuitMessage(0);
                break;
            }
        }
        break;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}

void EnableOpenGL(HWND hwnd, HDC* hDC, HGLRC* hRC)
{
    PIXELFORMATDESCRIPTOR pfd;

    int iFormat;

    /* get the device context (DC) */
    *hDC = GetDC(hwnd);

    /* set the pixel format for the DC */
    ZeroMemory(&pfd, sizeof(pfd));

    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW |
                  PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 24;
    pfd.iLayerType = PFD_MAIN_PLANE;

    iFormat = ChoosePixelFormat(*hDC, &pfd);

    SetPixelFormat(*hDC, iFormat, &pfd);

    /* create and enable the render context (RC) */
    *hRC = wglCreateContext(*hDC);

    wglMakeCurrent(*hDC, *hRC);
}
void DisableOpenGL (HWND hwnd, HDC hDC, HGLRC hRC)
{
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hwnd, hDC);
}

