#include <windows.h>
#include "CGE7.h"

/*------------------------------------------------
	color table
  ------------------------------------------------*/
DWORD colortable[8] = {
	0x000000,	/* black   */
	0x0000ff,	/* blue    */
	0xff0000,	/* red     */
	0xff00ff,	/* magenta */
	0x00ff00,	/* green   */
	0x00ffff,	/* cyan    */
	0xffff00,	/* yellow  */
	0xffffff	/* white   */
};

/*------------------------------------------------
	back buffer
  ------------------------------------------------*/
void createBackBuffer(HWND hWnd, int w, int h, myBackBuf *bkbuf) {
	HDC	hdcWin, hdcMem;
	HBITMAP	hBMP;
	LPBYTE	lpBMP;
	BITMAPINFO myDIB;

	ZeroMemory(&myDIB, sizeof(BITMAPINFO));
	myDIB.bmiHeader.biSize		= sizeof(BITMAPINFOHEADER); /* BITMAPINFO構造体 */
	myDIB.bmiHeader.biWidth		= w;
	myDIB.bmiHeader.biHeight	= h;
	myDIB.bmiHeader.biPlanes	= 1;
	myDIB.bmiHeader.biBitCount	= 32;
	myDIB.bmiHeader.biCompression	= BI_RGB;

	hdcWin = GetDC(hWnd); /* ウインドウのDC を取得 */

	/* DIB とウインドウのDC からDIBSection を作成 */
	hBMP = CreateDIBSection(hdcWin, &myDIB, DIB_RGB_COLORS, &lpBMP, NULL, 0);
	hdcMem = CreateCompatibleDC(hdcWin); /* メモリDC を作成 */
	SelectObject(hdcMem, hBMP); /* メモリDC にビットマップを選択 */

	ReleaseDC(hWnd, hdcWin);

	bkbuf->hBMP   = hBMP;
	bkbuf->hdcMem = hdcMem;
	bkbuf->lpBMP  = lpBMP;
	bkbuf->hWnd   = hWnd;
	bkbuf->w      = w;
	bkbuf->h      = h;

}

void destroyBackBuffer(myBackBuf *bkbuf) {
	if (bkbuf == NULL) return;
	DeleteDC(bkbuf->hdcMem);
	DeleteObject(bkbuf->hBMP);
	bkbuf->hBMP   = 0;
	bkbuf->hdcMem = 0;
	bkbuf->lpBMP  = 0;
}
