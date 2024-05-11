/*=====================================================================
	text input
 ======================================================================*/
#include <windows.h>
#include "CGE7.h"

extern int floaterchr[1000];
extern int floateratr[1000];
extern int forecolor;
extern int backcolor;
extern int floater;

extern void show_selection(int, int, int, int);
extern void refresh_floater(void);

unsigned char buf[1024];
int tategaki;
int charset2;

/* ASCII to MZ-700 Display code table */
int dispc[96] = {
   /* SP    !     "     #     $     %     &     '     (     )     *     +     ,     -     .     / */
	0x00, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6b, 0x6a, 0x2f, 0x2a, 0x2e, 0x2d,
   /* 0     1     2     3     4     5     6     7     8     9     :     ;     <     =     >     ? */
	0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x4f, 0x2c, 0x51, 0x2b, 0x57, 0x49,
   /* @     A     B     C     D     E     F     G     H     I     J     K     L     M     N     O */
	0x55, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
   /* P     Q     R     S     T     U     V     W     X     Y     Z     [     \     ]     ^     _ */
	0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x52, 0x59, 0x54, 0x50, 0x3c,
   /* `     a     b     c     d     e     f     g     h     i     j     k     l     m     n     o */
	0xA4, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f,
   /* p     q     r     s     t     u     v     w     x     y     z     {     |     }     ~       */
	0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9a, 0xbc, 0x79, 0x40, 0xe3, 0
};

/*----------------------------------------------------------------------
	
 ----------------------------------------------------------------------*/
int SetWinCenter(HWND hWnd)
{
    HWND hDeskWnd;
    RECT deskrc, rc;
    int x, y;

    hDeskWnd = GetDesktopWindow();
    GetWindowRect(hDeskWnd, (LPRECT)&deskrc);
    GetWindowRect(hWnd, (LPRECT)&rc);
    x = (deskrc.right - (rc.right-rc.left)) / 2;
    y = (deskrc.bottom - (rc.bottom-rc.top)) / 2;
    SetWindowPos(hWnd, HWND_TOP, x, y, (rc.right-rc.left), (rc.bottom-rc.top),SWP_SHOWWINDOW);
    return 0;
}

LRESULT CALLBACK StrInputDlgProc(HWND hDlgWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	int i;
    switch (msg) {
        case WM_INITDIALOG:
			if (tategaki)
			SendMessage(GetDlgItem(hDlgWnd, IDC_CHECK_TATE), BM_SETCHECK, (WPARAM)1, 0L);
			SetWindowText(GetDlgItem(hDlgWnd, IDC_EDIT_TEXT), buf);
			SendMessage(GetDlgItem(hDlgWnd, IDC_EDIT_TEXT), EM_SETSEL, 0, -1);
			SetWinCenter(hDlgWnd);
			SetFocus(GetDlgItem(hDlgWnd, IDC_EDIT_TEXT));
            return FALSE;
        case WM_COMMAND:
            switch (LOWORD(wp)) {
                case IDOK:
	    			i = GetWindowText(GetDlgItem(hDlgWnd, IDC_EDIT_TEXT), buf, 1024);
					buf[i] = 0;
					tategaki = 0;
					charset2 = 0;
					if (IsDlgButtonChecked(hDlgWnd, IDC_CHECK_TATE)) { tategaki = 1; }
					if (IsDlgButtonChecked(hDlgWnd, IDC_CHECK_2NDCS)) { charset2 = 1; }
                    EndDialog(hDlgWnd, IDOK);
                    break;
                case IDCANCEL:
                    EndDialog(hDlgWnd, IDCANCEL);
                    break;
                default:
                    return FALSE;
            }
    		break;
        default:
            return FALSE;
    }
    return TRUE;
}

/************************************************************
	Open Dialog
 ************************************************************/
int x, y, w, h;
int incx(void) {
	x++;
	if (w < x) w = x;
	if (x > 38) {
	    x = 0;
	    y++;
	    if (y > 24) return 0;
	    h++;
	}
	return 1;
}
int incy(void) {
	y++;
	if (h < y) h = y;
	if (y > 24) {
	    y = 0;
	    x -= 2;
	    if (x < 0) return 0;
	    w += 2;
	}
	return 1;
}
int putmoji(int k) {
	int i, c, a;
	i = y*40+x;
	c = k & 0x00ff;
	a = ((k & 0x8000)>>8) | (forecolor << 4) | backcolor;
	floaterchr[i] = c;
	floateratr[i] = a;
	if (!tategaki) {
	    if (!incx()) return 0;
	}
	i++;
	if (k & 0x4000) {
	    floaterchr[i] = 0xBC;
	    floateratr[i] = a;
	    if (!tategaki) {
		if (!incx()) return 0;
	    }
	} else if (k & 0x2000) {
	    floaterchr[i] = 0xB8;
	    floateratr[i] = a;
	    if (!tategaki) {
		if (!incx()) return 0;
	    }
	}
	if (tategaki) {
	    if (!incy()) return 0;
	}
	return 1;
}

void openTextInputDialog(HINSTANCE hInst, HWND hwnd) {
	unsigned char c, *p;
	int i, j, k;
	if (DialogBox(hInst, MAKEINTRESOURCE(IDD_STRINPUT), hwnd, (DLGPROC)StrInputDlgProc) == IDOK) {
	    for (i=0; i<1000; i++) {
			floaterchr[i] = 0;
			floateratr[i] = (forecolor << 4) | backcolor;
	    }
	    p = buf;
	    if (tategaki) {
			x = 38;
			y =  0;
			w = 2;
			h = 0;
	    }
		else {
			x = y = w = 0;
			h = 1;
	    }
	    while ((c = *p++) != 0) {
			

			if (c >= 0x20 && c <= 0x7e) {
				k = dispc[c - 0x20];
				if (c >= 0x41 && c <= 0x5A && charset2 == 1) { k += 0x8000; }
				if (!putmoji(k)) break;
			}

			else if (c == 0x0d) {
			 
				if (!tategaki) {
					x = 0;
					y++;
					if (y > 24) break;
					h++;
				}
				else {
					y = 0;
					x -= 2;
					if (x < 0) break;
					w += 2;
			    }
			}
	    }

		if (tategaki) {
			for (i=0; i<h; i++) {
			    for (j=0; j<w; j++) {
					k = i*40+j;
					floaterchr[k] = floaterchr[k+40-w];
					floateratr[k] = floateratr[k+40-w];
				}
			}
	    }

		show_selection(1, 1, w, h);
	    floater = 1;
	    refresh_floater();
	}
}
