#ifndef WIN_LABEL_H
#define WIN_LABEL_H

#include <windows.h>

char szLabelClassName[] = "clsLabel";

LRESULT CALLBACK LabelProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_PAINT:
            RECT rect;
            HDC  hDC;
            PAINTSTRUCT ps;
            char szText[200];

            GetClientRect(hWnd, &rect);

            hDC = BeginPaint(hWnd, &ps);
            {
                DrawText(hDC, szText, GetWindowText(hWnd, szText, sizeof(szText)),
                          &rect, DT_WORDBREAK | DT_CENTER);
            }
            EndPaint(hWnd, &ps);

            return 0;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

#endif // WIN_LABEL_H
