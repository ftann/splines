#ifndef LANGUAGE_H_INCLUDED
#define LANGUAGE_H_INCLUDED

#include <windows.h>

#define LNG_ENG 9
#define LNG_ENG_AU 3081
#define LNG_ENG_BZ 10249
#define LNG_ENG_CA 4105
#define LNG_ENG_IE 6153
#define LNG_ENG_JM 8201
#define LNG_ENG_NZ 5129
#define LNG_ENG_SA 7177
#define LNG_ENG_TT 11273
#define LNG_ENG_UK 2057
#define LNG_ENG_US 1033

#define LNG_ENG_TTLERR "Splines : Error"
#define LNG_ENG_TTLFOPEN "Splines : Open File"
#define LNG_ENG_TTLFSAVE "Splines : Save File"
#define LNG_ENG_TTLFSAVEBMP "Splines : Save Bitmap"
#define LNG_ENG_TTLHELP "Splines : Help"
#define LNG_ENG_TTLVER "Splines : Version"
#define LNG_ENG_TXTERRBMPF "Can not create bitmap file"
#define LNG_ENG_TXTERRBMPMEM "Can not reserve memory for bitmap"
#define LNG_ENG_TXTERRFOPEN "Can not open file"
#define LNG_ENG_TXTERRFSAVE "Can not save file"
#define LNG_ENG_TXTERRPT "Maximal number of points reached"
#define LNG_ENG_TXTHELP "Help is not implemented yet.\n\nPleas contact florian.tanner@gmail.com"
#define LNG_ENG_FLTRSPTF "All\0*.*\0Splines Point File\0*.sptf\0"
#define LNG_ENG_FLTRBMP "All\0*.*\0Bitmap\0*.bmp\0"

#define LNG_DE 1031
#define LNG_DE_AT 3079
#define LNG_DE_LI 5127
#define LNG_DE_LU 4103
#define LNG_DE_CH 2055

#define LNG_DE_TTLERR "Splines : Fehler"
#define LNG_DE_TTLFOPEN "Splines : Datei öffnen"
#define LNG_DE_TTLFSAVE "Splines : Datei speichern"
#define LNG_DE_TTLFSAVEBMP "Splines : Bitmap speichern"
#define LNG_DE_TTLHELP "Splines : Hilfe"
#define LNG_DE_TTLVER "Splines : Version"
#define LNG_DE_TXTERRBMPF "Kann Bitmap-Datei nicht erstellen"
#define LNG_DE_TXTERRBMPMEM "Kann Speicher für Bitmap nicht reservieren"
#define LNG_DE_TXTERRFOPEN "Kann Datei nicht öffnen"
#define LNG_DE_TXTERRFSAVE "Kann Datei nicht speichern"
#define LNG_DE_TXTERRPT "Maximale Anzahl Punkte erreicht"
#define LNG_DE_TXTHELP "Help is not implemented yet.\n\nPleas contact florian.tanner@gmail.com"
#define LNG_DE_FLTRSPTF "Alle\0*.*\0Splines Punkt Datei\0*.sptf\0"
#define LNG_DE_FLTRBMP "Alle\0*.*\0Bitmap\0*.bmp\0"

int getSystemLanguage(void);
void setProgramLanguage(int);
void setStringSet(int);

typedef struct mlstrings {
    int lcid;
    struct title{
        char error[128];
        char fileopen[128];
        char filesave[128];
        char bmpsave[128];
        char help[128];
        char version[128];
    } title;
    struct text {
        char errorbitmapfile[128];
        char errorbitmapmemory[128];
        char errorfileopen[128];
        char errorfilesave[128];
        char errorpoint[128];
        char help[128];
    } text;
    struct filter {
        char sptf[128];
        char bmp[128];
    } filter;
} mlstrings, *pmlstrings;

#endif // LANGUAGE_H_INCLUDED
