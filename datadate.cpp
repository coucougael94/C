#include "datadate.h"
#include "mainwindow.h"



#include <QVector>
#include <QPointF>
#include <stdexcept>

class CubicSpline {
public:
    // Initialisation avec les données des points (x, y)
    CubicSpline() {

        m_points = {
            QPointF(0, 0.5),
            QPointF(500, 0.6),
            QPointF(1000, 0.7),
            QPointF(7000, 1),
            QPointF(10000, 0.86),
            QPointF(20000, 0.7),
            QPointF(50000, 0.6),
            QPointF(60000, 0.5),
            QPointF(80000, 0.4),
            QPointF(100000, 0.1)
        };
        computeSplineCoefficients();
    }

    // Méthode pour obtenir la valeur y pour un x donné
    double getValue(double x) {
        // Trouver l'intervalle dans lequel x tombe
        int i = findInterval(x);
        if (i == -1) {
            throw std::out_of_range("x is out of the range of the spline.");
        }

        // Calculer la valeur de y en x en utilisant les coefficients de la spline cubique
        double dx = x - m_points[i].x();
        return m_a[i] + m_b[i] * dx + m_c[i] * dx * dx + m_d[i] * dx * dx * dx;
    }

private:
    QVector<QPointF> m_points;  // Points d'entrée (x, y)
    QVector<double> m_a, m_b, m_c, m_d;  // Coefficients de la spline cubique

    // Calcul des coefficients de la spline cubique
    void computeSplineCoefficients() {
        int n = m_points.size();
        QVector<double> h(n - 1), alpha(n - 1), l(n), mu(n), z(n);

        // Calculer les h, alpha, l, mu, z
        for (int i = 0; i < n - 1; ++i) {
            h[i] = m_points[i + 1].x() - m_points[i].x();
            if (h[i] == 0) continue;  // Prevent division by zero
            alpha[i] = (3.0 / h[i]) * (m_points[i + 1].y() - m_points[i].y());
        }

        l[0] = 1.0;
        mu[0] = 0.0;
        z[0] = 0.0;

        for (int i = 1; i < n - 1; ++i) {
            l[i] = 2 * (m_points[i + 1].x() - m_points[i - 1].x()) - h[i - 1] * mu[i - 1];
            mu[i] = h[i] / l[i];
            z[i] = (alpha[i] - h[i - 1] * z[i - 1]) / l[i];
        }

        l[n - 1] = 1.0;
        z[n - 1] = 0.0;
        m_c.resize(n, 0.0);

        // Back substitution to find c
        for (int j = n - 2; j >= 0; --j) {
            m_c[j] = z[j] - mu[j] * m_c[j + 1];
            m_b.resize(n - 1);
            m_b[j] = (m_points[j + 1].y() - m_points[j].y()) / h[j] - h[j] * (m_c[j + 1] + 2 * m_c[j]) / 3.0;
            m_d.resize(n - 1);
            m_d[j] = (m_c[j + 1] - m_c[j]) / (3.0 * h[j]);
            m_a.push_back(m_points[j].y());
        }
    }

    int findInterval(double x) {
        int n = m_points.size();
        if (x < m_points[0].x() || x > m_points[n - 1].x()) {
            return -1;
        }

        int i = 0;
        for (i = 0; i < n - 1; ++i) {
            if (x >= m_points[i].x() && x <= m_points[i + 1].x()) {
                break;
            }
        }
        return i;
    }
};


DataDate::DataDate(QJsonObject data, int hour, QObject *parent)
    : QObject{parent}, m_data(data),m_hour(hour)
{


}




double middle(int azimuth, double m1, double m2){
    double m1trunc = (m1>m2)? m1-m2 : m1;
    double m2trunc = (m1>m2)? m2 : m2-m1;

    double coeffTurnOver= std::pow(2.718281828459045, 1-(m1trunc+m2trunc)/360)/2.718281828459045;//amplitude du scan (plus le scan est petit plus l'air est stagnant donc l'exposition est grande)

    double petit = (m1>m2)? m2 : m1;
    double grand = (m1>m2)? m1 : m2;

    double coeffInterieure = 0.2;
    if(petit < azimuth && grand > azimuth)
        coeffInterieure = 1;

    return coeffTurnOver * coeffInterieure;
}

double DataDate::windScanCoeff(int azimuth){
    if(m_hour>=167)
        return 0;

    double d1 = middle(azimuth, m_data["wind_direction_10m"][m_hour+1].toDouble() , m_data["wind_direction_10m"][m_hour].toDouble());
    double d2 = middle(azimuth, m_data["wind_direction_80m"][m_hour+1].toDouble() , m_data["wind_direction_80m"][m_hour].toDouble());
    double d3 = middle(azimuth, m_data["wind_direction_120m"][m_hour+1].toDouble() , m_data["wind_direction_120m"][m_hour].toDouble());
    double d4 = middle(azimuth, m_data["wind_direction_180m"][m_hour+1].toDouble() , m_data["wind_direction_180m"][m_hour].toDouble());

    return (d1+d2+d3+d4)/4;
}


report DataDate::genHourCoeff(QGeoCoordinate meteo, QGeoCoordinate cremato){
    report r;
    int azimuth = meteo.azimuthTo(cremato);
    double distance = cremato.distanceTo(meteo);

    std::array<double, 4> coeff_dist = {MainWindow::wc->c41,MainWindow::wc->c42,MainWindow::wc->c43,MainWindow::wc->c44};

    if(distance < MainWindow::wc->d1){//en metre
        coeff_dist={MainWindow::wc->c11,MainWindow::wc->c12,MainWindow::wc->c13,MainWindow::wc->c14};
    } else if(distance < MainWindow::wc->d2){
        coeff_dist={MainWindow::wc->c21,MainWindow::wc->c22,MainWindow::wc->c23,MainWindow::wc->c24};
    } else if(distance < MainWindow::wc->d3){
        coeff_dist={MainWindow::wc->c31,MainWindow::wc->c32,MainWindow::wc->c33,MainWindow::wc->c34};//Jamais 180m de haut par rapport au sol n'est représentatif.
    }

    CubicSpline spline;
    double spline_coeff = spline.getValue(distance);


    //recuperer la vitesse du vent
        std::array<double, 4> vitesses = {
            m_data["wind_speed_10m"][m_hour].toDouble()  *coeff_dist[0],
            m_data["wind_speed_80m"][m_hour].toDouble()  *coeff_dist[1],
            m_data["wind_speed_120m"][m_hour].toDouble() *coeff_dist[2],
            m_data["wind_speed_180m"][m_hour].toDouble() *coeff_dist[3]
            };
        double vitesse_coeff = (vitesses[0]+vitesses[1]+vitesses[2]+vitesses[3]) * 1000;

    //recuperer l'azimuth
        std::array<double, 4> directions = {
            m_data["wind_direction_10m"][m_hour].toDouble()  *coeff_dist[0],
            m_data["wind_direction_80m"][m_hour].toDouble()  *coeff_dist[1],
            m_data["wind_direction_120m"][m_hour].toDouble() *coeff_dist[2],
            m_data["wind_direction_180m"][m_hour].toDouble() *coeff_dist[3]
        };

        double azimuth_risque_coeff = (directions[0]+directions[1]+directions[2]+directions[3]);

        r.vitesse_risque = (distance / vitesse_coeff)*spline_coeff;
        r.azimuth_risque = (1-abs((azimuth-azimuth_risque_coeff)/360))*windScanCoeff(azimuth);

    //heures
        //19h(18h ferme) à 8h00(8h30 ouvre)
        if(m_hour%24 < 8 || m_hour%24 >19){
            r.vitesse_risque=0.09;
            r.azimuth_risque=0.09;
        }

    return r;
}

QString DataDate::reportHour(QGeoCoordinate meteo, QGeoCoordinate cremato, QString name){


    struct report r = genHourCoeff(meteo, cremato);

    QString pageStr = "";

    pageStr +="<font color='"
               +
               QString((r.azimuth_risque>0.3)? "red" : "green")
               +
               "'>• Azimuth</font> &nbsp;";
    pageStr +="<font color='"
               +
               QString((r.vitesse_risque>0.1)? "red" : "green")
               +
               "'>• Vitesse</font>";

    pageStr+=" "
               + name;


    pageStr +=
                " - "
               +
               QString::number(m_hour/24+1)+"j - "+QString::number((m_hour)%24)+ "h"
                +
               " - "
               +
               QString::number(cremato.azimuthTo(meteo))+"-"+
               QString::number(meteo.azimuthTo(cremato))
               +
               " - "
               +
               "vr="
               +
               QString::number(r.vitesse_risque)
               +"&ar="+
               QString::number(r.azimuth_risque)
               +
               "<br>";


    return pageStr;// environs 312 va pas
}
