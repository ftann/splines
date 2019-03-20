#include "spline.h"

//p = Punkt
int clsSpline::IntersectionPoint(point_t p) {
    int cntPoints;
    for (cntPoints = 0; cntPoints <= m_nPoints; cntPoints++) {
        if (pow(cnvWindowCoord_x(p.x) - cnvWindowCoord_x(m_Points[cntPoints].x), 2) + pow(cnvWindowCoord_y(p.y) - cnvWindowCoord_y(m_Points[cntPoints].y), 2) <= pow(m_Radius, 2))
            return cntPoints;
    }
    return SPLINE_ERROR;
}
