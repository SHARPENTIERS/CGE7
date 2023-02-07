#include <windows.h>

/*------------------------------------------------
	undo buffer
  ------------------------------------------------*/
#define	BUFSZ	4096		/* must be 2^n */
#define INC(x) { x = (x+1) & (BUFSZ-1); }
#define DEC(x) { x = (x-1) & (BUFSZ-1); }

typedef struct {
	BYTE	x;
	BYTE	y;
	BYTE	separator;
	BYTE	reserved;
	BYTE	oldc;
	BYTE	olda;
	BYTE	newc;
	BYTE	newa;
} undounit;

HGLOBAL	undobuf;
int	sp, sptop, spbottom;
int	sep_next;

int	spedit;

extern void updateWinTitle(int);

/*------------------------------------------------
	edit flag
  ------------------------------------------------*/
int undoIsModified(void) {
	return (sp != spedit);
}
void undoResetModifiedFlag(void) {
	spedit = sp;
	updateWinTitle(0);
}

/*------------------------------------------------
	initialize/terminate
  ------------------------------------------------*/
int undoInit(void) {
	undobuf = GlobalAlloc(GHND, BUFSZ*sizeof(undounit));
	if (undobuf == NULL) return 0;
	sp = sptop = spbottom = 0;
	spedit = 0;
	return 1;
}
void undoTini(void) {
	if (undobuf != NULL) GlobalFree(undobuf);
	undobuf = NULL;
}

/*------------------------------------------------
	entry data
  ------------------------------------------------*/
void undoStartEntry(void) {
	sep_next = 1;
}
void undoEntry(int x, int y, int oc, int oa, int nc, int na) {
	undounit *p, *p0;
	p0 = (undounit *)GlobalLock(undobuf);
	p = &p0[sp];
	p->x = x;
	p->y = y;
	p->oldc = oc;
	p->olda = oa;
	p->newc = nc;
	p->newa = na;
	p->separator = sep_next;
	sep_next = 0;
	INC(sp);
	sptop = sp;
	/* if buffer is full, delete oldest group */
	if (sp == spbottom) {
	    do { INC(spbottom); } while (!p0[spbottom].separator);
	    spedit = spbottom;
	}
	GlobalUnlock(undobuf);
	updateWinTitle(undoIsModified());
}
void undoFlush(void) {
	sp = sptop = spbottom = 0;
	spedit = 0;
	updateWinTitle(0);
}
void undoCancelRedo(void) {
	sptop = sp;
}

/*------------------------------------------------
	get undo data
  ------------------------------------------------*/
int undoCanUndo(void) {
	return (sp != spbottom);
}
int undoCanRedo(void) {
	return (sp != sptop);
}

/* get one undo/redo data from buffer		*
 *  returns 1 if there are subsequent data	*
 *  returns 0 if the data is the last one	*/
int undoUndo(int *x, int *y, int *c, int *a) {
	undounit *p, *p0;
	int r;
	if (!undoCanUndo()) return -1;
	p0 = (undounit *)GlobalLock(undobuf);
	DEC(sp);
	p = &p0[sp];
	*x = p->x;
	*y = p->y;
	*c = p->oldc;
	*a = p->olda;
	r = !p->separator;
	GlobalUnlock(undobuf);
	updateWinTitle(undoIsModified());
	return r;
}
int undoRedo(int *x, int *y, int *c, int *a) {
	undounit *p, *p0;
	int r;
	if (!undoCanRedo()) return -1;
	p0 = (undounit *)GlobalLock(undobuf);
	p = &p0[sp];
	*x = p->x;
	*y = p->y;
	*c = p->newc;
	*a = p->newa;
	INC(sp);
	if (sp == sptop) r=0;
	else r = !p0[sp].separator;
	GlobalUnlock(undobuf);
	updateWinTitle(undoIsModified());
	return r;
}

undoTest(HWND hwnd) {
	char buf[256];
	wsprintf(buf, "SP:\t%d\ntop:\t%d\nbottom\t%d", sp, sptop, spbottom);
	MessageBox(hwnd, buf, "Undo test", MB_OK);
}

