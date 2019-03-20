#include "spline.h"

//cn = Nummer der Kurve
//a*x^3+b*x^2+c*x+d
flpn_t clsSpline::calcSplineValueAt(flpn_t x, int nCurve) {
    return  1.0 * (flpn_t)pow(x, 3.0) * m_EquationMatrix[4 * nCurve + 0][m_nMatrix]
            + (flpn_t)pow(x, 2.0) * m_EquationMatrix[4 * nCurve + 1][m_nMatrix]
            + x * m_EquationMatrix[4 * nCurve + 2][m_nMatrix]
            + m_EquationMatrix[4 * nCurve + 3][m_nMatrix];
}

void clsSpline::calcEquations(void) {
    //swap-sort -> bringt Punkte in x-Richtung aufsteigende Ordnung

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

    //Matrix initialisieren

    m_nMatrix = 4 * m_nPoints;

    int cntMatrix;
    int cntPoint;
    int cntX;
    int cntY;

    for (cntX = 0; cntX < m_nMatrix; cntX++)
        for (cntY = 0; cntY <= m_nMatrix; cntY++)
            m_EquationMatrix[cntX][cntY] = 0;

    cntMatrix = SPLINE_ERROR;
    cntMatrix++;

    m_EquationMatrix[cntMatrix][0] = 6.0 * m_Points[0].x;
    m_EquationMatrix[cntMatrix][1] = 2.0;

    for (cntPoint = 0; cntPoint < m_nPoints; cntPoint++) {
        if (cntPoint < m_nPoints - 1) {
            //übereinstimmung in der zweiten Ableitung
            cntMatrix++;
            m_EquationMatrix[cntMatrix][4 * cntPoint] = 6.0 * m_Points[cntPoint + 1].x;
            m_EquationMatrix[cntMatrix][4 * cntPoint + 1] = 2.0;
            m_EquationMatrix[cntMatrix][4 * cntPoint + 4] = -6.0 * m_Points[cntPoint + 1].x;
            m_EquationMatrix[cntMatrix][4 * cntPoint + 5] = -2.0;
            m_EquationMatrix[cntMatrix][m_nMatrix] = 0;
            //übereinstimmung in der zweiten Ableitung
            cntMatrix++;
            m_EquationMatrix[cntMatrix][4 * cntPoint] = 3.0 * pow(m_Points[cntPoint + 1].x, 2);
            m_EquationMatrix[cntMatrix][4 * cntPoint + 1] = 2.0 * m_Points[cntPoint + 1].x;
            m_EquationMatrix[cntMatrix][4 * cntPoint + 2] = 1.0;
            m_EquationMatrix[cntMatrix][4 * cntPoint + 4] = -3.0 * pow(m_Points[cntPoint + 1].x, 2);
            m_EquationMatrix[cntMatrix][4 * cntPoint + 5] = -2.0 * m_Points[cntPoint + 1].x;
            m_EquationMatrix[cntMatrix][4 * cntPoint + 6] = -1.0;
            m_EquationMatrix[cntMatrix][m_nMatrix] = 0;
        }
        //übereinstimmung im ersten Punkt
        cntMatrix++;
        m_EquationMatrix[cntMatrix][4 * cntPoint] = pow(m_Points[cntPoint].x, 3);
        m_EquationMatrix[cntMatrix][4 * cntPoint + 1] = pow(m_Points[cntPoint].x, 2);
        m_EquationMatrix[cntMatrix][4 * cntPoint + 2] = m_Points[cntPoint].x;
        m_EquationMatrix[cntMatrix][4 * cntPoint + 3] = 1;
        m_EquationMatrix[cntMatrix][m_nMatrix] = m_Points[cntPoint].y;
        //zweiter Punkt
        cntMatrix++;
        m_EquationMatrix[cntMatrix][4 * cntPoint] = pow(m_Points[cntPoint + 1].x, 3);
        m_EquationMatrix[cntMatrix][4 * cntPoint + 1] = pow(m_Points[cntPoint + 1].x, 2);
        m_EquationMatrix[cntMatrix][4 * cntPoint + 2] = m_Points[cntPoint + 1].x;
        m_EquationMatrix[cntMatrix][4 * cntPoint + 3] = 1;
        m_EquationMatrix[cntMatrix][m_nMatrix] = m_Points[cntPoint + 1].y;
    }

    cntMatrix++;
    cntPoint--;

    m_EquationMatrix[cntMatrix][4 * cntPoint] = 6.0 * m_Points[cntPoint + 1].x;
    m_EquationMatrix[cntMatrix][4 * cntPoint + 1] = 2.0;

    //das eigentliche Auflösen
    for (cntX = 0; cntX < m_nMatrix; cntX++) { //für alle Zeilen
        //normieren der aktuellen Zeile
        m_Q = m_EquationMatrix[cntX][cntX];

        //verhindern, dass Division durch Null mittels Addition einer der folgenden Gleichungen, die an der entsprechenden Stelle keine Null enthalten
        if (m_Q == 0) {
            cntMatrix = cntX;

            while (m_EquationMatrix[cntMatrix][cntX] == 0 && cntMatrix < m_nMatrix) cntMatrix++; //solange bis ungleich null

            for (cntY = 0; cntY <= m_nMatrix; cntY++)
                m_EquationMatrix[cntX][cntY] += m_EquationMatrix[cntMatrix][cntY];

            m_Q = m_EquationMatrix[cntX][cntX]; //Teilungsfaktor gefunden
        }
        //eigentliches normieren
        for (cntY = 0; cntY <= m_nMatrix; cntY++) //alles durch Q dividieren
            if (cntY != cntX)
                m_EquationMatrix[cntX][cntY] /= m_Q;

        m_EquationMatrix[cntX][cntX] = 1.0;

        //abziehen des Vielfachen davon bei allen anderen Gleichungen
        for (cntMatrix = 0; cntMatrix < m_nMatrix; cntMatrix++) {
            m_Q = m_EquationMatrix[cntMatrix][cntX];

            if (cntMatrix != cntX)
                for (cntY = 0; cntY <= m_nMatrix; cntY++)
                    m_EquationMatrix[cntMatrix][cntY] -= m_Q * m_EquationMatrix[cntX][cntY];

        }
    }
}
