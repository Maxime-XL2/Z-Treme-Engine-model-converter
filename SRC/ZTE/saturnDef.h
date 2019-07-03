#define	    toFIXED(a)		((int32_t)(65536.0 * (a)))
#define	    POStoFIXED(x,y,z)	{toFIXED(x),toFIXED(y),toFIXED(z)}
#define	    ATTRIBUTE(f,s,t,c,g,a,d,o)	{f,(s)|(((d)>>16)&0x1c)|(o),t,(a)|(((d)>>24)&0xc0),c,g,(d)&0x3f}
#define	    SPR_ATTRIBUTE(t,c,g,a,d)	{t,(a)|(((d)>>24)&0xc0),c,g,(d)&0x0f3f}
#define	    DEGtoANG(d)		((ANGLE)((65536.0 * (d)) / 360.0))
#define     RADtoANG(d)         ((ANGLE)((65536.0 * (d)) / (2*M_PI)))
#define	    C_RGB(r,g,b)		(0x8000|((b)<<10)|((g)<<5)|(r))
#define	    DGTtoRGB(c)		(0x8000|(((c)&0x0f00)<<3)|(((c)&0x4000)>>4)|(((c)&0x00f0)<<2)|(((c)&0x2000)>>8)|(((c)&0x000f)<<1)|(((c)&0x1000)>>12))
#define	    SLocate(x,y,p)	((p) * Page + (y) * Line + (x))

/*  Sprite control function */

#define	    FUNC_Sprite		1
#define	    FUNC_Texture	2
#define	    FUNC_Polygon	4
#define	    FUNC_PolyLine	5
#define	    FUNC_Line		6
#define	    FUNC_SystemClip	9
#define	    FUNC_UserClip	8
#define	    FUNC_BasePosition	10
#define	    FUNC_End		-1

#define	    _ZmLT		(0x05 << 8)	/* Zoom base Left Top */
#define	    _ZmLC		(0x09 << 8)	/* Zoom base Left Center */
#define	    _ZmLB		(0x0d << 8)	/* Zoom base Left Bottom */
#define	    _ZmCT		(0x06 << 8)	/* Zoom base Center Top */
#define	    _ZmCC		(0x0a << 8)	/* Zoom base Center Center */
#define	    _ZmCB		(0x0e << 8)	/* Zoom base Center Bottom */
#define	    _ZmRT		(0x07 << 8)	/* Zoom base Right Top */
#define	    _ZmRC		(0x0b << 8)	/* Zoom base Right Center */
#define	    _ZmRB		(0x0f << 8)	/* Zoom base Right Bottom */

#define	    MSBon		(1 << 15)	/* ƒtƒŒ?[ƒ€ƒoƒbƒtƒ@‚É?‘‚«?ž‚Þ‚l‚r‚a‚ð‚P‚É‚·‚é */
#define	    HSSon		(1 << 12)	/* ƒnƒCƒXƒs?[ƒhƒVƒ…ƒŠƒ“ƒN—LŒø */
#define	    HSSoff		(0 << 12)	/* ƒnƒCƒXƒs?[ƒhƒVƒ…ƒŠƒ“ƒN–³Œø(default) */
#define	    No_Window		(0 << 9)	/* ƒEƒBƒ“ƒhƒE‚Ì?§ŒÀ‚ðŽó‚¯‚È‚¢(default)*/
#define	    Window_In		(2 << 9)	/* ƒEƒBƒ“ƒhƒE‚Ì“à‘¤‚É•\Ž¦ */
#define	    Window_Out		(3 << 9)	/* ƒEƒBƒ“ƒhƒE‚ÌŠO‘¤‚É•\Ž¦ */
#define	    MESHoff		(0 << 8)	/* ’Ê?í•\Ž¦(default) */
#define	    MESHon		(1 << 8)	/* ƒ?ƒbƒVƒ…‚Å•\Ž¦ */
#define	    ECdis		(1 << 7)	/* ƒGƒ“ƒhƒR?[ƒh‚ðƒpƒŒƒbƒg‚Ì‚Ð‚Æ‚Â‚Æ‚µ‚ÄŽg—p */
#define	    ECenb		(0 << 7)	/* ƒGƒ“ƒhƒR?[ƒh—LŒø */
#define	    SPdis		(1 << 6)	/* ƒXƒy?[ƒXƒR?[ƒh‚ðƒpƒŒƒbƒg‚Ì‚Ð‚Æ‚Â‚Æ‚µ‚ÄŽg—p */
#define	    SPenb		(0 << 6)	/* ƒXƒy?[ƒX‚Í•\Ž¦‚µ‚È‚¢(default) */
#define	    CL16Bnk		(0 << 3)	/* ƒJƒ‰?[ƒoƒ“ƒN‚P‚U?Fƒ‚?[ƒh (default) */
#define	    CL16Look		(1 << 3)	/* ƒJƒ‰?[ƒ‹ƒbƒNƒAƒbƒv‚P‚U?Fƒ‚?[ƒh */
#define	    CL64Bnk		(2 << 3)	/* ƒJƒ‰?[ƒoƒ“ƒN‚U‚S?Fƒ‚?[ƒh */
#define	    CL128Bnk		(3 << 3)	/* ƒJƒ‰?[ƒoƒ“ƒN‚P‚Q‚W?Fƒ‚?[ƒh */
#define	    CL256Bnk		(4 << 3)	/* ƒJƒ‰?[ƒoƒ“ƒN‚Q‚T‚U?Fƒ‚?[ƒh */
#define	    CL32KRGB		(5 << 3)	/* ‚q‚f‚a‚R‚Q‚j?Fƒ‚?[ƒh */
#define	    CL_Replace		0		/* ?d‚Ë?‘‚«(?ã?‘‚«)ƒ‚?[ƒh */
#define	    CL_Shadow		1		/* ‰eƒ‚?[ƒh */
#define	    CL_Half		2		/* ”¼‹P“xƒ‚?[ƒh */
#define	    CL_Trans		3		/* ”¼“§–¾ƒ‚?[ƒh */
#define	    CL_Gouraud		4		/* ƒO?[ƒ??[ƒVƒF?[ƒfƒBƒ“ƒOƒ‚?[ƒh */

#define	    UseTexture		(1 << 2)	/* ƒeƒNƒXƒ`ƒƒ‚ð“\‚éƒ|ƒŠƒSƒ“ */
#define	    UseLight		(1 << 3)	/* ŒõŒ¹‚Ì‰e‹¿‚ðŽó‚¯‚éƒ|ƒŠƒSƒ“ */
#define	    UsePalette		(1 << 5)	/* ƒ|ƒŠƒSƒ“‚ÌƒJƒ‰?[ */

/* SGL Ver3.0 Add */
#define	    UseNearClip		(1 << 6)	/* ƒjƒA?EƒNƒŠƒbƒsƒ“ƒO‚ð‚·‚é */
#define	    UseGouraud		(1 << 7)	/* ƒŠƒAƒ‹ƒO?[ƒ??[ */
#define	    UseDepth		(1 << 4)	/* ƒfƒvƒXƒLƒ…?[ */
/* */

#if 0 /* SGL Ver3.0 Delete */
#define	    UseClip		(1 << 4)		/* ‰æ–ÊŠO‚É’¸“_‚ª?o‚½‚ç•\Ž¦‚µ‚È‚¢ */
#else
#define	    UseClip		UseNearClip		/* ƒjƒA?EƒNƒŠƒbƒsƒ“ƒO‚ð‚·‚é */
#endif

#define	    sprHflip		((1 << 4) | FUNC_Texture | (UseTexture << 16))
#define	    sprVflip		((1 << 5) | FUNC_Texture | (UseTexture << 16))
#define	    sprHVflip		((3 << 4) | FUNC_Texture | (UseTexture << 16))
#define	    sprNoflip		((0) | FUNC_Texture | (UseTexture << 16))
#define	    sprPolygon		(FUNC_Polygon | ((ECdis | SPdis) << 24))
#define	    sprPolyLine		(FUNC_PolyLine | ((ECdis | SPdis) << 24))
#define	    sprLine		(FUNC_Line | ((ECdis | SPdis) << 24))

#define	    No_Texture		0		/* ƒeƒNƒXƒ`ƒƒ‚ðŽg—p‚µ‚È‚¢Žž */
#define	    No_Option		0		/* ƒIƒvƒVƒ‡ƒ“‚ðŽg—p‚µ‚È‚¢Žž */
#define	    No_Gouraud		0		/* ƒO?[ƒ??[ƒVƒF?[ƒfƒBƒ“ƒO‚ðŽg—p‚µ‚È‚¢Žž */
#define	    No_Palet	        0		/* ƒJƒ‰?[ƒpƒŒƒbƒg‚ÌŽw’è‚ª‚¢‚ç‚È‚¢Žž */
#define	    GouraudRAM		(0x00080000 - (32*8))	/* ŒõŒ¹‚Ì‰e‹¿—p‚ÌƒO?[ƒ??[ƒoƒbƒtƒ@ */

#define	    COL_16	(2+1)		/* ƒJƒ‰?[ƒoƒ“ƒN‚P‚U?Fƒ‚?[ƒh */
#define	    COL_64	(2+0)		/* ƒJƒ‰?[ƒoƒ“ƒN‚U‚S?Fƒ‚?[ƒh */
#define	    COL_128	(2+0)		/* ƒJƒ‰?[ƒoƒ“ƒN‚P‚Q‚W?Fƒ‚?[ƒh */
#define	    COL_256	(2+0)		/* ƒJƒ‰?[ƒoƒ“ƒN‚Q‚T‚U?Fƒ‚?[ƒh */
#define	    COL_32K	(2-1)		/* ‚q‚f‚a‚R‚Q‚j?Fƒ‚?[ƒh */

    enum base {
	SORT_BFR ,			/* ’¼‘O‚É•\Ž¦‚µ‚½ƒ|ƒŠƒSƒ“‚ÌˆÊ’u‚ðŽg‚¤ */
	SORT_MIN ,			/* ‚S“_‚Ì“à?Aˆê”ÔŽè‘O‚Ì“_‚ðŽg‚¤ */
	SORT_MAX ,			/* ‚S“_‚Ì“àˆê”Ô‰“‚¢“_‚ðŽg‚¤ */
	SORT_CEN			/* ‚S“_‚Ì•½‹ÏˆÊ’u‚ðŽg‚¤ */
    } ;

    enum pln {
	Single_Plane ,			/* •Ð–Êƒ|ƒŠƒSƒ“ */
	Dual_Plane			/* —¼–Êƒ|ƒŠƒSƒ“(•\— ”»’è‚ÌŒ‹‰Ê‚ðŒ©‚È‚¢) */
    } ;

typedef struct {
    uint16_t	CTRL ;			/* ƒRƒ“ƒgƒ??[ƒ‹ƒtƒ@ƒ“ƒNƒVƒ‡ƒ“ */
    uint16_t	LINK ;			/* ƒŠƒ“ƒNƒAƒhƒŒƒX */
    uint16_t	PMOD ;			/* ƒvƒbƒgƒ‚?[ƒh */
    uint16_t	COLR ;			/* ƒJƒ‰?[ƒf?[ƒ^ */
    uint16_t	SRCA ;			/* ‚b‚fƒAƒhƒŒƒX */
    uint16_t	SIZE ;			/* ƒLƒƒƒ‰ƒNƒ^ƒTƒCƒY */
    int16_t	    XA ;			/* •\Ž¦ˆÊ’u ‚` */
    int16_t	    YA ;
    int16_t	    XB ;			/* •\Ž¦ˆÊ’u ‚a */
    int16_t	    YB ;
    int16_t	    XC ;			/* •\Ž¦ˆÊ’u ‚b */
    int16_t	    YC ;
    int16_t	    XD ;			/* •\Ž¦ˆÊ’u ‚c */
    int16_t	    YD ;
    uint16_t	GRDA ;			/* ƒO?[ƒ??[ƒVƒF?[ƒfƒBƒ“ƒOƒe?[ƒuƒ‹ */
    int16_t	    Z ;			/* Z distance for the z sort buffer */
    uint32_t *  NEXT ;
} SPRITE_T ;

extern const void* Zbuffer;
extern const SPRITE_T* SpriteBuf;



typedef int16_t ZPOINT[4]; //vertex data = 3 Points, 1 color data

typedef struct {
    uint8_t   vertices[4];
    uint16_t  texno;
    uint16_t  cmdctrl;
    uint16_t  cmdpmod;
    uint16_t  cmdcolr;
} ZPOLYGON; //12 bytes each


typedef struct {
    uint16_t StartPoint;
    uint16_t EndPoint;
    uint16_t StartPol;
    uint16_t EndPol;
    uint16_t LightID; //A copy to transfer to the slave
    int16_t  LightDistance;
} ZPDATA;


#define	    SpriteVRAM		0x25c00000
