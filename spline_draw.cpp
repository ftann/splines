#include "spline.h"
#include "win_opensave.h"

extern bool bSaveBMP;
extern char* pszBMPPath;

flpn_t fyCoord;

void clsSpline::draw(HDC hDC) {
    HDC hDCMem = CreateCompatibleDC(hDC);
    HBITMAP hbmMem = CreateCompatibleBitmap(hDC, m_Frame.right, m_Frame.bottom);
    HANDLE hOld = NULL;

    if (hbmMem)
        hOld = SelectObject(hDCMem, hbmMem);

    HGDIOBJ hgdiobjOldBrush = NULL;
    HBRUSH hBackGround = CreateSolidBrush(GetSysColor(COLOR_WINDOW));

    if (hBackGround)
        hgdiobjOldBrush = SelectObject(hDC, hBackGround);

    FillRect(hDCMem, &m_Frame, hBackGround);

    if (hgdiobjOldBrush)
        SelectObject(hDC, hgdiobjOldBrush);

    if (hBackGround)
        DeleteObject(hBackGround);

    SetBkMode(hDCMem, TRANSPARENT);

    //draw
    drawAxes(hDCMem);
    if (m_nPoints > 0) {
        calcEquations();

        drawPolynom(hDCMem);
        drawSingleSplines(hDCMem);
        drawSplines(hDCMem);
    }
    drawPoints(hDCMem);

    if (bSaveBMP) {
        saveBMP(pszBMPPath, hbmMem);
        bSaveBMP = false;
    }

    BitBlt(hDC, 0, 0, m_Frame.right, m_Frame.bottom, hDCMem, 0, 0, SRCCOPY);

    SelectObject(hDCMem, hOld);
    DeleteObject(hbmMem);
    DeleteDC(hDCMem);
}

void clsSpline::drawAxes(HDC hDC) {
    if (m_bAxes) {
        HPEN hNewPen = CreatePen(PS_SOLID, SPLINE_WIDTH_AXES, SPLINE_COLOR_AXES());
        HGDIOBJ hgdiobjOldPen = NULL;

        if (hNewPen)
            hgdiobjOldPen = SelectObject(hDC, hNewPen);

        MoveToEx(hDC, m_Frame.right / 2, m_Frame.bottom, NULL);
        LineTo(hDC, m_Frame.right / 2, m_Frame.top);
        MoveToEx(hDC, m_Frame.left, m_Frame.bottom / 2, NULL);
        LineTo(hDC, m_Frame.right, m_Frame.bottom / 2);

        if (hgdiobjOldPen)
            SelectObject(hDC, hgdiobjOldPen);

        if (hNewPen)
            DeleteObject(hNewPen);
    }
}

void clsSpline::drawCoord(HWND hWnd, point_t* p) {
    if (m_bCoord) {
        char* szTextOut = new char[SPLINE_MAX_TEXTLEN];
        HDC hDC = GetDC(hWnd);
        HFONT hNewFont = CreateFont(23, 0, 0, 0, FW_BLACK, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_ONLY_PRECIS, CLIP_TT_ALWAYS, ANTIALIASED_QUALITY, VARIABLE_PITCH, "Arial");
        HGDIOBJ hgdiobjOldFont = NULL;

        if (hNewFont)
            hgdiobjOldFont = SelectObject(hDC, hNewFont);

        SetTextColor(hDC, SPLINE_COLOR_TEXT());

        sprintf(szTextOut, "    % -5.2f / % -5.2f ", p->x, p->y);

        DrawText(hDC, szTextOut, strlen(szTextOut), &m_Frame, DT_RIGHT | DT_BOTTOM | DT_SINGLELINE);

        delete [] szTextOut;

        if (hgdiobjOldFont)
            SelectObject(hDC, hgdiobjOldFont);

        if (hNewFont)
            DeleteObject(hNewFont);

        ReleaseDC(hWnd, hDC);
    }
}

void clsSpline::drawPoints(HDC hDC) {
    if (m_bPoints) {
        int cntPoint;
        for (cntPoint = 0; cntPoint <= m_nPoints; cntPoint++) {
            HPEN hNewPen = CreatePen(PS_SOLID, SPLINE_WIDTH_POINT - 8, SPLINE_COLOR_SPLINE());
            HBRUSH hNewBrush = NULL;
            HGDIOBJ hgdiobjOldPen = NULL;
            HGDIOBJ hgdiobjOldBrush = NULL;

            if (hNewPen)
                hgdiobjOldPen = SelectObject(hDC, hNewPen);

            if (m_nSelectedPoint == cntPoint)
                hNewBrush = CreateSolidBrush(SPLINE_COLOR_POINTSELECTED());
            else
                hNewBrush = CreateSolidBrush(SPLINE_COLOR_POINT());

            if (hNewBrush)
                hgdiobjOldBrush = SelectObject(hDC, hNewBrush);

            Ellipse(hDC,
                    static_cast<int>((cnvWindowCoord_x(m_Points[cntPoint].x) - m_Radius)),
                    static_cast<int>((cnvWindowCoord_y(m_Points[cntPoint].y) - m_Radius)),
                    static_cast<int>((cnvWindowCoord_x(m_Points[cntPoint].x) + m_Radius)),
                    static_cast<int>((cnvWindowCoord_y(m_Points[cntPoint].y) + m_Radius))
                    );


            if (hgdiobjOldBrush)
                SelectObject(hDC, hgdiobjOldBrush);

            if (hNewBrush)
                DeleteObject(hNewBrush);

            if (hgdiobjOldPen)
                SelectObject(hDC, hgdiobjOldPen);

            if (hNewPen)
                DeleteObject(hNewPen);
        }
    }
    return;
}

void clsSpline::drawPolynom(HDC hDC) {
    if (m_bPolynom) {
        calcPolynom();

        HPEN hNewPen = CreatePen(PS_SOLID, SPLINE_WIDTH_POLYNOM, SPLINE_COLOR_POLYNOM());
        HGDIOBJ hgdiobjOldPen = NULL;

        if (hNewPen)
            hgdiobjOldPen = SelectObject(hDC, hNewPen);

        MoveToEx(hDC, 0, static_cast<int>(cnvWindowCoord_y(calcPolynomValueAt(cnvGraphCoord_x(0)))), NULL);

        int cntPolynom;
        for (cntPolynom = 0; cntPolynom < m_Frame.right; cntPolynom++) {
            fyCoord = cnvWindowCoord_y(calcPolynomValueAt(cnvGraphCoord_x(cntPolynom)));

            //if(fyCoord <= m_Frame.bottom && fyCoord >= 0)
            LineTo(hDC, cntPolynom, static_cast<int>(fyCoord));
            //else
            //MoveToEx(hDC, cntPolynom, static_cast<int>(fyCoord), NULL);
        }

        if (hgdiobjOldPen)
            SelectObject(hDC, hgdiobjOldPen);

        if (hNewPen)
            DeleteObject(hNewPen);
    }
}

void clsSpline::drawSingleSplines(HDC hDC) {
    if (m_bSingleSplines) {
        int cntPoint;
        for (cntPoint = 0; cntPoint < m_nPoints; cntPoint++) {
#if (SPLINE_COLOR_SINGLESPLINES_GRADIENT == 1)
            HPEN hNewPen = CreatePen(PS_SOLID, SPLINE_WIDTH_SINGLESPLINES, RGB(0, 255 - cntPoint * 255 / m_nPoints , cntPoint * 255 / m_nPoints));
#else
            HPEN hNewPen = CreatePen(PS_SOLID, SPLINE_WIDTH_SINGLESPLINES, SPLINE_COLOR_SINGLESPLINES());
#endif
            HGDIOBJ hgdiobjOldPen = NULL;

            if (hNewPen)
                hgdiobjOldPen = SelectObject(hDC, hNewPen);

            MoveToEx(hDC, 0, static_cast<int>(cnvWindowCoord_y(calcSplineValueAt(cnvGraphCoord_x(0), cntPoint))), NULL);

            int cntSingleSpline;
            for (cntSingleSpline = 0; cntSingleSpline < m_Frame.right; cntSingleSpline++) {
                fyCoord = cnvWindowCoord_y(calcSplineValueAt(cnvGraphCoord_x(cntSingleSpline), cntPoint));
                //if(fyCoord <= m_Frame.bottom && fyCoord >= 0)
                LineTo(hDC, cntSingleSpline, static_cast<int>(fyCoord));
                //else
                //MoveToEx(hDC, cntSingleSpline, static_cast<int>(fyCoord), NULL);
            }

            if (hgdiobjOldPen)
                SelectObject(hDC, hgdiobjOldPen);

            if (hNewPen)
                DeleteObject(hNewPen);
        }
    }
}

void clsSpline::drawSplines(HDC hDC) {
    if (m_bSpline) {
        MoveToEx(hDC, 0, static_cast<int>(cnvWindowCoord_y(calcSplineValueAt(cnvGraphCoord_x(0), 0))), NULL);

        HPEN hNewPen = CreatePen(PS_SOLID, SPLINE_WIDTH_SPLINE, SPLINE_COLOR_SPLINE());
        HGDIOBJ hgdiobjOldPen = NULL;

        if (hNewPen)
            hgdiobjOldPen = SelectObject(hDC, hNewPen);

        //vom linken Rand bis zum ersten Punkt
        int cntPoint;
        int cntSpline;
        for (cntSpline = 0; cntSpline < static_cast<int>(cnvWindowCoord_x(m_Points[0].x)); cntSpline++) {
            fyCoord = cnvWindowCoord_y(calcSplineValueAt(cnvGraphCoord_x(cntSpline), 0));
            //if(fyCoord <= m_Frame.bottom && fyCoord >= 0)
            LineTo(hDC, cntSpline, static_cast<int>(fyCoord));
            //else
            //MoveToEx(hDC, cntSpline, static_cast<int>(fyCoord), NULL);
        }

        //bis zum letzten Punkt
        for (cntPoint = 0; cntPoint < m_nPoints; cntPoint++)
            for (cntSpline = static_cast<int>(cnvWindowCoord_x(m_Points[cntPoint].x)); cntSpline < cnvWindowCoord_x(m_Points[cntPoint + 1].x); cntSpline++) {
                fyCoord = cnvWindowCoord_y(calcSplineValueAt(cnvGraphCoord_x(cntSpline), cntPoint));
                //if(fyCoord <= m_Frame.bottom && fyCoord >= 0)
                LineTo(hDC, cntSpline, static_cast<int>(fyCoord));
                //else
                //MoveToEx(hDC, cntSpline, static_cast<int>(fyCoord), NULL);
            }

        //vom letzen Punkt bis zum rechten Rand
        for (cntSpline = static_cast<int>(cnvWindowCoord_x(m_Points[m_nPoints].x)); cntSpline < m_Frame.right; cntSpline++) {
            fyCoord = cnvWindowCoord_y(calcSplineValueAt(cnvGraphCoord_x(cntSpline), m_nPoints - 1));
            //if(fyCoord <= m_Frame.bottom && fyCoord >= 0)
            LineTo(hDC, cntSpline, static_cast<int>(fyCoord));
            //else
            //MoveToEx(hDC, cntSpline, static_cast<int>(fyCoord), NULL);
        }

        if (hgdiobjOldPen)
            SelectObject(hDC, hgdiobjOldPen);

        if (hNewPen)
            DeleteObject(hNewPen);
    }
}
