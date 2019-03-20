#include "win_opensave.h"

extern clsSpline* Spline;
extern char* pszLastOpen;

bool bSaveBMP = false;
char* pszBMPPath = new char[MAX_PATH];

void readSPTF(char* pszPath) {
    FILE* flOpen = fopen(pszPath, "r");
    if (!flOpen) {
        MessageBox(NULL, "Can not open file", "Splines : Error", MB_APPLMODAL | MB_ICONERROR | MB_OK);
        return;
    }

    int i = 0;
    char szRead[SPLINE_MAX_TEXTLEN];
    char* pszArg = NULL;

    memset(Spline->m_Points, 0, sizeof(Spline->m_Points));
    Spline->m_nPoints = 0;

    while (feof(flOpen) == 0 && i < SPLINE_MAX_POINTS) {
        fgets(szRead, SPLINE_MAX_TEXTLEN, flOpen);

        pszArg = strtok(szRead, TEXT("/"));
        Spline->m_Points[i].x = atof(pszArg);
        pszArg = strtok(NULL, TEXT("/"));
        Spline->m_Points[i].y = atof(pszArg);

        i++;
    }

    Spline->m_nPoints = i - 1;

    fclose(flOpen);
}

void readSPFFile(HWND hWnd) {
    OPENFILENAME ofn;
    char szFile[MAX_PATH];

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "All\0*.*\0Splines Point File\0*.sptf\0";
    ofn.nFilterIndex = 2;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;

    char szBuf[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, szBuf);

    ofn.lpstrInitialDir = szBuf;
    ofn.lpstrTitle = "Splines : File Open";
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileName(&ofn) == TRUE)
        readSPTF(ofn.lpstrFile);
}

void saveSPTF(char* pszPath) {
    if (strstr(pszPath, SPLINE_POINT_FILE) == NULL)
        strcat(pszPath, SPLINE_POINT_FILE);

    FILE* flSave = fopen(pszPath, "w");
    if (!flSave) {
        MessageBox(NULL, "Can not save file", "Splines : Error", MB_APPLMODAL | MB_ICONERROR | MB_OK);
        return;
    }

    int i = 0;
    while (i <= Spline->m_nPoints) {
        fprintf(flSave, "%f/%f/", Spline->m_Points[i].x, Spline->m_Points[i].y);
        if (i < Spline->m_nPoints)
            fprintf(flSave, "\n");

        i++;
    }

    fclose(flSave);
}

void saveSPFFile(HWND hWnd) {
    OPENFILENAME ofn;
    char szFile[MAX_PATH];

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "All\0*.*\0Splines Point File\0*.sptf\0";
    ofn.nFilterIndex = 2;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;

    char szBuf[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, szBuf);

    ofn.lpstrInitialDir = szBuf;
    ofn.lpstrTitle = "Splines : File Save";
    ofn.Flags = OFN_CREATEPROMPT | OFN_OVERWRITEPROMPT;

    if (GetSaveFileName(&ofn) == TRUE)
        saveSPTF(ofn.lpstrFile);
}

void saveBMP(char *pszFilename, HBITMAP hBitmap) {
    if (strstr(pszFilename, SPLINE_IMAGE_FILE) == NULL)
        strcat(pszFilename, SPLINE_IMAGE_FILE);

    HDC hdc = NULL;
    FILE* fp = NULL;
    LPVOID pBuf = NULL;
    BITMAPINFO bmpInfo;
    BITMAPFILEHEADER bmpFileHeader;

    do {
        hdc = GetDC(NULL);
        ZeroMemory(&bmpInfo, sizeof(BITMAPINFO));
        bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        GetDIBits(hdc, hBitmap, 0, 0, NULL, &bmpInfo, DIB_RGB_COLORS);

        if (bmpInfo.bmiHeader.biSizeImage<=0)
            bmpInfo.bmiHeader.biSizeImage = bmpInfo.bmiHeader.biWidth * abs(bmpInfo.bmiHeader.biHeight) * (bmpInfo.bmiHeader.biBitCount + 7) / 8;

        if ((pBuf = malloc(bmpInfo.bmiHeader.biSizeImage)) == NULL) {
            MessageBox(NULL, "Can not reserve memory for bitmap", "Splines : Error", MB_OK | MB_ICONERROR);
            break;
        }

        bmpInfo.bmiHeader.biCompression = BI_RGB;

        GetDIBits(hdc, hBitmap, 0, bmpInfo.bmiHeader.biHeight, pBuf, &bmpInfo, DIB_RGB_COLORS);

        if ((fp = fopen(pszFilename, "wb")) == NULL) {
            MessageBox(NULL, "Can not create bitmap file", "Splines : Error", MB_OK | MB_ICONERROR);
            break;
        }

        bmpFileHeader.bfReserved1 = 0;
        bmpFileHeader.bfReserved2 = 0;
        bmpFileHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + bmpInfo.bmiHeader.biSizeImage;
        bmpFileHeader.bfType = 0x4D42;
        bmpFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

        fwrite(&bmpFileHeader, sizeof(BITMAPFILEHEADER), 1, fp);
        fwrite(&bmpInfo.bmiHeader, sizeof(BITMAPINFOHEADER), 1, fp);
        fwrite(pBuf, bmpInfo.bmiHeader.biSizeImage, 1, fp);
    } while (false);

    if (hdc)
        ReleaseDC(NULL,hdc);

    if (pBuf)
        free(pBuf);

    if (fp)
        fclose(fp);
}

void saveBMPFile(HWND hWnd) {
    OPENFILENAME ofn;
    char szFile[MAX_PATH];

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "All\0*.*\0Bitmap\0*.bmp\0";
    ofn.nFilterIndex = 2;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;

    char szBuf[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, szBuf);

    ofn.lpstrInitialDir = szBuf;
    ofn.lpstrTitle = "Splines : Bitmap Save";
    ofn.Flags = OFN_CREATEPROMPT | OFN_OVERWRITEPROMPT;

    if (GetSaveFileName(&ofn) == TRUE) {
        bSaveBMP = true;
        strcpy(pszBMPPath, ofn.lpstrFile);
        InvalidateRect(hWnd, NULL, TRUE);
    }
}
