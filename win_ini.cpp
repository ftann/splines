#include "win_ini.h"
#include "win_opensave.h"

extern clsSpline* Spline;
extern char* pszIniFile;

void readOptions(HWND hWnd) {
    HMENU hMenu = GetMenu(hWnd);
    char* szInputBuffer = new char[SPLINE_MAX_TEXTLEN];

    GetPrivateProfileString(SECTION_OPTION, KEY_0, "ON", szInputBuffer, SPLINE_MAX_TEXTLEN, pszIniFile);
    if (strncmp(szInputBuffer, "ON", 4) == 0) {
        Spline->m_bAxes = true;
        CheckMenuItem(hMenu, IDM_VIEW_AXES, MF_CHECKED);
    } else {
        Spline->m_bAxes = false;
        CheckMenuItem(hMenu, IDM_VIEW_AXES, MF_UNCHECKED);
    }

    GetPrivateProfileString(SECTION_OPTION, KEY_1, "ON", szInputBuffer, SPLINE_MAX_TEXTLEN, pszIniFile);
    if (strncmp(szInputBuffer, "ON", 4) == 0) {
        Spline->m_bAutoscale = true;
        CheckMenuItem(hMenu, IDM_OPTION_AUTOSCALE, MF_CHECKED);
    } else {
        Spline->m_bAutoscale = false;
        CheckMenuItem(hMenu, IDM_OPTION_AUTOSCALE, MF_UNCHECKED);
    }

    GetPrivateProfileString(SECTION_OPTION, KEY_2, "ON", szInputBuffer, SPLINE_MAX_TEXTLEN, pszIniFile);
    if (strncmp(szInputBuffer, "ON", 4) == 0) {
        Spline->m_bCoord = true;
        CheckMenuItem(hMenu, IDM_VIEW_COORD, MF_CHECKED);
    } else {
        Spline->m_bCoord = false;
        CheckMenuItem(hMenu, IDM_VIEW_COORD, MF_UNCHECKED);
    }

    GetPrivateProfileString(SECTION_OPTION, KEY_3, "ON", szInputBuffer, SPLINE_MAX_TEXTLEN, pszIniFile);
    if (strncmp(szInputBuffer, "ON", 4) == 0) {
        Spline->m_bLowEndSystem = true;
        CheckMenuItem(hMenu, IDM_OPTION_LOWENDSYSTEM, MF_CHECKED);
    } else {
        Spline->m_bLowEndSystem = false;
        CheckMenuItem(hMenu, IDM_OPTION_LOWENDSYSTEM, MF_UNCHECKED);
    }

    GetPrivateProfileString(SECTION_OPTION, KEY_4, "ON", szInputBuffer, SPLINE_MAX_TEXTLEN, pszIniFile);
    if (strncmp(szInputBuffer, "ON", 4) == 0) {
        Spline->m_bPolynom = true;
        CheckMenuItem(hMenu, IDM_VIEW_POLYNOM, MF_CHECKED);
    } else {
        Spline->m_bPolynom = false;
        CheckMenuItem(hMenu, IDM_VIEW_POLYNOM, MF_UNCHECKED);
    }


    GetPrivateProfileString(SECTION_OPTION, KEY_5, "ON", szInputBuffer, SPLINE_MAX_TEXTLEN, pszIniFile);
    if (strncmp(szInputBuffer, "ON", 4) == 0) {
        Spline->m_bPoints = true;
        CheckMenuItem(hMenu, IDM_VIEW_POINTS, MF_CHECKED);
    } else {
        Spline->m_bPoints = false;
        CheckMenuItem(hMenu, IDM_VIEW_POINTS, MF_UNCHECKED);
    }


    GetPrivateProfileString(SECTION_OPTION, KEY_6, "ON", szInputBuffer, SPLINE_MAX_TEXTLEN, pszIniFile);
    if (strncmp(szInputBuffer, "ON", 4) == 0) {
        Spline->m_bSingleSplines = true;
        CheckMenuItem(hMenu, IDM_VIEW_SINGLESPLINES, MF_CHECKED);
    } else {
        Spline->m_bSingleSplines = false;
        CheckMenuItem(hMenu, IDM_VIEW_SINGLESPLINES, MF_UNCHECKED);
    }

    GetPrivateProfileString(SECTION_OPTION, KEY_7, "ON", szInputBuffer, SPLINE_MAX_TEXTLEN, pszIniFile);
    if (strncmp(szInputBuffer, "ON", 4) == 0) {
        Spline->m_bSpline = true;
        CheckMenuItem(hMenu, IDM_VIEW_SPLINE, MF_CHECKED);
    } else {
        Spline->m_bSpline = false;
        CheckMenuItem(hMenu, IDM_VIEW_SPLINE, MF_UNCHECKED);
    }

    delete [] szInputBuffer;
}

void writeOptions(void) {
    WritePrivateProfileString(NULL, NULL, NULL, pszIniFile);
    WritePrivateProfileString(SECTION_OPTION, KEY_0, (Spline->m_bAxes) ? "ON" : "OFF" , pszIniFile);
    WritePrivateProfileString(SECTION_OPTION, KEY_1, (Spline->m_bAutoscale) ? "ON" : "OFF" , pszIniFile);
    WritePrivateProfileString(SECTION_OPTION, KEY_2, (Spline->m_bCoord) ? "ON" : "OFF" , pszIniFile);
    WritePrivateProfileString(SECTION_OPTION, KEY_3, (Spline->m_bLowEndSystem) ? "ON" : "OFF" , pszIniFile);
    WritePrivateProfileString(SECTION_OPTION, KEY_4, (Spline->m_bPolynom) ? "ON" : "OFF" , pszIniFile);
    WritePrivateProfileString(SECTION_OPTION, KEY_5, (Spline->m_bPoints) ? "ON" : "OFF" , pszIniFile);
    WritePrivateProfileString(SECTION_OPTION, KEY_6, (Spline->m_bSingleSplines) ? "ON" : "OFF" , pszIniFile);
    WritePrivateProfileString(SECTION_OPTION, KEY_7, (Spline->m_bSpline) ? "ON" : "OFF" , pszIniFile);
}
