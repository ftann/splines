#include "win_menu.h"
#include "version.h"

#include <iostream>

extern char* pszCompInfo;
extern clsSpline* Spline;
extern HWND hWndGlobalSplash;

void modMenu(HMENU* hMenu, UINT nID, bool* bSetting) {
    if (*bSetting) {
        CheckMenuItem(*hMenu, nID, MF_UNCHECKED);
        *bSetting = false;
    } else {
        CheckMenuItem(*hMenu, nID, MF_CHECKED);
        *bSetting = true;
    }
}

void MenuProc(HWND hWnd, WPARAM wParam, LPARAM lParam) {
    HMENU hMenu = GetMenu(hWnd);
    switch (LOWORD(wParam)) {
    case IDM_FILE_NEW:
        memset(Spline->m_Points, 0, sizeof(Spline->m_Points));
        Spline->m_nPoints = SPLINE_ERROR;
        InvalidateRect(hWnd, NULL, TRUE);
        break;

    case IDM_FILE_OPEN:
        readSPFFile(hWnd);
        break;

    case IDM_FILE_SAVE:
        saveSPFFile(hWnd);
        break;

    case IDM_FILE_SAVEASBITMAP:
        saveBMPFile(hWnd);
        break;

    case IDM_FILE_EXIT:
        DestroyWindow(hWnd);
        break;

    case IDM_VIEW_AXES:
        modMenu(&hMenu, IDM_VIEW_AXES, &Spline->m_bAxes);
        InvalidateRect(hWnd, NULL, TRUE);
        break;

    case IDM_VIEW_COORD:
        modMenu(&hMenu, IDM_VIEW_COORD, &Spline->m_bCoord);
        InvalidateRect(hWnd, NULL, TRUE);
        break;

    case IDM_VIEW_POINTS:
        modMenu(&hMenu, IDM_VIEW_POINTS, &Spline->m_bPoints);
        InvalidateRect(hWnd, NULL, TRUE);
        break;

    case IDM_VIEW_POLYNOM:
        modMenu(&hMenu, IDM_VIEW_POLYNOM, &Spline->m_bPolynom);
        InvalidateRect(hWnd, NULL, TRUE);
        break;

    case IDM_VIEW_SINGLESPLINES:
        modMenu(&hMenu, IDM_VIEW_SINGLESPLINES, &Spline->m_bSingleSplines);
        InvalidateRect(hWnd, NULL, TRUE);
        break;

    case IDM_VIEW_SPLINE:
        modMenu(&hMenu, IDM_VIEW_SPLINE, &Spline->m_bSpline);
        InvalidateRect(hWnd, NULL, TRUE);
        break;

    case IDM_VIEW_AUTOZOOM:
        flpn_t nPointA = fabs(Spline->m_Points[0].x);
        flpn_t nPointB = fabs(Spline->m_Points[Spline->m_nPoints].x);

        if (nPointA < nPointB) {
            Spline->m_xAxis = nPointB * 2;
            Spline->m_yAxis = Spline->scaleyAxis(nPointB * 2);
        } else if (nPointA > nPointB) {
            Spline->m_xAxis = nPointA * 2;
            Spline->m_yAxis = Spline->scaleyAxis(nPointA * 2);
        } else {
            Spline->m_xAxis = nPointA * 2;
            Spline->m_yAxis = Spline->scaleyAxis(nPointA * 2);
        }

        nPointA = 0;
        for (int i = 0; i <= Spline->m_nPoints; i++) {
            if (fabs(Spline->m_Points[i].y) > nPointA)
                nPointA = fabs(Spline->m_Points[i].y);
        }
        if (nPointA * 2 > Spline->m_yAxis) {
            Spline->m_xAxis = Spline->scalexAxis(nPointA * 2);
            Spline->m_yAxis = nPointA * 2;
        }

        InvalidateRect(hWnd, NULL, TRUE);
        break;

    case IDM_VIEW_REDRAW:
        InvalidateRect(hWnd, NULL, TRUE);
        break;


    case IDM_OPTION_AUTOSCALE:
        modMenu(&hMenu, IDM_OPTION_AUTOSCALE, &Spline->m_bAutoscale);
        InvalidateRect(hWnd, NULL, TRUE);
        break;

    case IDM_OPTION_LOWENDSYSTEM:
        modMenu(&hMenu, IDM_OPTION_LOWENDSYSTEM, &Spline->m_bLowEndSystem);
        break;

    case IDM_OPTION_STANDARD:
        if (Spline->m_bAutoscale != true)
            modMenu(&hMenu, IDM_OPTION_AUTOSCALE, &Spline->m_bAutoscale);

        if (Spline->m_bAxes != true)
            modMenu(&hMenu, IDM_VIEW_AXES, &Spline->m_bAxes);

        if (Spline->m_bCoord != true)
            modMenu(&hMenu, IDM_VIEW_COORD, &Spline->m_bCoord);

        if (Spline->m_bLowEndSystem != false)
            modMenu(&hMenu, IDM_OPTION_LOWENDSYSTEM, &Spline->m_bLowEndSystem);

        if (Spline->m_bPoints != true)
            modMenu(&hMenu, IDM_VIEW_POINTS, &Spline->m_bPoints);

        if (Spline->m_bPolynom != false)
            modMenu(&hMenu, IDM_VIEW_POLYNOM, &Spline->m_bPolynom);

        if (Spline->m_bSingleSplines != false)
            modMenu(&hMenu, IDM_VIEW_SINGLESPLINES, &Spline->m_bSingleSplines);

        if (Spline->m_bSpline != true)
            modMenu(&hMenu, IDM_VIEW_SPLINE, &Spline->m_bSpline);

        Spline->m_xAxis = SPLINE_GRAPH_AXES_STD;
        Spline->m_yAxis = Spline->scaleyAxis(SPLINE_GRAPH_AXES_STD);

        InvalidateRect(hWnd, NULL, TRUE);
        break;

    case IDM_HELP_ABOUT:
        ShowWindow(hWndGlobalSplash, SW_SHOW);
        break;

    case IDM_HELP_HELP:
        MessageBox(hWnd, "Help is not implemented yet.\n\nPlease contact florian.tanner@gmail.com", "Splines : Help", MB_OK | MB_ICONWARNING);
        break;

    case IDM_HELP_VERSION:
        MessageBox(hWnd, pszCompInfo, "Splines : Version", MB_OK | MB_ICONINFORMATION);
        break;
    }
    return;
}
