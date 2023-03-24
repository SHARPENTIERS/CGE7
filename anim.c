#include <windows.h>
#include <commctrl.h>
#include "cge7.h"

#define	ANIMTIMER_ID 99

extern HINSTANCE hInst;
extern AnimBox animsel[9];
extern DWORD colortable[];
extern unsigned char cgrom[4096];
extern int chr[1000];
extern int atr[1000];
extern int expansion;

int dw, dh;
int animdlg;
int animindex;
int duration;
myBackBuf abk;

void animdrawchr(int x, int y, int vx, int vy) {
	int ch, at;
	int i,j,k,l;
	int yy;
	unsigned char c, *cgr, m;
	DWORD fc, bc, *p1;
	ch = chr[vy*40+vx];
	at = atr[vy*40+vx];
	cgr = &cgrom[((at & 0x0080)<<4)+(ch<<3)];
	fc = colortable[(at & 0x70) >> 4];
	bc = colortable[(at & 0x07)     ];
	yy = y;
	for (i=0; i<8; i++) {
	    m = *cgr++;
	    for (k=0; k<expansion+1; k++) {
		p1 = (DWORD *)&abk.lpBMP[(abk.h-1-yy)*(abk.w)*4 + x*4];
		c = m;
		for (j=0; j<8; j++) {
		    if (c & 0x80) {
			for (l=0; l<expansion+1; l++) *p1++ = fc;
		    } else {
			for (l=0; l<expansion+1; l++) *p1++ = bc;
		    }
		    c <<= 1;
		}
		yy++;
	    }
	}

}

void CALLBACK do_anim(HWND hwnd, UINT msg, UINT idtimer, DWORD dwtime) {
	HDC hdc;
	RECT r;
	int i, j, w;

	w = 8 * (expansion + 1);
	SetRect(&r, 0, 0, abk.w, abk.h);
	FillRect(abk.hdcMem, &r, (HBRUSH)GetStockObject(BLACK_BRUSH));
	for (i=0; i<(animsel[animindex].y2 - animsel[animindex].y1 + 1); i++) {
	    for (j=0; j<(animsel[animindex].x2 - animsel[animindex].x1 + 1); j++) {
		animdrawchr(j*w, i*w, animsel[animindex].x1 + j, animsel[animindex].y1 + i);
	    }
	}

	hdc = GetDC(hwnd);
	BitBlt(hdc, (dw-abk.w)/2, (dh-abk.h-40-48)/2, abk.w, abk.h, abk.hdcMem, 0, 0, SRCCOPY);
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
	while (animsel[animindex].x1 < 0) {
	    animindex++;
	    if (animindex >= 9) animindex = 0;
	}
	do_anim(hwnd, 0, ANIMTIMER_ID, 0); /* first frame */
	SetTimer(hwnd, ANIMTIMER_ID, duration, (TIMERPROC)do_anim);
}

LRESULT CALLBACK AnimDlgProc(HWND hwnd,UINT iMsg,WPARAM wParam,LPARAM lParam) {
	HWND btn, sld;
	RECT r0, r1;
	int x, y, w, h, pos;

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

		/* スライダー移動 */
		sld = GetDlgItem(hwnd, IDC_SLIDER1);
		GetClientRect(sld, &r0);
		w = dw - 16;
		h = r0.bottom - r0.top;
		x = (dw - w)/2;
		y = dh - 32 - 48;
		MoveWindow(sld, x, y, w, h, TRUE);
		SendMessage(sld, TBM_SETRANGE, TRUE, MAKELPARAM(0, 100));
		SendMessage(sld, TBM_SETTICFREQ, 10, 0);
		SendMessage(sld, TBM_SETPOS, TRUE, 50);
		SendMessage(sld, TBM_SETPAGESIZE, 0, 10);

		animindex = 0;
		duration = 192;
		start_anim(hwnd);

		break;

	    case WM_COMMAND: /* コマンドメッセージ */
		switch (LOWORD(wParam)) {
		    case IDOK:
			goto xit;
		}
		break;

	    case WM_HSCROLL:
		sld = GetDlgItem(hwnd, IDC_SLIDER1);
		if (sld == (HWND)lParam) {
		    pos = SendMessage(sld, TBM_GETPOS, NULL, NULL);
		    pos = 32 * (pos / 10 + 1);
		    if (pos != duration) {
			duration = pos;
			stop_anim(hwnd);
			start_anim(hwnd);
		    }
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

	dw = maxw * 8 * (expansion+1);
	dh = maxh * 8 * (expansion+1);
	createBackBuffer(hwMain, dw, dh, &abk);

	dw += 48;
	dh += 48 + 48;
	if (dw < 100) dw = 100;
	if (dh < 120) dh = 120;
	CreateDialog(hInst, MAKEINTRESOURCE(IDD_ANIM), hwMain, (DLGPROC)AnimDlgProc);
	animdlg = 1;
}

int isAnimDlgOpen(void) {
	return animdlg;
}
