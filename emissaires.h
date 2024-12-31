#ifndef EMISSAIRES_H
#define EMISSAIRES_H

#include "datadate.h"
#include "qgeocoordinate.h"
#include <QObject>
#include <QWidget>
#include <QRegExp>
#include <QStringList>
#include <QStandardItemModel>

class Emissaires : public QObject
{
    Q_OBJECT
public:
    explicit Emissaires(QObject *parent = nullptr);



public slots:

    void initialise();
    void appendEmissaire(QGeoCoordinate geo, QString name="");
    void appendEmissaire(const QString & geo, QString name="");
    void setModel(QStandardItemModel *n_im);
    QGeoCoordinate convertToQGeoCoordinate(const QString &dmsString);

    QString report(QGeoCoordinate meteo, DataDate *dd);
    QStandardItemModel* model();

signals:

private:
    QStandardItemModel *im;

    QList<QGeoCoordinate> m_geoEmiss;
    QList<QString> m_nameEmiss;
};

#endif // EMISSAIRES_H
