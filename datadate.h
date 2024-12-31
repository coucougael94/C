#ifndef DATADATE_H
#define DATADATE_H

#include <QObject>
#include <QJsonObject>
#include <QGeoCoordinate>

class MainWindow;

struct WeatherCoeffs{
    int d1, d2, d3;
    double c11,c12,c13,c14;
    double c21,c22,c23,c24;
    double c31,c32,c33,c34;
    double c41,c42,c43,c44;
};


struct report{
    double azimuth_risque;
    double vitesse_risque;
};

class DataDate : public QObject
{
    Q_OBJECT
public:
    explicit DataDate(QJsonObject data, int hour, QObject *parent = nullptr);

    report genHourCoeff(QGeoCoordinate meteo, QGeoCoordinate cremato);
    QString reportHour(QGeoCoordinate meteo, QGeoCoordinate cremato, QString name="");

    double windScanCoeff(int azimuth);

private:
    QJsonObject const m_data;
    int m_hour;

signals:
};

#endif // DATADATE_H
