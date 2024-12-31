#include "NotificationHandler.h"
#ifdef Q_OS_ANDROID
#include <QtCore/private/qandroidextras_p.h>
//#include <QAndroidJniObject>
#include<QJniObject>

#endif

#include <QDebug>
NotificationHandler::NotificationHandler(QObject *parent) : QObject(parent) {}

void NotificationHandler::sendNotification(const QString &title, const QString &message)
{

#ifdef Q_OS_ANDROID
    QJniObject javaTitle = QJniObject::fromString(title);
    QJniObject javaMessage = QJniObject::fromString(message);

    QJniObject::callStaticMethod<void>(
        "com/example/notifications/NotificationHelper",  // Chemin de la classe Java
        "sendNotification",                             // Méthode Java
        "(Ljava/lang/String;Ljava/lang/String;)V",      // Signature JNI
        javaTitle.object<jstring>(),
        javaMessage.object<jstring>()
        );

#endif

#ifndef Q_OS_ANDROID
    qDebug()<<"Notif=" << title << "-" <<message;
#endif
}
