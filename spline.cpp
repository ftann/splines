#include "spline.h"

clsSpline::clsSpline(void):
        m_bAutoscale(true),
        m_bAxes(true),
        m_bCoord(true),
        m_bLowEndSystem(false),
        m_bPoints(true),
        m_bPolynom(false),
        m_bSingleSplines(false),
        m_bSpline(true),
        m_Radius(SPLINE_WIDTH_POINT),
        m_Q(0),
        m_nPoints(SPLINE_ERROR),
        m_nMatrix(0),
        m_nPolynom(0),
        m_nSelectedPoint(SPLINE_ERROR),
        m_nSwapped(0) {
    m_Frame.top = 0;
    m_Frame.left = 0;
    m_Frame.bottom = SPLINE_GRAPH_RESOLUTION_Y;
    m_Frame.right = SPLINE_GRAPH_RESOLUTION_X;

    m_xAxis = SPLINE_GRAPH_AXES_STD;
    m_yAxis = scaleyAxis(SPLINE_GRAPH_AXES_STD);
}

clsSpline::clsSpline(const clsSpline& s):
        m_bAutoscale(s.m_bAutoscale),
        m_bAxes(s.m_bAxes),
        m_bCoord(s.m_bCoord),
        m_bLowEndSystem(s.m_bLowEndSystem),
        m_bPoints(s.m_bPoints),
        m_bPolynom(s.m_bPolynom),
        m_bSingleSplines(s.m_bSingleSplines),
        m_bSpline(s.m_bSpline),
        m_Radius(s.m_Radius),
        m_Q(s.m_Q),
        m_nPoints(s.m_nPoints),
        m_nMatrix(s.m_nMatrix),
        m_nPolynom(s.m_nPolynom),
        m_nSelectedPoint(s.m_nSelectedPoint),
        m_nSwapped(s.m_nSwapped) {
    for (int i = 0; i <= s.m_nPoints; i++)
        m_Points[i] = s.m_Points[i];

    m_Frame.top = s.m_Frame.top;
    m_Frame.left = s.m_Frame.left;
    m_Frame.bottom = s.m_Frame.bottom;
    m_Frame.right = s.m_Frame.right;

    m_xAxis = s.m_xAxis;
    m_yAxis = s.m_yAxis;
}

clsSpline::~clsSpline() {
}
