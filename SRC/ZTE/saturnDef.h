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

#define	    MSBon		(1 << 15)	/* フレ?[ムバッファに?曹ｫ?桙ﾞＭＳＢを１にする */
#define	    HSSon		(1 << 12)	/* ハイスピ?[ドシュリンク有効 */
#define	    HSSoff		(0 << 12)	/* ハイスピ?[ドシュリンク無効(default) */
#define	    No_Window		(0 << 9)	/* ウィンドウの?ｧ限を受けない(default)*/
#define	    Window_In		(2 << 9)	/* ウィンドウの内側に表示 */
#define	    Window_Out		(3 << 9)	/* ウィンドウの外側に表示 */
#define	    MESHoff		(0 << 8)	/* 通?�表示(default) */
#define	    MESHon		(1 << 8)	/* �?ッシュで表示 */
#define	    ECdis		(1 << 7)	/* エンドコ?[ドをパレットのひとつとして使用 */
#define	    ECenb		(0 << 7)	/* エンドコ?[ド有効 */
#define	    SPdis		(1 << 6)	/* スペ?[スコ?[ドをパレットのひとつとして使用 */
#define	    SPenb		(0 << 6)	/* スペ?[スは表示しない(default) */
#define	    CL16Bnk		(0 << 3)	/* カラ?[バンク１６?Fモ?[ド (default) */
#define	    CL16Look		(1 << 3)	/* カラ?[ルックアップ１６?Fモ?[ド */
#define	    CL64Bnk		(2 << 3)	/* カラ?[バンク６４?Fモ?[ド */
#define	    CL128Bnk		(3 << 3)	/* カラ?[バンク１２８?Fモ?[ド */
#define	    CL256Bnk		(4 << 3)	/* カラ?[バンク２５６?Fモ?[ド */
#define	    CL32KRGB		(5 << 3)	/* ＲＧＢ３２Ｋ?Fモ?[ド */
#define	    CL_Replace		0		/* ?dね?曹ｫ(?�?曹ｫ)モ?[ド */
#define	    CL_Shadow		1		/* 影モ?[ド */
#define	    CL_Half		2		/* 半輝度モ?[ド */
#define	    CL_Trans		3		/* 半透明モ?[ド */
#define	    CL_Gouraud		4		/* グ?[�??[シェ?[ディングモ?[ド */

#define	    UseTexture		(1 << 2)	/* テクスチャを貼るポリゴン */
#define	    UseLight		(1 << 3)	/* 光源の影響を受けるポリゴン */
#define	    UsePalette		(1 << 5)	/* ポリゴンのカラ?[ */

/* SGL Ver3.0 Add */
#define	    UseNearClip		(1 << 6)	/* ニア?Eクリッピングをする */
#define	    UseGouraud		(1 << 7)	/* リアルグ?[�??[ */
#define	    UseDepth		(1 << 4)	/* デプスキュ?[ */
/* */

#if 0 /* SGL Ver3.0 Delete */
#define	    UseClip		(1 << 4)		/* 画面外に頂点が?oたら表示しない */
#else
#define	    UseClip		UseNearClip		/* ニア?Eクリッピングをする */
#endif

#define	    sprHflip		((1 << 4) | FUNC_Texture | (UseTexture << 16))
#define	    sprVflip		((1 << 5) | FUNC_Texture | (UseTexture << 16))
#define	    sprHVflip		((3 << 4) | FUNC_Texture | (UseTexture << 16))
#define	    sprNoflip		((0) | FUNC_Texture | (UseTexture << 16))
#define	    sprPolygon		(FUNC_Polygon | ((ECdis | SPdis) << 24))
#define	    sprPolyLine		(FUNC_PolyLine | ((ECdis | SPdis) << 24))
#define	    sprLine		(FUNC_Line | ((ECdis | SPdis) << 24))

#define	    No_Texture		0		/* テクスチャを使用しない時 */
#define	    No_Option		0		/* オプションを使用しない時 */
#define	    No_Gouraud		0		/* グ?[�??[シェ?[ディングを使用しない時 */
#define	    No_Palet	        0		/* カラ?[パレットの指定がいらない時 */
#define	    GouraudRAM		(0x00080000 - (32*8))	/* 光源の影響用のグ?[�??[バッファ */

#define	    COL_16	(2+1)		/* カラ?[バンク１６?Fモ?[ド */
#define	    COL_64	(2+0)		/* カラ?[バンク６４?Fモ?[ド */
#define	    COL_128	(2+0)		/* カラ?[バンク１２８?Fモ?[ド */
#define	    COL_256	(2+0)		/* カラ?[バンク２５６?Fモ?[ド */
#define	    COL_32K	(2-1)		/* ＲＧＢ３２Ｋ?Fモ?[ド */

    enum base {
	SORT_BFR ,			/* 直前に表示したポリゴンの位置を使う */
	SORT_MIN ,			/* ４点の内?A一番手前の点を使う */
	SORT_MAX ,			/* ４点の内一番遠い点を使う */
	SORT_CEN			/* ４点の平均位置を使う */
    } ;

    enum pln {
	Single_Plane ,			/* 片面ポリゴン */
	Dual_Plane			/* 両面ポリゴン(表裏判定の結果を見ない) */
    } ;

typedef struct {
    uint16_t	CTRL ;			/* コント�??[ルファンクション */
    uint16_t	LINK ;			/* リンクアドレス */
    uint16_t	PMOD ;			/* プットモ?[ド */
    uint16_t	COLR ;			/* カラ?[デ?[タ */
    uint16_t	SRCA ;			/* ＣＧアドレス */
    uint16_t	SIZE ;			/* キャラクタサイズ */
    int16_t	    XA ;			/* 表示位置 Ａ */
    int16_t	    YA ;
    int16_t	    XB ;			/* 表示位置 Ｂ */
    int16_t	    YB ;
    int16_t	    XC ;			/* 表示位置 Ｃ */
    int16_t	    YC ;
    int16_t	    XD ;			/* 表示位置 Ｄ */
    int16_t	    YD ;
    uint16_t	GRDA ;			/* グ?[�??[シェ?[ディングテ?[ブル */
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
