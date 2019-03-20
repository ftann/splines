#include <cstdio>
#include <cstdlib>
#include <windows.h>

#include "resource.h"
#include "spline.h"
#include "win_menu.h"
#include "win_opensave.h"
#include "win_ini.h"
#include "version.h"

#define SHOW_SPLASH 2

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK SplashProc(HWND, UINT, WPARAM, LPARAM);

clsSpline* Spline = new clsSpline();

char szMainClassName[] = "SplinesMainClass";
char szSplashClassName[] = "SplinesSplashClass";
char* pszAppDir = new char[MAX_PATH];
char* pszAppExe = new char[MAX_PATH];
char* pszIniFile = new char[MAX_PATH];
char* pszCompInfo = new char[SPLINE_MAX_TEXTLEN];

HWND hWndGlobalMain;
HWND hWndGlobalSplash;

const UINT tmrSplash = 1;

int nDrawed = 0;

int WINAPI WinMain(HINSTANCE hThisInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nCmdShow) {

    sprintf(pszCompInfo, "%s\n\nCompiled on %s", AutoVersion::FULLVERSION_STRING, __DATE__ );

    HWND hWnd;
    MSG msgMessage;
    WNDCLASSEX wincl;

    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szMainClassName;
    wincl.lpfnWndProc = WndProc;
    wincl.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wincl.cbSize = sizeof(WNDCLASSEX);
    wincl.hIcon = (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(SPLINE_ICON_32), IMAGE_ICON, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_COPYFROMRESOURCE);
    wincl.hIconSm = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(SPLINE_ICON_32));
    wincl.hCursor = LoadCursor(NULL, IDC_ARROW);
    wincl.lpszMenuName = MAKEINTRESOURCE(SPLINE_MENU);
    wincl.cbClsExtra = 0;
    wincl.cbWndExtra = 0;
    wincl.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);

    //Registriere MainWindow
    if (!RegisterClassEx(&wincl))
        return 0;

    wincl.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wincl.lpszClassName = szSplashClassName;
    wincl.lpszMenuName = NULL;
    wincl.lpfnWndProc = SplashProc;

    //Registriere SplashWindow
    if (!RegisterClassEx(&wincl))
        return 0;

    hWnd = CreateWindowEx(
        0,
        szMainClassName,
        TEXT("Splines"),
        WS_OVERLAPPEDWINDOW,
        GetSystemMetrics(SM_CXSCREEN) / 2 - Spline->m_Frame.right / 2,
        GetSystemMetrics(SM_CYSCREEN) / 2 - Spline->m_Frame.bottom / 2,
        Spline->m_Frame.right,
        Spline->m_Frame.bottom,
        HWND_DESKTOP,
        NULL,
        hThisInstance,
        NULL
        );

    hWndGlobalMain = hWnd;

    HWND hSplash = CreateWindow(
        szSplashClassName,
        NULL,
        WS_VISIBLE | WS_BORDER | WS_POPUP,
        GetSystemMetrics(SM_CXSCREEN) / 2 - 240,
        GetSystemMetrics(SM_CYSCREEN) / 2 - 150,
        480,
        300,
        hWnd,
        NULL,
        hThisInstance,
        NULL
        );

    hWndGlobalSplash = hSplash;

    //Argument
    if (strcmp(lpszArgument, "") != 0) {
        char szArg[MAX_PATH];
        unsigned int i;
        unsigned int j = 0;

        for (i = 1; i < strlen(lpszArgument) - 1; i++) {
            szArg[j] = lpszArgument[i];

            if (lpszArgument[i] == '\\') {
                szArg[j + 1] = '\\';
                j += 2;
            } else
                j++;
        }
        szArg[j] = '\0';
        readSPTF(szArg);

        ShowWindow(hWndGlobalMain, SW_SHOW);
    }

    HACCEL haccel = LoadAccelerators(hThisInstance, MAKEINTRESOURCE(SPLINE_ACCEL));

    while (GetMessage(&msgMessage, NULL, 0, 0)) {
        if (!TranslateAccelerator(hWnd, haccel, &msgMessage)) {
            TranslateMessage(&msgMessage);
            DispatchMessage(&msgMessage);
        }
    }
    return msgMessage.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    point_t p;

    switch (message) {
    case WM_CREATE: {
        GetModuleFileName(NULL, pszAppDir, MAX_PATH);
        strcpy(pszAppExe, strrchr(pszAppDir, '\\') + 1);

        if(strstr(pszAppDir, pszAppExe) != 0)
            pszAppDir[strlen(pszAppDir) - strlen(pszAppExe)] = '\0';

        strcpy(pszIniFile, pszAppDir);
        strcat(pszIniFile, "splines.ini");

        FILE* flIniRead =  fopen(pszIniFile, "r");
        if(flIniRead)
            readOptions(hWnd);

        fclose(flIniRead);

        return 0;
    }
    case WM_DESTROY: {
        writeOptions();
        delete [] pszAppDir;
        delete [] pszAppExe;
        delete [] pszIniFile;
        delete [] pszCompInfo;
        PostQuitMessage(0);
        return 0;
    }
    case WM_ERASEBKGND: {
        if (nDrawed == 3) {
            nDrawed = 0;
            return 1;
        } else
            nDrawed++;

        return 0;
    }
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hDC = BeginPaint(hWnd, &ps);
        Spline->draw(hDC);
        EndPaint(hWnd, &ps);
        return 0;
    }
    case WM_COMMAND: {
        MenuProc(hWnd, wParam, lParam);
        return 0;
    }
    case WM_SIZE: {
        Spline->m_Frame.right = LOWORD(lParam);
        Spline->m_Frame.bottom = HIWORD(lParam);
        return 0;
    }
    case WM_LBUTTONDOWN: {
        Spline->m_nClicked = 1;
        p.x = static_cast<flpn_t>(LOWORD(lParam));
        p.y = static_cast<flpn_t>(HIWORD(lParam));
        p = Spline->cnvGraphCoord(p);
        Spline->m_nSelectedPoint = Spline->IntersectionPoint(p);
        if (Spline->m_nSelectedPoint == SPLINE_ERROR) {
            if (Spline->m_nPoints < SPLINE_MAX_POINTS - 1) {
                p.x = static_cast<flpn_t>(LOWORD(lParam));
                p.y = static_cast<flpn_t>(HIWORD(lParam));
                p = Spline->cnvGraphCoord(p);
                Spline->m_nPoints++;
                Spline->m_Points[Spline->m_nPoints].x = p.x;
                Spline->m_Points[Spline->m_nPoints].y = p.y;
            } else
                MessageBox(hWnd, "Maximum number of points reached", "Splines : Error", MB_APPLMODAL | MB_ICONERROR | MB_OK);
        }
        InvalidateRect(hWnd, NULL, TRUE);
        return 0;
    }
    case WM_LBUTTONUP: {
        Spline->m_nClicked = 0;
        Spline->m_nSelectedPoint = SPLINE_ERROR;
        InvalidateRect(hWnd, NULL, TRUE);
        return 0;
    }
    case WM_RBUTTONDOWN: {
        Spline->m_nClicked = 1;
        p.x = static_cast<flpn_t>(LOWORD(lParam));
        p.y = static_cast<flpn_t>(HIWORD(lParam));
        p = Spline->cnvGraphCoord(p);
        Spline->m_nSelectedPoint = Spline->IntersectionPoint(p);
        if (Spline->m_nSelectedPoint != SPLINE_ERROR) {
            if (Spline->m_nPoints > SPLINE_ERROR) {
                Spline->m_Points[Spline->m_nSelectedPoint] = Spline->m_Points[Spline->m_nPoints];
                Spline->m_nPoints--;
                Spline->m_nSelectedPoint = SPLINE_ERROR;
            } else {
                Spline->m_nPoints = SPLINE_ERROR;
                Spline->m_nSelectedPoint = SPLINE_ERROR;
            }
        }
        return 0;
    }
    case WM_RBUTTONUP: {
        InvalidateRect(hWnd, NULL, TRUE);
        return 0;
    }
    case WM_MOUSEMOVE: {
        p.x = static_cast<flpn_t>(LOWORD(lParam));
        p.y = static_cast<flpn_t>(HIWORD(lParam));
        p = Spline->cnvGraphCoord(p);
        if (Spline->m_nSelectedPoint > SPLINE_ERROR && Spline->m_nClicked == 1) {
            Spline->m_Points[Spline->m_nSelectedPoint].x = p.x;
            Spline->m_Points[Spline->m_nSelectedPoint].y = p.y;

            if (!Spline->m_bLowEndSystem)
                InvalidateRect(hWnd, NULL, TRUE);
        }
        Spline->drawCoord(hWnd, &p);
        return 0;
    }
    case WM_MOUSEWHEEL: {
        int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
        if (zDelta < 0) {
            if (Spline->m_xAxis < SPLINE_GRAPH_AXES_MAX) {
                Spline->m_xAxis += SPLINE_GRAPH_AXES_RATIO;
                if (Spline->m_xAxis > SPLINE_GRAPH_AXES_MAX)
                    Spline->m_xAxis = SPLINE_GRAPH_AXES_MAX;

                Spline->m_yAxis = Spline->scaleyAxis(Spline->m_xAxis);// * ((Spline->m_bAutoscale) ? ((flpn_t)Spline->m_Frame.bottom  / (flpn_t)Spline->m_Frame.right) : 1.0);
            } else
                return 0;
        } else if (zDelta > 0) {
            if (Spline->m_xAxis > SPLINE_GRAPH_AXES_MIN) {
                Spline->m_xAxis -= SPLINE_GRAPH_AXES_RATIO;
                if (Spline->m_xAxis < SPLINE_GRAPH_AXES_MIN)
                    Spline->m_xAxis = SPLINE_GRAPH_AXES_MIN;

                Spline->m_yAxis = Spline->scaleyAxis(Spline->m_xAxis);// * ((Spline->m_bAutoscale) ? ((flpn_t)Spline->m_Frame.bottom  / (flpn_t)Spline->m_Frame.right) : 1.0);
            } else
                return 0;
        }
        InvalidateRect(hWnd, NULL, TRUE);
        return 0;
    }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

LRESULT CALLBACK SplashProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    HBITMAP hBitmap = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(SPLINE_SPLASHSCR));

    static unsigned int cntTimerSplash;

    switch (message) {
    case WM_CREATE:
        SetTimer(hWnd, tmrSplash, 500, (TIMERPROC)NULL);
        ShowWindow(hWnd, SW_SHOW);
        break;

    case WM_PAINT: {
        BITMAP bm;
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        HDC hdcMem = CreateCompatibleDC(hdc);
        HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, hBitmap);
        GetObject(hBitmap, sizeof(bm), &bm);

        BitBlt(hdc, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);

        SelectObject(hdcMem, hbmOld);
        DeleteDC(hdcMem);

        EndPaint(hWnd, &ps);

        break;
    }
    case WM_TIMER: {
        if (cntTimerSplash == SHOW_SPLASH) {
            ShowWindow(hWndGlobalMain, SW_SHOW);
            KillTimer(hWnd, tmrSplash);
            DeleteObject(hBitmap);
            ShowWindow(hWnd, SW_HIDE);
        } else
            cntTimerSplash++;

        break;
    }
    case WM_LBUTTONUP:
    case WM_RBUTTONUP:
    case WM_KEYUP:
        if (cntTimerSplash == SHOW_SPLASH)
            ShowWindow(hWnd, SW_HIDE);

        break;

    case WM_DESTROY:
        DeleteObject(hBitmap);
        KillTimer(hWnd, tmrSplash);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
