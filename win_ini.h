#ifndef WIN_INI_H
#define WIN_INI_H

#include <windows.h>

#include "spline.h"
#include "win_menu.h"
#include "resource.h"

#define SECTION_OPTION "OPT"
#define SECTION_LANGUAGE "LNG"

#define KEY_0 "AXS"     //Axes
#define KEY_1 "AXC"     //Autoscale
#define KEY_2 "CRD"     //Coord
#define KEY_3 "LES"     //Low-End-System
#define KEY_4 "POL"     //Polynom
#define KEY_5 "PTS"     //Points
#define KEY_6 "SSP"     //SingleSplines
#define KEY_7 "SPL"     //Spline

//#define KEY_8 "LNG"     //Language


void readLanguage(void);
void readOptions(HWND);
void writeOptions(void);

#endif // WIN_INI_H
