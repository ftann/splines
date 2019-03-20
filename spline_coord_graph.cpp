#include "spline.h"

point_t clsSpline::cnvGraphCoord(point_t p) {
    point_t ret;
    ret.x = cnvGraphCoord_x(p.x);
    ret.y = cnvGraphCoord_y(p.y);
    return ret;
}

flpn_t clsSpline::cnvGraphCoord_x(flpn_t f) {
    return m_xAxis * f / static_cast<flpn_t>(m_Frame.right) - m_xAxis / 2;
}

flpn_t clsSpline::cnvGraphCoord_y(flpn_t f) {
    return -1.0 * ((m_bAutoscale) ? (static_cast<flpn_t>(m_Frame.bottom)  / static_cast<flpn_t>(m_Frame.right)) : 1.0) * (static_cast<flpn_t>(f) * m_xAxis / static_cast<flpn_t>(m_Frame.bottom) - m_xAxis / 2);
}
