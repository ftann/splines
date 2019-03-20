#include "spline.h"

extern clsSpline* Spline;

flpn_t clsSpline::scalexAxis(flpn_t yAxis) {
    return yAxis * ((m_bAutoscale) ? (static_cast<flpn_t>(m_Frame.right) / static_cast<flpn_t>(m_Frame.bottom)) : 1.0);
}

flpn_t clsSpline::scaleyAxis(flpn_t xAxis) {
    //return xAxis * ((Spline->m_bAutoscale) ? ((flpn_t)Spline->m_Frame.bottom  / (flpn_t)Spline->m_Frame.right) : 1.0);
    return xAxis * ((m_bAutoscale) ? (static_cast<flpn_t>(m_Frame.bottom)  / static_cast<flpn_t>(m_Frame.right)) : 1.0);
}
