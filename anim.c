#include <windows.h>
#include "cge7.h"

#define	ANIMTIMER_ID 99

extern HINSTANCE hInst;
extern AnimBox animsel[9];
extern DWORD colortable[];
extern unsigned char cgrom[4096];
extern int chr[1000];
extern int atr[1000];

int dw, dh;
int animdlg;
int animindex;
myBackBuf abk;

void animdrawchr(int x, int y, int vx, int vy) {
	int i, j, ch, at;
	unsigned char c, *cgr;
	DWORD fc, bc, *p;
	ch = chr[vy*40+vx];
	at = atr[vy*40+vx];
	cgr = &cgrom[((at & 0x0080)<<4)+(ch<<3)];
	fc = colortable[(at & 0x70) >> 4];
	bc = colortable[(at & 0x07)     ];
	for (i=0; i<8; i++) {
	    c = *cgr++;
	    p = (DWORD *)&abk.lpBMP[(abk.h-1-y)*(abk.w)*4 + x*4];
	    for (j=0; j<8; j++) {
		*p++ = (c & 0x80) ? fc : bc;
		c <<= 1;
	    }
	    y++;
	}
}

void CALLBACK do_anim(HWND hwnd, UINT msg, UINT idtimer, DWORD dwtime) {
	HDC hdc;
	RECT r;
	int i, j;

	SetRect(&r, 0, 0, abk.w, abk.h);
	FillRect(abk.hdcMem, &r, (HBRUSH)GetStockObject(BLACK_BRUSH));
	for (i=0; i<(animsel[animindex].y2 - animsel[animindex].y1 + 1); i++) {
	    for (j=0; j<(animsel[animindex].x2 - animsel[animindex].x1 + 1); j++) {
		animdrawchr(j*8, i*8, animsel[animindex].x1 + j, animsel[animindex].y1 + i);
	    }
	}

	hdc = GetDC(hwnd);
	BitBlt(hdc, (dw-abk.w)/2, (dh-abk.h-40)/2, abk.w, abk.h, abk.hdcMem, 0, 0, SRCCOPY);
	ReleaseDC(hwnd, hdc);

	do {
	    animindex++;
	    if (animindex >= 9) animindex = 0;
	} while (animsel[animindex].x1 < 0);
}
void stop_anim(HWND hwnd) {
	KillTimer(hwnd, ANIMTIMER_ID);
}
void start_anim(HWND hwnd) {
	HDC hdc;
	animindex = 0;
	while (animsel[animindex].x1 < 0) {
	    animindex++;
	    if (animindex >= 9) animindex = 0;
	}
	do_anim(hwnd, 0, ANIMTIMER_ID, 0); /* first frame */
	SetTimer(hwnd, ANIMTIMER_ID, 300, (TIMERPROC)do_anim);
}

LRESULT CALLBACK AnimDlgProc(HWND hwnd,UINT iMsg,WPARAM wParam,LPARAM lParam) {
	HWND btn;
	RECT r0, r1;
	int x, y, w, h;

	switch (iMsg) {

	    case WM_INITDIALOG:
		/* サイズ調整 */
		SetRect(&r0, 0, 0, dw, dh);
		AdjustWindowRect(&r0, WS_CAPTION, FALSE);
		w = r0.right - r0.left;
		h = r0.bottom - r0.top;
		GetWindowRect(GetDesktopWindow(), &r1);
		SetWindowPos(hwnd, HWND_TOP,
			     (r1.right - r1.left - w)/2, (r1.bottom - r1.top - h)/2,
			     w, h, SWP_SHOWWINDOW);
		/* ボタン移動 */
		btn = GetDlgItem(hwnd, IDOK);
		GetClientRect(btn, &r0);
		w = r0.right - r0.left;
		h = r0.bottom - r0.top;
		x = (dw - w)/2;
		y = dh - 32;
		MoveWindow(btn, x, y, w, h, TRUE);

		start_anim(hwnd);

		break;

	    case WM_COMMAND: /* コマンドメッセージ */
		switch (LOWORD(wParam)) {
		    case IDOK:
			goto xit;
		}
		break;

	    case WM_CLOSE:
xit:		stop_anim(hwnd);
		destroyBackBuffer(&abk);
		DestroyWindow(hwnd);
		animdlg = 0;
		return TRUE;

	}
	return FALSE;
}

void openAnimDialog(HWND hwMain, int flag) {
	int i, w, h, maxw, maxh;
	maxw = maxh = 0;
	for (i=0; i<9; i++) {
	    if (animsel[i].x1 < 0) continue;
	    w = animsel[i].x2 - animsel[i].x1 + 1;
	    h = animsel[i].y2 - animsel[i].y1 + 1;
	    if (maxw < w) maxw = w;
	    if (maxh < h) maxh = h;
	}
	if (!maxw || !maxh) return;

	dw = maxw * 8;
	dh = maxh * 8;
	createBackBuffer(hwMain, dw, dh, &abk);

	dw += 48;
	dh += 48;
	if (dw < 100) dw = 100;
	if (dh <  80) dh =  80;
	CreateDialog(hInst, MAKEINTRESOURCE(IDD_ANIM), hwMain, (DLGPROC)AnimDlgProc);
	animdlg = 1;
}

int isAnimDlgOpen(void) {
	return animdlg;
}
