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
	0x00, 0x8001, 0x8002, 0x8003, 0x8004, 0x8005, 0x8006, 0x8007, 0x8008, 0x8009, 0x800a, 0x800b, 0x800c, 0x800d, 0x800e, 0x800f,
      /* p     q     r     s     t     u     v     w     x     y     z     {     |     }     ~       */
	0x8010, 0x8011, 0x8012, 0x8013, 0x8014, 0x8015, 0x8016, 0x8017, 0x8018, 0x8019, 0x801a, 0x00, 0x79, 0x00, 0xe3, 0
};
int dispck[64] = {
	0x00, 0xbd, 0x9d, 0xb1, 0xb5, 0xb9, 0xb4, 0x9e, 0xb2, 0xb6, 0xba, 0xbe, 0x9f, 0xb3, 0xb7, 0xbb,
	0xbf, 0xa3, 0x85, 0xa4, 0xa5, 0xa6, 0x94, 0x87, 0x88, 0x9c, 0x82, 0x98, 0x84, 0x92, 0x90, 0x83,
	0x91, 0x81, 0x9a, 0x97, 0x93, 0x95, 0x89, 0xa1, 0xaf, 0x8b, 0x86, 0x96, 0xa2, 0xab, 0xaa, 0x8a,
	0x8e, 0xb0, 0xad, 0x8d, 0xa7, 0xa8, 0xa9, 0x8f, 0x8c, 0xae, 0xac, 0x9b, 0xa0, 0x99, 0xbc, 0xb8
};

unsigned char *zentbl =
	"�@�A�B�C�D�F�G�H�I"
	"�J�K�f�[�^�_�`�i�j"
	"�f�h�m�n�u�v�{�|��"
	"������������������"
	"������������������"
	"�O�P�Q�R�S�T�U�V�W�X"
	"�`�a�b�c�d�e�f�g�h�i"
	"�j�k�l�m�n�o�p�q�r�s"
	"�t�u�v�w�x�y"
	"��������������������"
	"��������������������"
	"������������"
	"��������������������"
	"��������������������"
	"��������������������"
	"�����������ÂĂłƂ�"
	"�Ȃɂʂ˂�"
	"�͂΂ςЂт҂ӂԂՂւׂ؂قڂ�"
	"�܂݂ނ߂�"
	"����������������"
	"�@�A�B�C�D�E�F�G�H�I"
	"�J�K�L�M�N�O�P�Q�R�S"
	"�T�U�V�W�X�Y�Z�[�\�]"
	"�^�_�`�a�b�c�d�e�f�g�h"
	"�i�j�k�l�m"
	"�n�o�p�q�r�s�t�u�v�w�x�y�z�{�|"
	"�}�~������"
	"������������������������������"
	"�����ΐ��؋��y���N�����b�~";

int zen2disp[] = {
//	"�@�A�B�C�D�F�G�H�I"
	0x00, 0xB5, 0xBD, 0x2F, 0x2E, 0x4F, 0x2C, 0x49, 0x61,
//	"�J�K�f�[�^�_�`�i�j"
	0xBC, 0xB8, 0x67, 0xBF, 0x2D, 0x59, 0xE3, 0x68, 0x69,
//	"�f�h�m�n�u�v�{�|��"
	0x67, 0x62, 0x52, 0x54, 0x9D, 0xB1, 0x6A, 0x2A, 0x2B,
//	"������������������"
	0x51, 0x57, 0xDD, 0x64, 0xDE, 0x65, 0x63, 0x66, 0x6B,
//	"������������������"
	0x55, 0x48, 0x47, 0x44, 0x43, 0x40, 0x45, 0x50, 0x80,
//	"�O�P�Q�R�S�T�U�V�W�X"
	0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29,
//	"�`�a�b�c�d�e�f�g�h�i"
	0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A,
//	"�j�k�l�m�n�o�p�q�r�s"
	0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14,
//	"�t�u�v�w�x�y"
	0x15, 0x16, 0x17, 0x18, 0x19, 0x1A,
//	"��������������������"
	0x8001, 0x8002, 0x8003, 0x8004, 0x8005, 0x8006, 0x8007, 0x8008, 0x8009, 0x800A,
//	"��������������������"
	0x800B, 0x800C, 0x800D, 0x800E, 0x800F, 0x8010, 0x8011, 0x8012, 0x8013, 0x8014,
//	"������������"
	0x8015, 0x8016, 0x8017, 0x8018, 0x8019, 0x801A,
//	"��������������������"
	0x809E, 0x80A3, 0x80B2, 0x8085, 0x80B6, 0x80A4, 0x80BA, 0x80A5, 0x80BE, 0x80A6,
//	"��������������������"
	0x8094, 0xC094, 0x8087, 0xC087, 0x8088, 0xC088, 0x809C, 0xC09C, 0x8082, 0xC082,
//	"��������������������"
	0x8098, 0xC098, 0x8084, 0xC084, 0x8092, 0xC092, 0x8090, 0xC090, 0x8083, 0xC083,
//	"�����������ÂĂłƂ�"
	0x8091, 0xC091, 0x8081, 0xC081, 0x80BB, 0x809A, 0xC09A, 0x8097, 0xC097, 0x8093, 0xC093,
//	"�Ȃɂʂ˂�"
	0x8095, 0x8089, 0x80A1, 0x80AF, 0x808B,
//	"�͂΂ςЂт҂ӂԂՂւׂ؂قڂ�"
	0x8086, 0xC086, 0xA086, 0x8096, 0xC096, 0xA096, 0x80A2, 0xC0A2, 0xA0A2, 0x80AB, 0xC0AB, 0xA0AB, 0x80AA, 0xC0AA, 0xA0AA,
//	"�܂݂ނ߂�"
	0x808A, 0x808E, 0x80B0, 0x80AD, 0x808D,
//	"����������������"
	0x809F, 0x80A7, 0x80B3, 0x80A8, 0x80B7, 0x80A9, 0x808F, 0x808C, 0x80AE, 0x80AC, 0x809B, 0x80A0, 0x80B4, 0x8099,
//	"�@�A�B�C�D�E�F�G�H�I"
	0x009E, 0x00A3, 0x00B2, 0x0085, 0x00B6, 0x00A4, 0x00BA, 0x00A5, 0x00BE, 0x00A6,
//	"�J�K�L�M�N�O�P�Q�R�S"
	0x0094, 0x4094, 0x0087, 0x4087, 0x0088, 0x4088, 0x009C, 0x409C, 0x0082, 0x4082,
//	"�T�U�V�W�X�Y�Z�[�\�]"
	0x0098, 0x4098, 0x0084, 0x4084, 0x0092, 0x4092, 0x0090, 0x4090, 0x0083, 0x4083,
//	"�^�_�`�a�b�c�d�e�f�g�h"
	0x0091, 0x4091, 0x0081, 0x4081, 0x00BB, 0x009A, 0x409A, 0x0097, 0x4097, 0x0093, 0x4093,
//	"�i�j�k�l�m"
	0x0095, 0x0089, 0x00A1, 0x00AF, 0x008B,
//	"�n�o�p�q�r�s�t�u�v�w�x�y�z�{�|"
	0x0086, 0x4086, 0x2086, 0x0096, 0x4096, 0x2096, 0x00A2, 0x40A2, 0x20A2, 0x00AB, 0x40AB, 0x20AB, 0x00AA, 0x40AA, 0x20AA,
//	"�}�~������"
	0x008A, 0x008E, 0x00B0, 0x00AD, 0x008D,
//	"������������������������������"
	0x009F, 0x00A7, 0x00B3, 0x00A8, 0x00B7, 0x00A9, 0x008F, 0x008C, 0x00AE, 0x00AC, 0x009B, 0x00A0, 0x00B4, 0x0099, 0x40A4,
//	"�����ΐ��؋��y���N�����b�~"
	0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xDB, 0xDC,
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
    int i,j,k;
    HWND ctrl;
    switch (msg) {
        case WM_INITDIALOG:
	    if (tategaki)
		SendMessage(GetDlgItem(hDlgWnd, IDC_CHECK_TATE), BM_SETCHECK, (WPARAM)1, 0L);
	    SetWindowText(GetDlgItem(hDlgWnd, IDC_EDIT_TEXT), buf);
	    SetWinCenter(hDlgWnd);
	    SetFocus(GetDlgItem(hDlgWnd, IDC_EDIT_TEXT));
            return FALSE;
        case WM_COMMAND:
            switch (LOWORD(wp)) {
                case IDOK:
	    	    i = GetWindowText(GetDlgItem(hDlgWnd, IDC_EDIT_TEXT), buf, 1024);
		    buf[i] = 0;
		    tategaki = 0;
		    if (IsDlgButtonChecked(hDlgWnd, IDC_CHECK_TATE)) tategaki = 1;
                    EndDialog(hDlgWnd, IDOK);
                    break;
                case IDCANCEL:
                    EndDialog(hDlgWnd, IDCANCEL);
                    break;
                default:
                    return FALSE;
            }
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

int openTextInputDialog(HINSTANCE hInst, HWND hwnd) {
	unsigned char c, *p;
	int i, j, k;
	if (DialogBox(hInst, MAKEINTRESOURCE(IDD_STRINPUT),
		      hwnd, (DLGPROC)StrInputDlgProc) == IDOK) {
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
	    } else {
		x = y = w = 0;
		h = 1;
	    }
	    while ((c = *p++) != 0) {
		if (c >= 0x20 && c <= 0x7e) k = dispc[c - 0x20];
		else if (c >= 0xa0 && c <= 0xdf) k = dispck[c - 0xa0];
		else if (c == 0x0d) {
		    if (!tategaki) {
			x = 0;
			y++;
			if (y > 24) break;
			h++;
		    } else {
			y = 0;
			x -= 2;
			if (x < 0) break;
			w += 2;
		    }
		    continue;
		} else if (c < 0x20) continue;
		else {
		    unsigned char *zp;
		    zp = zentbl;
		    k = 0;
		    for (i=0; *zp; zp+=2,i++) {
			if (zp[0] == c && zp[1] == *p) {
			    k = zen2disp[i];
			    break;
			}
		    }
		    p++;
		}
		if (!putmoji(k)) break;
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
