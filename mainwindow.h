#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "locationfetcher.h"
#include "weatherfetcher.h"
#include "stackmgr.h"
#include "qgeocoordinate.h"
#include "qgeopositioninfo.h"
#include <QMainWindow>
#include <QInputDialog>
#include <QDate>

#include "emissaires.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    static WeatherCoeffs *wc;


public slots:
    void givePoints(QGeoPositionInfo c);

    void putPage(WeatherResult n_wr);

    void reaffectWeatherCoeffs(int i);
    void reaffectWeatherCoeffs(double i);
    void reaffectWeatherCoeffs();

    void checkWeather(int i);

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();

    void on_tabWidget_tabCloseRequested(int index);

private:
    Ui::MainWindow *ui;
    LocationFetcher *fetcher;
    WeatherFetcher *wf;
    StackMgr *m_sm;
    QGeoCoordinate gc;
    Emissaires *emissaires;
    QList<WeatherResult> *wr;
};
#endif // MAINWINDOW_H
