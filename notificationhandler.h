#ifndef NOTIFICATIONHANDLER_H
#define NOTIFICATIONHANDLER_H

#include <QObject>

class NotificationHandler : public QObject
{
    Q_OBJECT
public:
    explicit NotificationHandler(QObject *parent = nullptr);

    Q_INVOKABLE void sendNotification(const QString &title, const QString &message);

};

#endif // NOTIFICATIONHANDLER_H
