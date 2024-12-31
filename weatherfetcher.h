#ifndef WEATHERFETCHER_H
#define WEATHERFETCHER_H

#include <QObject>
#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QGeoCoordinate>
#include <QDebug>
#include <QTimer>
#include <QThread>
#include <QDateTime>
#include <QList>
#include <datadate.h>

struct WeatherResult{

    QString log;

    int deg;

    QList<DataDate*> dd;

    QString vent;
};


class WeatherFetcher : public QObject {
    Q_OBJECT

public:
    WeatherFetcher(QObject *parent = nullptr) : QObject(parent) {
        // Initialisation du gestionnaire réseau
        manager = new QNetworkAccessManager(this);
        rep=nullptr;
        QObject::connect(manager, &QNetworkAccessManager::finished, this, &WeatherFetcher::onReplyFinished);

    }

    // Fonction pour récupérer les informations météorologiques
    void fetchWeather(const QGeoCoordinate &coordinate) {
        QString apiKey = "e4615076e8177d7b2c001a683714dbb9"; // Remplacez avec votre clé API OpenWeather
        // URL de l'API OpenWeather v3 avec latitude et longitude
        QString url = QString("http://api.open-meteo.com/v1/forecast"
                              "?latitude=%1&longitude=%2"
                              "&current=wind_speed_10m,wind_direction_10m"
                              "&hourly=wind_speed_10m,wind_speed_80m,wind_speed_120m,wind_speed_180m,wind_direction_10m,wind_direction_80m,wind_direction_120m,wind_direction_180m"
                              "&timezone=Europe/Berlin"
                              )
                          .arg(coordinate.latitude())
                          .arg(coordinate.longitude());

        qDebug() << "URL de la requête:" << url;

        QNetworkRequest request((QUrl(url)));  // Créer la requête


        static bool firstTime = true;
        static QDateTime t = QDateTime::currentDateTime();

        if(firstTime || t.toSecsSinceEpoch() +5<QDateTime::currentSecsSinceEpoch()){//5Secs

            qDebug() << "GET effectué:" << url;
            rep = manager->get(request);  // Effectuer la requête GET
        }
        t=QDateTime::currentDateTime();
        firstTime = false;
    }

signals:
    void replyFinished(WeatherResult);


public slots:
    void getState(){
        onReplyFinished(rep);
    }
    void onReplyFinished(QNetworkReply *reply) {
        qDebug() << "Réponse terminée.";
        if (reply->error() != QNetworkReply::NoError) {
            qWarning() << "Erreur réseau:" << reply->errorString();
            reply->deleteLater();
            return;
        }

        WeatherResult wr;
        // Analyser la réponse JSON
        QByteArray response = reply->readAll();
        qDebug()<<response;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
        if (jsonDoc.isObject()) {
            QJsonObject jsonObj = jsonDoc.object();
            if (jsonObj.contains("hourly")) {
                QJsonObject hourly = jsonObj["hourly"].toObject();
                //double temp = main["temp"].toDouble();  // Température
                int deg = hourly["deg"].toInt();
                //qDebug() << "Widn.deg:" << deg<<wind;
                qDebug() << "Wind.deg:" << deg;

                for(int i = 0; i < 168 ;i++){
                    DataDate *d = new DataDate(hourly, i);
                    wr.dd.append(d);
                }
                wr.deg = deg;
                wr.vent = response;
                wr.log="";


                //{"coord":
                //  {"lon":2.3768,"lat":48.7831},
                //  "weather":[{"id":800,"main":"Clear","description":"clear sky","icon":"01d"}],
                //  "base":"stations",
                //  "main":{"temp":281.84,"feels_like":280.03,"temp_min":281.09,"temp_max":282.35,"pressure":1033,"humidity":78,"sea_level":1033,"grnd_level":1023},
                //  "visibility":10000,
                //  "wind":{"speed":3.09,"deg":40},"clouds":{"all":0},"dt":1735222888,
                //  "sys":{"type":1,"id":6548,"country":"FR","sunrise":1735198978,"sunset":1735228755},
                //  "timezone":3600,"id":6452043,"name":"Vitry-sur-Seine","cod":200}

            } else {
                qWarning() << "Données principales non disponibles.";
            }
        }
        emit replyFinished(wr);

        reply->deleteLater();
    }


private:
    QNetworkAccessManager *manager;
    QNetworkReply *rep;
};


#endif // WEATHERFETCHER_H
