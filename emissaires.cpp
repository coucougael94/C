#include "emissaires.h"

Emissaires::Emissaires(QObject *parent)
    : QObject{parent}
{
    im = new QStandardItemModel();
    QList<QGeoCoordinate> m_geoEmiss;
}

void Emissaires::initialise(){
    appendEmissaire(QGeoCoordinate(48.80339129490158, 2.343056635028882));
}

// Fonction pour convertir degrés, minutes, secondes (DMS) en degrés décimaux
double dmsToDecimal(int degrees, int minutes, double seconds, const QString &direction) {
    double decimal = degrees + minutes / 60.0 + seconds / 3600.0;
    if (direction == "S" || direction == "W") {
        decimal = -decimal; // Sud et Ouest sont négatifs
    }
    return decimal;
}

// Fonction pour convertir une chaîne en QGeoCoordinate
QGeoCoordinate Emissaires::convertToQGeoCoordinate(const QString &dmsString) {
    if(!dmsString.contains("°")){
        int i = dmsString.indexOf(',');
        return QGeoCoordinate(dmsString.left(i).toDouble(), dmsString.sliced(i).toDouble());
    }

    // Expression régulière pour capturer le format DMS
    QRegExp regex(R"S((\d+)°\s*(\d+)'?\s*([\d.]+)"?\s*([NSEW]))S");
    QStringList matches;
    double latitude = 0, longitude = 0;

    // Recherche de la latitude
    if (regex.indexIn(dmsString) != -1) {
        matches = regex.capturedTexts();
        int latDeg = matches[1].toInt();
        int latMin = matches[2].toInt();
        double latSec = matches[3].toDouble();
        QString latDir = matches[4];
        latitude = dmsToDecimal(latDeg, latMin, latSec, latDir);
    }

    // Recherche de la longitude (doit être après la première correspondance)
    if (regex.indexIn(dmsString, regex.pos() + 1) != -1) {
        matches = regex.capturedTexts();
        int lonDeg = matches[1].toInt();
        int lonMin = matches[2].toInt();
        double lonSec = matches[3].toDouble();
        QString lonDir = matches[4];
        longitude = dmsToDecimal(lonDeg, lonMin, lonSec, lonDir);
    }

    return QGeoCoordinate(latitude, longitude);
}


void Emissaires::appendEmissaire(const QString& geo, QString name){
    appendEmissaire(convertToQGeoCoordinate(geo), name);
}

void Emissaires::appendEmissaire(QGeoCoordinate geo ,QString name){
    im->appendRow(new QStandardItem(geo.toString()));
    m_geoEmiss.append(geo);
    m_nameEmiss.append(name);
}

QString Emissaires::report(QGeoCoordinate meteo, DataDate *dd){
    QString r="";
    auto it2=m_nameEmiss.begin();
    for(auto it = m_geoEmiss.begin(); it != m_geoEmiss.end();it++, it2++){
        r+= dd->reportHour(meteo, (*it), (*it2));
    }
    return r;
}

void Emissaires::setModel(QStandardItemModel *n_im){
    im = n_im;
}

QStandardItemModel* Emissaires::model(){
    return im;
}
