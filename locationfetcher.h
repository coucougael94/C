#ifndef LOCATIONFETCHER_H
#define LOCATIONFETCHER_H

#include <QObject>

#include <QCoreApplication>
#include <QGeoPositionInfoSource>
#include <QGeoPositionInfo>
#include <QGeoCoordinate>
#include <QDebug>
#include <QTimer>

class LocationFetcher : public QObject {
    Q_OBJECT

public:
    LocationFetcher(QObject *parent = nullptr) : QObject(parent) {
        // Récupérer la source de position (par exemple, GPS)
        source = QGeoPositionInfoSource::createDefaultSource(this);

        if (source) {
            connect(source, &QGeoPositionInfoSource::positionUpdated, this, &LocationFetcher::onPositionUpdated);
            source->startUpdates(); // Commencer à recevoir les mises à jour de localisation
            source->requestUpdate(3600*1000);//1heures -> milliseconds

        } else {
            qWarning() << "Impossible de récupérer la source de position.";
        }


    }

signals:
    void localisation(QGeoPositionInfo i);

private slots:
    void onPositionUpdated(const QGeoPositionInfo &info) {
        // Récupérer la position
        QGeoCoordinate coordinate = info.coordinate();

        qDebug() << "Latitude:" << coordinate.latitude();
        qDebug() << "Longitude:" << coordinate.longitude();
        qDebug() << "Altitude:" << coordinate.altitude();
        emit localisation(info);
    }

private:
    QGeoPositionInfoSource *source;
};


#endif // LOCATIONFETCHER_H



