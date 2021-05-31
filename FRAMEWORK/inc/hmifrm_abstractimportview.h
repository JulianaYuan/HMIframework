#ifndef Frm_AbstractImportView_H
#define Frm_AbstractImportView_H

//#include <QtDeclarative/QDeclarativeListProperty>
#include "hmifrm_viewclass.h"
#include <QString>


#define VARIABLEDATA_READ(variable)   ( (NULL != variableData) ? (variableData->variable) : (0) )
#define VARIABLEDATA_WRITE(variable,value)   ( (NULL != variableData) ? (variableData->variable = value) : (0))
#define VARIABLEARYDATA_READ(variable,classname)   ( (NULL != variableData) ? QDeclarativeListProperty<classname>( this, &variableData->variable, classname::appendFunction, \
                                            classname::countFunction, classname::atFunction, classname::clearFunction ) : QDeclarativeListProperty<classname>( this, NULL, 0, 0, 0, 0 ) )
#define VARIABLEARYDATA_COUNT(variable)   (VARIABLEDATA_READ(variable.count()))


class Frm_AbstractImportView : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int modeid READ modeID NOTIFY sigAbstractPropertyChange )


    private:    

    public:

        Frm_AbstractImportView(QObject* parent = 0);
        ~Frm_AbstractImportView();

        void setAccessClass(Frm_AbstractViewClass* viewP);

        virtual void setConcreteAccessClass() = 0;

        Q_INVOKABLE void reqTrans(quint16 screenid = 0);
        Q_INVOKABLE void reqReturn();
        Q_INVOKABLE void reqChange(quint16 screenid);
        //touch announce
        Q_INVOKABLE void touchButton(quint16 buttonid = 0);

        int modeID();
    signals:

        void sigAbstractPropertyChange();

    protected:

        Frm_AbstractViewClass* view;

};

#endif // Frm_AbstractImportView_H

