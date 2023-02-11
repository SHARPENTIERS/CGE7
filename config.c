/*=====================================================================
	save/load config
 ======================================================================*/
#include <windows.h>
#include "CGE7.h"

char INIFilePath[MAX_PATH];

extern int showgrid;
extern int expansion;
extern int zoomratio;
extern int showdispc00;
extern int RecentFileIndex[9];
extern char RecentFiles[9][MAX_PATH];

struct {
	char *name;
	int  *var;
	int  defvar;
	char *str;
} ConfigList[] = {
	{ "ShowGrid",	&showgrid,	1,	NULL },
	{ "Zoom",	&expansion,	1,	NULL },
	{ "ZoomRatio",	&zoomratio,	1,	NULL },
	{ "MarkDispc0",	&showdispc00,	0,	NULL },
	{ NULL,		NULL,		0,	NULL }
};

/*----------------------------------------------------------------------
	CGEdit format
 ----------------------------------------------------------------------*/
void load_config() {
	int i;
	char buf[256];

	GetCurrentDirectory(MAX_PATH , INIFilePath);
	wsprintf(INIFilePath, "%s\\%s", INIFilePath, "CGE7.ini");

	for (i=0; ConfigList[i].name != NULL; i++) {

	    if (ConfigList[i].var != NULL) {
		*(ConfigList[i].var) = GetPrivateProfileInt("Config", ConfigList[i].name, ConfigList[i].defvar, INIFilePath);
	    } else if (ConfigList[i].str != NULL) {
		GetPrivateProfileString("Config", ConfigList[i].name, "", ConfigList[i].str, MAX_PATH, INIFilePath);
	    }

	}

	for (i=0; i<9; i++) {
	    wsprintf(buf, "RecentFile%d", i+1);
	    GetPrivateProfileString("RecentFiles", buf, "", &RecentFiles[i][0], MAX_PATH, INIFilePath);
	    RecentFileIndex[i] = (RecentFiles[i][0] == 0) ? -1 : i;
	}
}

void save_config() {
	int i, j;
	char buf[4096 + MAX_PATH * 10];

	for (i=0, j=0; ConfigList[i].name != NULL; i++) {

	    if (ConfigList[i].var != NULL) {
		j += wsprintf(&buf[j], "%s=%d", ConfigList[i].name, *(ConfigList[i].var));
		j++;
	    } else if (ConfigList[i].str != NULL) {
		j += wsprintf(&buf[j], "%s=%s", ConfigList[i].name, ConfigList[i].str);
		j++;
	    }

	}
	buf[j] = 0;
	WritePrivateProfileSection("Config", buf, INIFilePath);

	for (i=0, j=0; i<9; i++) {
	    if (RecentFileIndex[i] >= 0) {
		j += wsprintf(&buf[j], "RecentFile%d=%s", i+1, &RecentFiles[RecentFileIndex[i]][0]);
		j++;
	    } else {
		j += wsprintf(&buf[j], "RecentFile%d=", i+1);
		j++;
	    }
	}
	buf[j] = 0;
	WritePrivateProfileSection("RecentFiles", buf, INIFilePath);
}
