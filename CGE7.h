#ifndef __CGE7_H__
#define __CGE7_H__

#define VERSTRING	"0.15"

#define YOKO	(8+640+10+(16*16+15*2)+8)
#define TATE	(8+400+8)

#define YOKO0	(8+320+10+(16*8+15*2)+8)
#define TATE0	(8+(22*8+21*2)+8)

#define MAINX	8
#define MAINY	8

#define CPALX	(8+640+10)
#define CPALY	(8-3)

#define CPALX0	(8+320+10)
#define CPALY0	(8-3)

#define FLOATERX    0
#define FLOATERY    TATE

#define	IDM_OPEN	1000
#define	IDM_SAVE	1001
#define	IDM_SAVEAS	1002
#define	IDM_TEST	1003
#define	IDM_END		1004
#define	IDM_LOADBMP	1005

#define	IDM_UNDO	1020
#define	IDM_CUT		1021
#define	IDM_COPY	1022
#define	IDM_PASTE	1023
#define IDM_REDO	1024
#define IDM_SELALL	1025
#define IDM_FILL	1026
#define IDM_STRINPUT	1027
#define IDM_BITMAPCOPY	1028
#define IDM_HREVERSE	1029
#define IDM_VREVERSE	1030

#define	IDM_GRID	1040
#define	IDM_EXPAND	1041
#define	IDM_ALTPAL	1042
#define	IDM_ANIMFRAME	1043
#define	IDM_OVERLAY	1044
#define IDM_SHOWSPACE	1045

#define	IDM_DO_ANIM	1060
#define	IDM_ANIM1	1061
#define	IDM_ANIM2	1062
#define	IDM_ANIM3	1063
#define	IDM_ANIM4	1064
#define	IDM_ANIM5	1065
#define	IDM_ANIM6	1066
#define	IDM_ANIM7	1067
#define	IDM_ANIM8	1068
#define	IDM_ANIM9	1069
#define	IDM_ANIM0	1070

#define IDM_SGMODE	1080
#define IDM_SG_LEFT	1081
#define IDM_SG_RIGHT	1082
#define IDM_SG_UP	1083
#define IDM_SG_DOWN	1084

#define	IDM_ABOUT	1999

#define ID_TOOLBAR	100
#define	ID_TOOLBARBMP	101
#define	IDTBB_CURSOR	2000
#define	IDTBB_PENCIL	2001
#define	IDTBB_F_BLACK	2002
#define	IDTBB_F_BLUE	2003
#define	IDTBB_F_RED	2004
#define	IDTBB_F_MAGENTA	2005
#define	IDTBB_F_GREEN	2006
#define	IDTBB_F_CYAN	2007
#define	IDTBB_F_YELLOW	2008
#define	IDTBB_F_WHITE	2009
#define	IDTBB_B_BLACK	2010
#define	IDTBB_B_BLUE	2011
#define	IDTBB_B_RED	2012
#define	IDTBB_B_MAGENTA	2013
#define	IDTBB_B_GREEN	2014
#define	IDTBB_B_CYAN	2015
#define	IDTBB_B_YELLOW	2016
#define	IDTBB_B_WHITE	2017
#define IDTBB_SWAP	2018
#define IDTBB_PAINT	2019
#define IDTBB_SAVE	2020

#define CPAL_REVERSE	0x00000200
#define CPAL_PRCOLOR	0x00000400
#define CPAL_FCMASK	0x00700000
#define CPAL_BCMASK	0x00070000

#define IDD_STRINPUT	5000
#define IDC_EDIT_TEXT	5001
#define IDC_CHECK_TATE	5002

#define	IDD_ANIM	5050

typedef struct myBackBufTag {
	HBITMAP	hBMP;	/* DIBSection */
	HDC	hdcMem;	/* memory dc */
	LPBYTE	lpBMP;	/* pixel buffer address */
	HWND	hWnd;	/* handle of main window */
	int	w;	/* width */
	int	h;	/* height */
} myBackBuf;

typedef struct AnimBoxTag {
	int	x1;
	int	y1;
	int	x2;
	int	y2;
} AnimBox;

#endif /*__CGE7_H__*/
