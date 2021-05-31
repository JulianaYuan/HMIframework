#ifndef CURSORCTRLCLASS_H
#define CURSORCTRLCLASS_H

#include <QObject>
#include <QTimerEvent>
#include <QEvent>

#define CURSORCTRL_LAYER_MAX        (10)
#define CURSORCTRL_MOVETYPE_PAGE    (0)
#define CURSORCTRL_MOVETYPE_TOTAL  (1)

typedef enum tagCursorStatusEnum
{
    EN_CURSORCTRL_STATUS_DISAPPEAR,
    EN_CURSORCTRL_STATUS_SHOW,
    EN_CURSORCTRL_STATUS_HIDE,

    EN_CURSORCTRL_STATUS_MAX
}CursorstatusEnum;


typedef enum tagOperateTypeEnum
{
    EN_CURSORCTRL_OPE_TYPE_RIGHT,
    EN_CURSORCTRL_OPE_TYPE_LEFT,
    EN_CURSORCTRL_OPE_TYPE_ENTER,
    EN_CURSORCTRL_OPE_TYPE_BACK,
    EN_CURSORCTRL_OPE_TYPE_CLEAR,
    EN_CURSORCTRL_OPE_TYPE_HIDE,

    EN_CURSORCTRL_OPE_TYPE_MAX
}OperateTypeEnum;

typedef enum tagActionTypeEnum
{
    EN_CURSORCTRL_ACT_NONE,
    EN_CURSORCTRL_ACT_INIT_DISAPPEAR,
    EN_CURSORCTRL_ACT_INIT_SHOW,
    EN_CURSORCTRL_ACT_SHOW,
    EN_CURSORCTRL_ACT_MOVEUP,
    EN_CURSORCTRL_ACT_MOVEDOWN,
    EN_CURSORCTRL_ACT_HIDE,
    EN_CURSORCTRL_ACT_HIDE_UPDATE,
    EN_CURSORCTRL_ACT_ENTER,
    EN_CURSORCTRL_ACT_BACK,
    EN_CURSORCTRL_ACT_ENTER_HIDE,
    EN_CURSORCTRL_ACT_BACK_HIDE,

    EN_CURSORCTRL_ACT_MAX
}ActionTypeEnum;

typedef enum tagCursorTypeEnum
{
    EN_CURSORCTRL_CURSOR_TYPE_NO_CURSOR,
    EN_CURSORCTRL_CURSOR_TYPE_RETURN_BTN,
    EN_CURSORCTRL_CURSOR_TYPE_TAB_BTN,
    EN_CURSORCTRL_CURSOR_TYPE_FUNC_BTN,
    EN_CURSORCTRL_CURSOR_TYPE_HOME_BTN,

    EN_CURSORCTRL_CURSOR_TYPE_MAX
}CursorTypeEnum;

typedef enum tagLayerTypeEnum
{
    EN_CURSORCTRL_LAYER_TYPE_NORMAL,
    EN_CURSORCTRL_LAYER_TYPE_SETTING_LIST,
    EN_CURSORCTRL_LAYER_TYPE_MEIDA_LIST,
    EN_CURSORCTRL_LAYER_TYPE_EQ_LIST,
    EN_CURSORCTRL_LAYER_TYPE_CHARGE_TIME,
    EN_CURSORCTRL_LAYER_TYPE_MAX
}LayerTypeEnum;

typedef struct tagLayerSet_t
{
    int i_startIndex;
    int i_cursorIndex_abs;
    LayerTypeEnum en_layerType;
}Layerset_t;

typedef struct tagPageDate_t
{
    CursorTypeEnum en_initstatus;
    int icursorPos;
    int i_returnBtnExist;
    int i_itemPerPage; //item count of one page. if page is not a list. it should set count of function button

    int ilayerCount; // how many layer of list, default is 1
    int i_currentLayer; // the layer which is display, 0 is the first layer
    Layerset_t a_stLayerset[CURSORCTRL_LAYER_MAX]; // detail information of every layer
}PageDate_t;

class CursorFeature
{
public:
    virtual void moveCursorTo( int i_cursorIndex) = 0;
    virtual void hideCursorFrom() = 0;
    virtual void TabBtnFoucsed(CursorTypeEnum en_type, int i_pos) = 0;
    virtual void TabBtnUnfoucsed(CursorTypeEnum en_type) = 0;
    virtual int get_totalNum() = 0;
    virtual int get_totalNumOfTabButton() = 0;
    virtual int get_curStartIndex() = 0;
    virtual int get_curStartIndexOfTabBtn(){return 0;}
    virtual int get_btn_sta_by_index(CursorTypeEnum en_type,int index)
    {
        qDebug("en_type = %d, index = %d",en_type, index);
        return 0;
    }
};

class CursorCtrlClass: public QObject
{
    Q_OBJECT
public:
    explicit CursorCtrlClass(CursorFeature* p_CursorFeature);
    ~CursorCtrlClass();

    void updateListCursr(OperateTypeEnum enOpeType);
    void initCursor(PageDate_t * p_pagedate);
    int getCurosrPostion_Rel();
    int getCurosrPostion_Abs();
    CursorTypeEnum getCurosrType();

    void KnobPressed();
    void KnobReleased();
    void StopTime();
    void ResetTimer();

private:
    CursorFeature* p_CursorFuntion;

    Layerset_t str_LayerSet[CURSORCTRL_LAYER_MAX];
    int m_iTimer_id;
    CursorstatusEnum m_enCursorStatus;
    CursorTypeEnum   m_enCusorType;
    PageDate_t m_pageDate;
    int m_currentLayer;

    int m_iCursorPos_rel;
    int m_iCursorPos_abs;
    int m_iTabCurSorPos_rel;

    void MoveCursor2FuncTop(int type);
    void MoveCursor2FuncBottom(int type);
    void MoveCursor2TabTop();
    void MoveCursor2TabBottom();

protected:
    int act_initDisappear();
    int act_initShow();
    int act_show();
    int act_moveUp();
    int act_moveDown();
    int act_hide();
    int act_hideUpdate();
    int act_enter();
    int act_back();
    int act_enterHide();
    int act_backHide();

    void timerStart();
    void timerStop();
    void timerEvent(QTimerEvent *p_event);
};

#endif // CURSORCTRLCLASS_H
