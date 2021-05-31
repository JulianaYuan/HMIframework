#ifndef FMODEHOME_H
#define FMODEHOME_H

#include "hmifrm_modeclass.h"
#include <QDebug>
#include <QObject>
class Mode_Home:public Frm_AbstractModeClass
{
    Q_OBJECT
public:
    Mode_Home();
    ~Mode_Home();
    Mode_Home(
             QObject* Framework=0,
             QObject* parent = 0,
             quint16 modeid = 0,
             quint16 screenid = 0,
             quint16 modeattr = 1
             );

    static Frm_AbstractModeClass* constructer(
            QObject* Framework=0,
            QObject* parent = 0,
            quint16 modeid = 0,
            quint16 screenid = 0,
            quint16 modeattr = 1)
    {
        return new Mode_Home(Framework,parent,modeid,screenid,modeattr);
    }

    bool concreteEvent(QEvent *);
    int consumeEvent(QEvent *e);
    void concreteVisible();
    void concreteInvisible();
    void concreteStart();
    void concreteEnd();

private:
    bool hardKeyEventProc(QEvent *e);

};
#endif
