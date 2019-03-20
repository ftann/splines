#ifndef SPLINE_H
#define SPLINE_H

#include <stdio.h>
#include <windows.h>
#include <math.h>

#define SPLINE_ERROR (-1)

//Dateierweiterungen
#define SPLINE_IMAGE_FILE ".bmp"
#define SPLINE_POINT_FILE ".sptf"

//maximale Anzahl Punkte
#define SPLINE_MAX_POINTS 50
#define SPLINE_MAX_MATRIX 4 * SPLINE_MAX_POINTS
#define SPLINE_MAX_TEXTLEN 100

/*
Graph
*/

//Auflösung
//definiere standard Auflösung
#define SPLINE_GRAPH_RESOLUTION_X 800
#define SPLINE_GRAPH_RESOLUTION_Y 640

//Achsen
//Graph -> -SPLINE_GRAPH_AXES_STD/2 <= x <= SPLINE_GRAPH_AXES_STD/2
#define SPLINE_GRAPH_AXES_RATIO 10
#define SPLINE_GRAPH_AXES_STD 20
#define SPLINE_GRAPH_AXES_MIN 10
#define SPLINE_GRAPH_AXES_MAX 1000

//Linienbreite
#define SPLINE_WIDTH_AXES 1
#define SPLINE_WIDTH_PEN 3
#define SPLINE_WIDTH_POLYNOM 5
#define SPLINE_WIDTH_POINT 10
#define SPLINE_WIDTH_SINGLESPLINES 5
#define SPLINE_WIDTH_SPLINE 5

//Farben
#define SPLINE_COLOR_AXES() RGB(96, 96, 96)
#define SPLINE_COLOR_POINT() RGB(224, 224, 224)
#define SPLINE_COLOR_POINTSELECTED() RGB(73, 240, 93)
#define SPLINE_COLOR_POLYNOM() RGB(255, 0, 0)//RGB(10, 120, 220)
#define SPLINE_COLOR_SINGLESPLINES() RGB(192, 192, 192)
#define SPLINE_COLOR_SINGLESPLINES_GRADIENT 1
#define SPLINE_COLOR_SPLINE() RGB(0, 0, 0)
#define SPLINE_COLOR_TEXT() RGB(64, 64, 64)

//Gleitkommazahl (floating-point number = flpn)
typedef float flpn_t;
//Punkt-Struktur
typedef struct {
    flpn_t x;
    flpn_t y;
} point_t;
//Rechteck-Struktur

class clsSpline {
public:

    //Einstellungen für Zeichenobjekte & System
    bool m_bAutoscale;
    bool m_bAxes;
    bool m_bCoord;
    bool m_bLowEndSystem;
    bool m_bPoints;
    bool m_bPolynom;
    bool m_bSingleSplines;
    bool m_bSpline;

    flpn_t m_EquationMatrix[SPLINE_MAX_MATRIX][SPLINE_MAX_MATRIX];
    flpn_t m_PolynomMatrix[SPLINE_MAX_MATRIX][SPLINE_MAX_MATRIX];
    flpn_t m_xAxis;
    flpn_t m_yAxis;
    flpn_t m_Radius;
    //Faktor für die Normierung einer einzelnen Zeile
    flpn_t m_Q;

    //Linker Mausbutton gedrückt?
    int m_nClicked;
    //Anzahl Punkte
    int m_nPoints;
    //Anzahl Variablen = Anzahl Gleichungen
    int m_nMatrix;
    int m_nPolynom;
    //Ausgewählter Punkt
    int m_nSelectedPoint;
    //Swap-Sort
    int m_nSwapped;

    point_t m_ActPoint;
    //Speicher für Punkte
    point_t m_Points[SPLINE_MAX_POINTS];

    RECT m_Frame;

//public:
    //Gibt zu gegebenem x-Wert der Kurve cn den y-Wert zurück
    flpn_t calcPolynomValueAt(flpn_t);
    flpn_t calcSplineValueAt(flpn_t, int);

    //Schnittpunktberechnung
    int IntersectionPoint(point_t);

    //Skaliert y-Achse
    flpn_t scalexAxis(flpn_t);
    flpn_t scaleyAxis(flpn_t);

    //rechnet Fenster-Koordinaten in Graphenkoordinaten um und umgekehrt
    point_t cnvGraphCoord(point_t);
    flpn_t cnvGraphCoord_x(flpn_t);
    flpn_t cnvGraphCoord_y(flpn_t);

    POINT cnvWindowCoord(point_t);
    flpn_t cnvWindowCoord_x(flpn_t);
    flpn_t cnvWindowCoord_y(flpn_t);

    //Gleichungen lösen
    void calcEquations(void);
    void calcPolynom(void);
    //Zeichnet Graphen
    void draw(HDC hDC);
    //Zeichnet Koordinaten am unteren rechten Bildrand
    void drawAxes(HDC);
    void drawCoord(HWND, point_t*);
    void drawPoints(HDC);
    void drawPolynom(HDC);
    void drawSingleSplines(HDC);
    void drawSplines(HDC);

    //Konsolenausgabe der Gleichungen und der Gleichungsmatrix
    void printEquations(void);
    void printEquationMatrix(void);
    void printPolynomMatrix(void);

    //Klassenkonstruktor, -copy-konstruktor, -destruktor
    clsSpline();
    clsSpline(const clsSpline&);
    ~clsSpline();
};

#endif // SPLINE_H
