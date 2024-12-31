#include "stackmgr.h"
#include "qlabel.h"

StackMgr::StackMgr(QTabWidget *w, QObject *parent)
    : QObject{parent}, m_sm(w)
{
    m_labelList = new QList<QLabel *>();
}


void StackMgr::putPage(QString const& text, const QString&label){

    QLabel *l = new QLabel(text);
    l->setAlignment(Qt::AlignTop);
    m_sm->addTab(l, label);
    m_labelList->append(l);
}

void StackMgr::removedPage(const int & index){
    m_labelList->at(index)->deleteLater();
}



void StackMgr::changePage(QString const& text, const QString&label){
    m_sm->setTabText(m_sm->currentIndex(), label);
    m_labelList->at(m_sm->currentIndex())->setText(text);
}

