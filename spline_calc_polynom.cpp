#include "spline.h"

flpn_t clsSpline::calcPolynomValueAt(flpn_t x) {
    flpn_t result = 0;

    int cntPolynom;
    for (cntPolynom = 0; cntPolynom <= m_nPoints; cntPolynom++)
        result += m_PolynomMatrix[cntPolynom][m_nPoints + 1] * pow(x, static_cast<flpn_t>(cntPolynom));

    return result;
}

void clsSpline::calcPolynom(void) {
    m_nSwapped = 1;

    while (m_nSwapped != 0) {
        m_nSwapped = 0;

        for (int i = 0; i < m_nPoints; i++) {
            if (m_Points[i].x > m_Points[i + 1].x) {
                m_ActPoint.x = m_Points[i].x;
                m_ActPoint.y = m_Points[i].y;
                m_Points[i].x = m_Points[i + 1].x;
                m_Points[i].y = m_Points[i + 1].y;
                m_Points[i + 1].x = m_ActPoint.x;
                m_Points[i + 1].y = m_ActPoint.y;

                m_nSwapped++;

                if (m_nSelectedPoint == i)
                    m_nSelectedPoint++;
                else
                    m_nSelectedPoint--;
            }
        }
    }

    m_nPolynom = m_nPoints + 1;

    int cntX;
    int cntY;
    int cntZ;

    for (cntX = 0; cntX <= m_nPolynom; cntX++) {
        for (cntY = 0; cntY <= m_nPoints; cntY++)
            m_PolynomMatrix[cntX][cntY] = pow(m_Points[cntX].x, static_cast<flpn_t>(cntY));

        m_PolynomMatrix[cntX][m_nPoints + 1] = m_Points[cntX].y;
    }
    //eigentliches Auflösen

    for (cntX = 0; cntX < m_nPolynom; cntX++) {
        //normieren der aktuellen Zeile
        m_Q = m_PolynomMatrix[cntX][cntX];

        //verhindern, dass Division durch Null mittels Addition einer der folgenden Gleichungen, die an der entsprechenden Stelle keine Null enthalten
        if (m_Q == 0) {
            cntZ = cntX;

            while (m_PolynomMatrix[cntZ][cntX] == 0 && cntZ < m_nPolynom)
                cntZ++;

            for (cntY = 0; cntY <= m_nPolynom; cntY++)
                m_PolynomMatrix[cntX][cntY] += m_PolynomMatrix[cntZ][cntY];

            //Teilungsfaktor gefunden
            m_Q = m_PolynomMatrix[cntX][cntX];
        }

        //eigentliches normieren
        for (cntY = 0; cntY <= m_nPolynom; cntY++) //alles durch m_Q dividieren
            if (cntY != cntX)
                m_PolynomMatrix[cntX][cntY] /= m_Q;

        m_PolynomMatrix[cntX][cntX] = 1.0;

        //abziehen des Vielfachen davon bei allen anderen Gleichungen
        for (cntZ = 0; cntZ < m_nPolynom; cntZ++) {
            m_Q = m_PolynomMatrix[cntZ][cntX];

            if (cntZ != cntX)
                for (cntY = 0; cntY <= m_nPolynom; cntY++)
                    m_PolynomMatrix[cntZ][cntY] -= m_Q * m_PolynomMatrix[cntX][cntY];
        }
    }
}
