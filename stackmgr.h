#ifndef STACKMGR_H
#define STACKMGR_H

#include <QObject>
#include <QWidget>
#include <QTabWidget>
#include <QList>
#include<QLabel>

class StackMgr : public QObject
{
    Q_OBJECT
public:
    explicit StackMgr(QTabWidget *w, QObject *parent = nullptr);

public slots:
    void putPage(QString const&text, const QString &label);
    void changePage(QString const& text, const QString&label);
    void removedPage(const int & index);


signals:

private:
    QTabWidget *m_sm;
    QList<QLabel *> *m_labelList;
};

#endif // STACKMGR_H
