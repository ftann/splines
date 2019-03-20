#include "spline.h"

void clsSpline::printEquations(void) {
    printf("\n\nEquations\n");

    int i;
    for (i = 0; i < m_nPoints; i++)
        printf("From (%.1f/%.1f) to (%.1f/%.1f): %.2f * x^3 + %.2f * x^2 + %.2f * x + %.2f\n",
               m_Points[i].x,
               m_Points[i].y,
               m_Points[i + 1].x,
               m_Points[i + 1].y,
               m_EquationMatrix[4 * i + 0][m_nMatrix],
               m_EquationMatrix[4 * i + 1][m_nMatrix],
               m_EquationMatrix[4 * i + 2][m_nMatrix],
               m_EquationMatrix[4 * i + 3][m_nMatrix]);

}

void clsSpline::printEquationMatrix(void) {
    printf("\n\nEquation matrix\n");

    int x;
    int y;
    for (x = 0; x < m_nMatrix; x++) {
        for (y = 0; y <= m_nMatrix; y++)
            printf("%.2f ", m_EquationMatrix[x][y]);

        printf("\n");
    }
}

void clsSpline::printPolynomMatrix(void) {
    printf("\n\nPolynom matrix\n");

    int x;
    int y;
    for (x = 0; x < m_nPolynom; x++) {
        for (y = 0; y <= m_nPolynom; y++)
            printf("%.2f ", m_PolynomMatrix[x][y]);

        printf("\n");
    }
}
