#include "mainwindow.h"
#include "locationfetcher.h"
#include "ui_mainwindow.h"
#include <QGeoCoordinate>

#include "stackmgr.h"


#include <QCoreApplication>
#ifdef Q_OS_ANDROID
#include <QtCore/private/qandroidextras_p.h>
//#include <QAndroidJniObject>
#include<QJniObject>
#endif
#include <QDebug>



WeatherCoeffs* MainWindow::wc= new WeatherCoeffs();


void requestPermission(const QString &permission);

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
    gc(48.78307484657831, 2.376607631936812)//home
{
    ui->setupUi(this);
    ui->label->setText("ENETER");


    // Request location-related permissions
    requestPermission("android.permission.ACCESS_FINE_LOCATION");
    requestPermission("android.permission.ACCESS_COARSE_LOCATION");
    fetcher = new LocationFetcher(this);
    QObject::connect(fetcher, SIGNAL(localisation(QGeoPositionInfo)), this, SLOT(givePoints(QGeoPositionInfo)));

    m_sm = new StackMgr(ui->tabWidget, this);



    emissaires = new Emissaires(this);

    wf = new WeatherFetcher(this);
    QObject::connect(wf, SIGNAL(replyFinished(WeatherResult)), this, SLOT(putPage(WeatherResult)));

    reaffectWeatherCoeffs();
    QObject::connect(ui->spinBox, SIGNAL(valueChanged(int)), this, SLOT(reaffectWeatherCoeffs(int)));
    QObject::connect(ui->spinBox_2, SIGNAL(valueChanged(int)), this, SLOT(reaffectWeatherCoeffs(int)));
    QObject::connect(ui->spinBox_3, SIGNAL(valueChanged(int)), this, SLOT(reaffectWeatherCoeffs(int)));

    QObject::connect(ui->doubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(reaffectWeatherCoeffs(double)));
    QObject::connect(ui->doubleSpinBox_2, SIGNAL(valueChanged(double)), this, SLOT(reaffectWeatherCoeffs(double)));
    QObject::connect(ui->doubleSpinBox_3, SIGNAL(valueChanged(double)), this, SLOT(reaffectWeatherCoeffs(double)));
    QObject::connect(ui->doubleSpinBox_4, SIGNAL(valueChanged(double)), this, SLOT(reaffectWeatherCoeffs(double)));
    QObject::connect(ui->doubleSpinBox_5, SIGNAL(valueChanged(double)), this, SLOT(reaffectWeatherCoeffs(double)));
    QObject::connect(ui->doubleSpinBox_6, SIGNAL(valueChanged(double)), this, SLOT(reaffectWeatherCoeffs(double)));
    QObject::connect(ui->doubleSpinBox_7, SIGNAL(valueChanged(double)), this, SLOT(reaffectWeatherCoeffs(double)));
    QObject::connect(ui->doubleSpinBox_8, SIGNAL(valueChanged(double)), this, SLOT(reaffectWeatherCoeffs(double)));
    QObject::connect(ui->doubleSpinBox_9, SIGNAL(valueChanged(double)), this, SLOT(reaffectWeatherCoeffs(double)));
    QObject::connect(ui->doubleSpinBox_10, SIGNAL(valueChanged(double)), this, SLOT(reaffectWeatherCoeffs(double)));
    QObject::connect(ui->doubleSpinBox_11, SIGNAL(valueChanged(double)), this, SLOT(reaffectWeatherCoeffs(double)));
    QObject::connect(ui->doubleSpinBox_12, SIGNAL(valueChanged(double)), this, SLOT(reaffectWeatherCoeffs(double)));
    QObject::connect(ui->doubleSpinBox_13, SIGNAL(valueChanged(double)), this, SLOT(reaffectWeatherCoeffs(double)));
    QObject::connect(ui->doubleSpinBox_14, SIGNAL(valueChanged(double)), this, SLOT(reaffectWeatherCoeffs(double)));
    QObject::connect(ui->doubleSpinBox_15, SIGNAL(valueChanged(double)), this, SLOT(reaffectWeatherCoeffs(double)));
    QObject::connect(ui->doubleSpinBox_16, SIGNAL(valueChanged(double)), this, SLOT(reaffectWeatherCoeffs(double)));


    for(int i = 0 ; i<168 ;i++){
        ui->comboBox->addItem(QString::number(i) + " - " + QString::number(i/24) + "j - " + QString::number(i%24) +  "h" );
    }

    emissaires->appendEmissaire(QGeoCoordinate(48.80339129490158, 2.343056635028882), "Cachan");
    emissaires->appendEmissaire(QGeoCoordinate(48.57453,2.23768), "Arpajon");
    emissaires->appendEmissaire(QGeoCoordinate(48.67471,2.20431), "Ulis");
    emissaires->appendEmissaire(QGeoCoordinate(48.78754,2.24696), "Clamart");
    emissaires->appendEmissaire(QGeoCoordinate(48.87493,2.20898), "Mont-Valérien");
    emissaires->appendEmissaire(QGeoCoordinate(48.86223,2.39624), "Pere-Lachaise");
    emissaires->appendEmissaire(QGeoCoordinate(48.73503,2.47331), "Valenton");
    emissaires->appendEmissaire(QGeoCoordinate(48.61896,2.39644), "Bondoufle");
    emissaires->appendEmissaire(QGeoCoordinate(48.81305,2.53486), "Champigny/marne");
    emissaires->appendEmissaire(QGeoCoordinate(48.95247,2.35251), "Joncherolles");
    emissaires->appendEmissaire(QGeoCoordinate(48.89877,2.57354), "Montfermeil");
    emissaires->appendEmissaire(QGeoCoordinate(48.95298,2.18957), "Bois rochefort");

    emissaires->appendEmissaire(QGeoCoordinate(49.24517,1.42871), "Les Andelys");

    wr = new QList<WeatherResult>();
    QObject::connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(checkWeather(int)));
    ui->comboBox->setCurrentIndex( (QDate::currentDate().dayOfWeek()-1)*24 + QDateTime::currentDateTime().toString("HH").toInt());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::givePoints(QGeoPositionInfo c){
    ui->label->setText(c.coordinate().toString()+"<br>\n"+ui->label->text());
}

void MainWindow::reaffectWeatherCoeffs(){
    ui->listView->setModel(emissaires->model());
    wc->d1 = ui->spinBox->value();wc->d2 = ui->spinBox_2->value();wc->d3 = ui->spinBox_3->value();

    wc->c11 = ui->doubleSpinBox->value();wc->c12 = ui->doubleSpinBox_2->value();wc->c13 = ui->doubleSpinBox_3->value();wc->c14 = ui->doubleSpinBox_4->value();wc->c21 = ui->doubleSpinBox_5->value();wc->c22 = ui->doubleSpinBox_6->value();wc->c23 = ui->doubleSpinBox_7->value();
    wc->c24 = ui->doubleSpinBox_8->value();wc->c31 = ui->doubleSpinBox_9->value();wc->c32 = ui->doubleSpinBox_10->value();wc->c33 = ui->doubleSpinBox_11->value();wc->c34 = ui->doubleSpinBox_12->value();wc->c41 = ui->doubleSpinBox_13->value();wc->c42 = ui->doubleSpinBox_14->value();wc->c43 = ui->doubleSpinBox_15->value();wc->c44 = ui->doubleSpinBox_16->value();
}
void MainWindow::reaffectWeatherCoeffs(double i){
    reaffectWeatherCoeffs();Q_UNUSED(i);
}
void MainWindow::reaffectWeatherCoeffs(int i){
    reaffectWeatherCoeffs();Q_UNUSED(i);
}

void requestPermission(const QString &permission) {
#ifdef Q_OS_ANDROID
    // Create a Java string object for the permission
    QJniObject javaPermission = QJniObject::fromString(permission);

    if (!javaPermission.isValid()) {
        qWarning() << "Invalid permission string:" << permission;
        return;
    }

    // Get the main Android activity
    QJniObject activity = QtAndroidPrivate::activity();

    if (!activity.isValid()) {
        qWarning() << "Unable to access Android activity.";
        return;
    }

    // Create a JNI array for the permission
    QJniEnvironment env;
    jobjectArray permissionsArray = env->NewObjectArray(
        1, // Array size
        env->FindClass("java/lang/String"), // Array type
        javaPermission.object<jstring>()); // Initial value

    if (!permissionsArray) {
        qWarning() << "Failed to create JNI permissions array.";
        return;
    }

    // Call ActivityCompat.requestPermissions to request the permission
    QJniObject::callStaticMethod<void>(
        "androidx/core/app/ActivityCompat",
        "requestPermissions",
        "(Landroid/app/Activity;[Ljava/lang/String;I)V",
        activity.object(),
        permissionsArray,
        0); // Request code (arbitrary, can use to handle results later)

    // Clean up local references
    env->DeleteLocalRef(permissionsArray);
#else
    qDebug() << "Permission requests are only valid on Android.";
#endif
}




void MainWindow::on_pushButton_clicked()
{

    qDebug() << "GGTEST1:";
    QGeoPositionInfoSource *source = QGeoPositionInfoSource::createDefaultSource(this);
    if (source) {
        source->setPreferredPositioningMethods(QGeoPositionInfoSource::AllPositioningMethods);
        connect(source, &QGeoPositionInfoSource::positionUpdated, this, [this](const QGeoPositionInfo &info) {
            qDebug() << "Position1 updated:" << info.coordinate();

            QGeoCoordinate cremato(48.80339129490158, 2.343056635028882);//cremato
            qDebug() << "azi"<<info.coordinate().azimuthTo(cremato);

            gc = info.coordinate();
            this->ui->label->setText(info.coordinate().toString());

        });
        source->startUpdates();
    } else {
        qWarning() << "No default source available for positioning. Default : Home = IAK";
#ifndef Q_OS_ANDROID

        ui->label->setText(gc.toString());


        QGeoCoordinate cremato(48.80339129490158, 2.343056635028882);
        qDebug() << "azi"<<cremato.azimuthTo(gc);
        qDebug() << "azi2"<<gc.azimuthTo(cremato);
#endif
    }

}

void MainWindow::on_pushButton_2_clicked()
{// "Consulter vent"
    wf->fetchWeather(gc);
}

void MainWindow::putPage(WeatherResult n_wr){
    QString label ="";
    label += QDateTime::currentDateTime().toString("HH:mm");
    label +=" - ";
    label += ui->label->text();

    int h = ui->comboBox->currentText().toInt();

    QString pageStr="";
    pageStr+=emissaires->report(gc, n_wr.dd.at(h));

    ui->label_2->setText(n_wr.log);
    m_sm->putPage(pageStr, label);// Ajouter en titre l'adresse coordonnée & Heure
    wr->append(n_wr);
}
void MainWindow::checkWeather(int i){
    if(ui->tabWidget->count()==0)
        return;
    i--;
    QString hh = QString::number(i/24);
    QString label = ui->tabWidget->tabText(ui->tabWidget->currentIndex());
    qDebug()<< ui->tabWidget->count() << label ;
    label = label.sliced(1);
    label.prepend(hh);
    if(label[1] == ":")
        label.prepend("0");
    ui->tabWidget->setTabText(ui->tabWidget->currentIndex(), label);
    int h = ui->comboBox->currentIndex();

    QString pageStr="";
    pageStr+=emissaires->report(gc, wr->at(ui->tabWidget->currentIndex()).dd.at(h));

    m_sm->changePage(pageStr, label);// Ajouter en titre l'adresse coordonnée & Heure
}

void MainWindow::on_pushButton_3_clicked()
{
    bool ok = false;
    QString text = QInputDialog::getText(this,
                        "Ajouter un émissaire",
                        "Coordonnées de l'émissaire"
                        " (genre 48.858844,2.294351,35)",
                        QLineEdit::Normal,
                        QString(),
                        &ok);
    QString name = QInputDialog::getText(this,
                                         "Ajouter un émissaire",
                                         "Nom de l'émissaire"
                                         " (genre Ulis)",
                                         QLineEdit::Normal,
                                         QString(),
                                         &ok);

    if(ok){
        emissaires->appendEmissaire(text , name);
    }
}


void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
    ui->tabWidget->removeTab(index);
    m_sm->removedPage(index);

}

