#include "cursorctrlclass.h"
#include "hmi_log.h"
#include <memory.h>
#include "../../FRAMEWORK/inc/hmifrm_common.h"

typedef struct TagstatusChange_t
{
    ActionTypeEnum en_action;
    CursorstatusEnum en_status;

}statusChange_t;

statusChange_t ast_actionSet[EN_CURSORCTRL_STATUS_MAX][EN_CURSORCTRL_OPE_TYPE_MAX] =
{
    //EN_CURSORCTRL_STATUS_DISAPPEAR,
    {
        {EN_CURSORCTRL_ACT_INIT_SHOW,       EN_CURSORCTRL_STATUS_SHOW},         //EN_CURSORCTRL_OPE_TYPE_RIGHT,
        {EN_CURSORCTRL_ACT_INIT_SHOW,       EN_CURSORCTRL_STATUS_SHOW},         //EN_CURSORCTRL_OPE_TYPE_LEFT,
        {EN_CURSORCTRL_ACT_ENTER_HIDE,      EN_CURSORCTRL_STATUS_DISAPPEAR},    //EN_CURSORCTRL_OPE_TYPE_ENTER,
        {EN_CURSORCTRL_ACT_BACK_HIDE,       EN_CURSORCTRL_STATUS_DISAPPEAR},    //EN_CURSORCTRL_OPE_TYPE_BACK,
        {EN_CURSORCTRL_ACT_NONE,            EN_CURSORCTRL_STATUS_DISAPPEAR},    //EN_CURSORCTRL_OPE_TYPE_CLEAR,
        {EN_CURSORCTRL_ACT_NONE,            EN_CURSORCTRL_STATUS_DISAPPEAR},    //EN_CURSORCTRL_OPE_TYPE_HIDE,
    },
    //EN_CURSORCTRL_STATUS_SHOW,
    {
        {EN_CURSORCTRL_ACT_MOVEDOWN,        EN_CURSORCTRL_STATUS_SHOW},         //EN_CURSORCTRL_OPE_TYPE_RIGHT,
        {EN_CURSORCTRL_ACT_MOVEUP,          EN_CURSORCTRL_STATUS_SHOW},         //EN_CURSORCTRL_OPE_TYPE_LEFT,
        {EN_CURSORCTRL_ACT_ENTER,           EN_CURSORCTRL_STATUS_SHOW},         //EN_CURSORCTRL_OPE_TYPE_ENTER,
        {EN_CURSORCTRL_ACT_BACK,            EN_CURSORCTRL_STATUS_SHOW},         //EN_CURSORCTRL_OPE_TYPE_BACK,
        {EN_CURSORCTRL_ACT_INIT_DISAPPEAR,  EN_CURSORCTRL_STATUS_DISAPPEAR},    //EN_CURSORCTRL_OPE_TYPE_CLEAR,
        {EN_CURSORCTRL_ACT_HIDE,            EN_CURSORCTRL_STATUS_HIDE},         //EN_CURSORCTRL_OPE_TYPE_HIDE,
    },
    //EN_CURSORCTRL_STATUS_HIDE,
    {
        {EN_CURSORCTRL_ACT_SHOW,            EN_CURSORCTRL_STATUS_SHOW},         //EN_CURSORCTRL_OPE_TYPE_RIGHT,
        {EN_CURSORCTRL_ACT_SHOW,            EN_CURSORCTRL_STATUS_SHOW},         //EN_CURSORCTRL_OPE_TYPE_LEFT,
        {EN_CURSORCTRL_ACT_ENTER_HIDE,      EN_CURSORCTRL_STATUS_HIDE},         //EN_CURSORCTRL_OPE_TYPE_ENTER,
        {EN_CURSORCTRL_ACT_BACK_HIDE,       EN_CURSORCTRL_STATUS_HIDE},         //EN_CURSORCTRL_OPE_TYPE_BACK,
        {EN_CURSORCTRL_ACT_INIT_DISAPPEAR,  EN_CURSORCTRL_STATUS_DISAPPEAR},    //EN_CURSORCTRL_OPE_TYPE_CLEAR,
        {EN_CURSORCTRL_ACT_NONE,            EN_CURSORCTRL_STATUS_HIDE},         //EN_CURSORCTRL_OPE_TYPE_HIDE,
    }
};

CursorCtrlClass::CursorCtrlClass(CursorFeature *p_CursorFeature)
{
    p_CursorFuntion = p_CursorFeature;

    memset(str_LayerSet, 0, sizeof(Layerset_t));
    memset(&m_pageDate, 0, sizeof(PageDate_t));
    m_pageDate.icursorPos = -1;

    m_iTimer_id = 0;
    m_enCursorStatus = EN_CURSORCTRL_STATUS_DISAPPEAR;
    m_currentLayer = 0;
}

CursorCtrlClass::~CursorCtrlClass()
{
    timerStop();
}

void CursorCtrlClass::initCursor(PageDate_t * p_pagedate)
{
    int i_isTimerOn = 0;
    int i_tagetTotal = 0;
    int i_startIdx = 0;

    if (NULL == p_CursorFuntion)
    {
        HMI_ERROR("NULL == p_CursorFuntion");
        return;
    }

    memcpy (&m_pageDate, p_pagedate, sizeof(PageDate_t));

    m_pageDate.i_returnBtnExist = 0; // general v0.14 changed, Home and return do not cursor

    if (CURSORCTRL_LAYER_MAX < m_pageDate.ilayerCount)
    {
        m_pageDate.ilayerCount = CURSORCTRL_LAYER_MAX;
    }
    if (0 > m_pageDate.ilayerCount )
    {
        m_pageDate.ilayerCount = 1;
    }
    if ((0 > m_pageDate.i_currentLayer )|| (CURSORCTRL_LAYER_MAX < m_pageDate.i_currentLayer))
    {
        m_pageDate.i_currentLayer = 0;
    }

    Frm_CommonClassIns->getCursorStatus(&m_enCursorStatus);
    HMI_LOG("m_enCursorStatus = %d",m_enCursorStatus);
    if (m_enCursorStatus == EN_CURSORCTRL_STATUS_HIDE)
    {
        m_enCursorStatus = EN_CURSORCTRL_STATUS_DISAPPEAR;
        m_pageDate.en_initstatus = EN_CURSORCTRL_CURSOR_TYPE_NO_CURSOR;
    }

    m_currentLayer = m_pageDate.i_currentLayer;
    m_enCusorType = m_pageDate.en_initstatus;
    m_iCursorPos_abs = 0;
    m_iCursorPos_rel = 0;
    m_iTabCurSorPos_rel = 0;

    switch (m_pageDate.en_initstatus)
    {
    case EN_CURSORCTRL_CURSOR_TYPE_NO_CURSOR:
        break;
    case EN_CURSORCTRL_CURSOR_TYPE_RETURN_BTN:
        if (1 == m_pageDate.i_returnBtnExist)
        {
            m_enCusorType = EN_CURSORCTRL_CURSOR_TYPE_RETURN_BTN;
            m_pageDate.icursorPos = 0;
            i_isTimerOn = 1;
            p_CursorFuntion->TabBtnFoucsed(m_enCusorType, 0);
        }
        else
        {
            HMI_LOG("para error!! i_returnBtnExist = 0, en_initstatus = EN_CURSORCTRL_CURSOR_TYPE_RETURN_BTN");
        }
        break;
    case EN_CURSORCTRL_CURSOR_TYPE_TAB_BTN:

        i_tagetTotal = p_CursorFuntion->get_totalNumOfTabButton();

        if (0 < i_tagetTotal)
        {
            if(m_pageDate.icursorPos >= i_tagetTotal)
            {
                m_pageDate.icursorPos = i_tagetTotal -1;
            }
            m_enCusorType =EN_CURSORCTRL_CURSOR_TYPE_TAB_BTN;
            i_isTimerOn = 1;
            p_CursorFuntion->TabBtnFoucsed(m_enCusorType, m_pageDate.icursorPos);
            m_iCursorPos_abs = m_pageDate.icursorPos;
            m_iCursorPos_rel = m_pageDate.icursorPos;
            m_iTabCurSorPos_rel = m_pageDate.icursorPos - i_startIdx;
        }
        else
        {
            MoveCursor2FuncTop(CURSORCTRL_MOVETYPE_TOTAL);
            i_isTimerOn = 1;
            HMI_LOG("para error!! Tab button count = 0, en_initstatus = EN_CURSORCTRL_CURSOR_TYPE_TAB_BTN");
        }
        break;
    case EN_CURSORCTRL_CURSOR_TYPE_FUNC_BTN:
        HMI_LOG("EN_CURSORCTRL_CURSOR_TYPE_FUNC_BTN");
        i_tagetTotal = p_CursorFuntion->get_totalNum();
        i_startIdx = p_CursorFuntion->get_curStartIndex();

        if (0 < i_tagetTotal)
        {
            if(m_pageDate.icursorPos >= i_tagetTotal)
            {
                m_pageDate.icursorPos = i_tagetTotal -1;
            }

            m_enCusorType = EN_CURSORCTRL_CURSOR_TYPE_FUNC_BTN;
            m_iCursorPos_rel = m_pageDate.icursorPos - i_startIdx;
            m_iCursorPos_abs = m_pageDate.icursorPos;

            p_CursorFuntion->moveCursorTo(m_pageDate.icursorPos);
            i_isTimerOn = 1;
            m_pageDate.a_stLayerset[m_currentLayer].i_startIndex = i_startIdx;
            m_pageDate.a_stLayerset[m_currentLayer].i_cursorIndex_abs = m_iCursorPos_abs;
        }
        else
        {
            HMI_LOG("para error!! Func button count = 0, en_initstatus = EN_CURSORCTRL_CURSOR_TYPE_FUNC_BTN");
        }
        break;
    case EN_CURSORCTRL_CURSOR_TYPE_HOME_BTN:
        if (1 == m_pageDate.i_returnBtnExist)
        {
            m_enCusorType = EN_CURSORCTRL_CURSOR_TYPE_HOME_BTN;
            m_pageDate.icursorPos = 0;
            i_isTimerOn = 1;
            p_CursorFuntion->TabBtnFoucsed(m_enCusorType, 0);
        }
        else
        {
            HMI_LOG("para error!! i_returnBtnExist = 0, en_initstatus = EN_CURSORCTRL_CURSOR_TYPE_HOME_BTN");
        }
        break;
    default:
        break;
    }

    if (1 == i_isTimerOn)
    {
        m_enCursorStatus = EN_CURSORCTRL_STATUS_SHOW;
        timerStart();
    }
    else
    {
        m_enCursorStatus = EN_CURSORCTRL_STATUS_DISAPPEAR;
        m_pageDate.icursorPos = -1; // cursor disaapear
    }

    HMI_LOG("333 Frm_CommonClassIns->setCursorStatus(%d);", m_enCursorStatus);
    Frm_CommonClassIns->setCursorStatus(m_enCursorStatus);
    Frm_CommonClassIns->setCursorType(m_enCusorType, m_iCursorPos_abs);

    return;
}

void CursorCtrlClass::updateListCursr(OperateTypeEnum enOpeType)
{
    statusChange_t *p_state = NULL;
    int iret = 0;

    HMI_LOG("enOpeType = %d", enOpeType);
    HMI_LOG("m_enCursorStatus = %d", m_enCursorStatus);

    p_state = &ast_actionSet[m_enCursorStatus][enOpeType];
    HMI_LOG("p_state->en_action = %d",p_state->en_action);
    switch(p_state->en_action)
    {
    case EN_CURSORCTRL_ACT_INIT_DISAPPEAR:
        iret = act_initDisappear();
        break;
    case EN_CURSORCTRL_ACT_INIT_SHOW:
        iret = act_initShow();
        break;
    case  EN_CURSORCTRL_ACT_SHOW:
        iret = act_show();
        break;
    case  EN_CURSORCTRL_ACT_MOVEUP:
        iret = act_moveUp();
        break;
    case  EN_CURSORCTRL_ACT_MOVEDOWN:
        iret = act_moveDown();
        break;
    case  EN_CURSORCTRL_ACT_HIDE:
        iret = act_hide();
        break;
    case  EN_CURSORCTRL_ACT_HIDE_UPDATE:
        iret = act_hideUpdate();
        break;
    case  EN_CURSORCTRL_ACT_ENTER:
        iret = act_enter();
        break;
    case  EN_CURSORCTRL_ACT_BACK:
        iret = act_back();
        break;
    case  EN_CURSORCTRL_ACT_ENTER_HIDE:
        iret = act_enterHide();
        break;
    case  EN_CURSORCTRL_ACT_BACK_HIDE:
        iret = act_backHide();
        break;
    default:
        break;
    }

    if (0 == iret)
    {
        m_enCursorStatus =  p_state->en_status;
    }

    if((EN_CURSORCTRL_OPE_TYPE_HIDE == enOpeType) && (EN_CURSORCTRL_STATUS_DISAPPEAR == m_enCursorStatus))
    {
        m_enCusorType = EN_CURSORCTRL_CURSOR_TYPE_NO_CURSOR;
        //when nocursor screen press back to last screen,shoud hide the cursor
        HMI_LOG("m_enCusorType = EN_CURSORCTRL_CURSOR_TYPE_NO_CURSOR");
    }

    HMI_LOG("1111Frm_CommonClassIns->setCursorStatus(%d);", m_enCursorStatus);

    Frm_CommonClassIns->setCursorStatus(m_enCursorStatus);
    Frm_CommonClassIns->setCursorType(m_enCusorType, m_iCursorPos_abs);

    return;
}
int CursorCtrlClass::getCurosrPostion_Rel()
{
    HMI_LOG("m_iCursorPos_rel = %d",m_iCursorPos_rel);
    return m_iCursorPos_rel;
}

int CursorCtrlClass::getCurosrPostion_Abs()
{
    HMI_LOG("m_iCursorPos_abs = %d", m_iCursorPos_abs);
    return m_iCursorPos_abs;
}

CursorTypeEnum CursorCtrlClass::getCurosrType()
{
	if (EN_CURSORCTRL_STATUS_SHOW != m_enCursorStatus)
    {
        return EN_CURSORCTRL_CURSOR_TYPE_NO_CURSOR;
    }
    return m_enCusorType;
}

void CursorCtrlClass::timerStart()
{
    HMI_LOG("IN");
    if (0 != m_iTimer_id)
    {
        //delete timer
       killTimer(m_iTimer_id);
       m_iTimer_id = 0;
    }

    //start new timer
     m_iTimer_id = startTimer(10000);

    return;
}

void CursorCtrlClass::timerStop()
{
  HMI_LOG("in");
    if (0 != m_iTimer_id)
    {
        //delete timer
        killTimer(m_iTimer_id);
        m_iTimer_id = 0;
    }

    return;
}

void CursorCtrlClass::timerEvent( QTimerEvent *p_event )
{

    if (p_event->timerId() == m_iTimer_id )
    {
        act_hide();
        m_enCursorStatus = EN_CURSORCTRL_STATUS_HIDE;
        HMI_LOG("2222 Frm_CommonClassIns->setCursorStatus(%d);", m_enCursorStatus);
        Frm_CommonClassIns->setCursorStatus(m_enCursorStatus);
    }
}

int CursorCtrlClass::act_initDisappear()
{
    int iret = 0;

    HMI_LOG("m_enCusorType = %d ",m_enCusorType);
    if ( NULL == p_CursorFuntion )
    {
        HMI_LOG("NULL == p_CursorFuntion");
        return 0;
    }

    switch (m_enCusorType)
    {
    case EN_CURSORCTRL_CURSOR_TYPE_NO_CURSOR:
        break;
    case EN_CURSORCTRL_CURSOR_TYPE_RETURN_BTN:
    case EN_CURSORCTRL_CURSOR_TYPE_HOME_BTN:
        p_CursorFuntion->TabBtnUnfoucsed(m_enCusorType);
        break;
    case EN_CURSORCTRL_CURSOR_TYPE_TAB_BTN:
        p_CursorFuntion->TabBtnUnfoucsed(m_enCusorType);
        break;
    case EN_CURSORCTRL_CURSOR_TYPE_FUNC_BTN:
        p_CursorFuntion->hideCursorFrom();
        break;
    default:
        break;
    }
    m_enCusorType = EN_CURSORCTRL_CURSOR_TYPE_NO_CURSOR;
    m_pageDate.icursorPos = -1;
    m_iCursorPos_abs = -1;
    m_iCursorPos_rel = -1;
    timerStop();

    return iret;
}
int CursorCtrlClass::act_initShow()
{
    if ( NULL == p_CursorFuntion )
    {
        HMI_LOG("NULL == p_CursorFuntion");
        return 0;
    }

    int iret = 0;
    int i_tab_btn_total = 0;
    int i_func_btn_total = 0;

    m_pageDate.icursorPos = 0;
    m_iCursorPos_abs = 0;
    m_iCursorPos_rel = 0;

    HMI_LOG("m_enCusorType = %d ",m_enCusorType);
    i_tab_btn_total = p_CursorFuntion->get_totalNumOfTabButton();
    if (0 >= i_tab_btn_total)
    {
        i_func_btn_total = p_CursorFuntion->get_totalNum();
        if (0 >= i_func_btn_total)
        {
            if(1 == m_pageDate.i_returnBtnExist)
            {
                p_CursorFuntion->TabBtnFoucsed(EN_CURSORCTRL_CURSOR_TYPE_HOME_BTN, 0);
                m_enCusorType = EN_CURSORCTRL_CURSOR_TYPE_HOME_BTN;
            }
            else
            {
                HMI_LOG("Error");
                return 0;
            }
        }
        else
        {
            m_enCusorType = EN_CURSORCTRL_CURSOR_TYPE_FUNC_BTN;

            m_pageDate.a_stLayerset[m_currentLayer].i_startIndex = 0;
            m_pageDate.a_stLayerset[m_currentLayer].i_cursorIndex_abs = 0;

            if (0 == p_CursorFuntion->get_btn_sta_by_index(EN_CURSORCTRL_CURSOR_TYPE_FUNC_BTN,0))
            {
                p_CursorFuntion->moveCursorTo(0);
            }
            else
            {
                act_moveDown();
            }
        }
    }
    else
    {
        m_enCusorType = EN_CURSORCTRL_CURSOR_TYPE_TAB_BTN;

        if (0 == p_CursorFuntion->get_btn_sta_by_index(EN_CURSORCTRL_CURSOR_TYPE_TAB_BTN,0))
        {
            p_CursorFuntion->TabBtnFoucsed(EN_CURSORCTRL_CURSOR_TYPE_TAB_BTN, 0);
        }
        else
        {
            act_moveDown();
        }

    }
    timerStart();

    return iret;
}

int CursorCtrlClass::act_moveUp()
{
    if ( NULL == p_CursorFuntion )
    {
        HMI_LOG("NULL == p_CursorFuntion");
        return 0;
    }
    int iret = 0;
    int i_startIdx = 0;
    int i_tab_btn_total = 0;
    int i_func_btn_total = 0;
    int i_resetTimer = 1;

    HMI_LOG("m_enCusorType = %d ",m_enCusorType);

    switch (m_enCusorType)
    {
    case EN_CURSORCTRL_CURSOR_TYPE_NO_CURSOR:
        i_resetTimer = 0;
        break;
    case EN_CURSORCTRL_CURSOR_TYPE_RETURN_BTN:
        p_CursorFuntion->TabBtnUnfoucsed(EN_CURSORCTRL_CURSOR_TYPE_RETURN_BTN);
        p_CursorFuntion->TabBtnFoucsed(EN_CURSORCTRL_CURSOR_TYPE_HOME_BTN, 0);
        m_enCusorType = EN_CURSORCTRL_CURSOR_TYPE_HOME_BTN;
        break;
    case EN_CURSORCTRL_CURSOR_TYPE_HOME_BTN:
        //cursor is on Home button, move cursor to 5th place of current page
        p_CursorFuntion->TabBtnUnfoucsed(EN_CURSORCTRL_CURSOR_TYPE_HOME_BTN);

        i_func_btn_total = p_CursorFuntion->get_totalNum();
        if (0 >= i_func_btn_total)
        {
            //there is not any function button, cursor move to tab button
            i_tab_btn_total = p_CursorFuntion->get_totalNumOfTabButton();
             if (0 >= i_tab_btn_total)
             {
                 //there is not any tab button, cursor move to return button
                 p_CursorFuntion->TabBtnFoucsed(EN_CURSORCTRL_CURSOR_TYPE_RETURN_BTN, 0);
                 m_enCusorType = EN_CURSORCTRL_CURSOR_TYPE_RETURN_BTN;
             }
             else
             {
                 //cursor move to buttom of Tab button
                 m_pageDate.icursorPos = i_tab_btn_total - 1;
                 m_iCursorPos_abs = m_pageDate.icursorPos;
                 m_iCursorPos_rel = m_pageDate.i_itemPerPage;

                 m_enCusorType = EN_CURSORCTRL_CURSOR_TYPE_TAB_BTN;

                 if (0 == p_CursorFuntion->get_btn_sta_by_index(EN_CURSORCTRL_CURSOR_TYPE_TAB_BTN,m_pageDate.icursorPos))
                 {
                     p_CursorFuntion->TabBtnFoucsed(EN_CURSORCTRL_CURSOR_TYPE_TAB_BTN, m_pageDate.icursorPos);
                 }
                 else
                 {
                     act_moveUp();
                 }
             }
        }
        else
        {
            MoveCursor2FuncBottom(CURSORCTRL_MOVETYPE_PAGE);
        }
        break;
    case EN_CURSORCTRL_CURSOR_TYPE_TAB_BTN:
        // cursor is first button of tab button,move cursor to return
        if(0 == m_pageDate.icursorPos)
        {
            p_CursorFuntion->TabBtnUnfoucsed(EN_CURSORCTRL_CURSOR_TYPE_TAB_BTN);
            if(1 == m_pageDate.i_returnBtnExist)
            {
                p_CursorFuntion->TabBtnFoucsed(EN_CURSORCTRL_CURSOR_TYPE_RETURN_BTN, 0);
                m_enCusorType = EN_CURSORCTRL_CURSOR_TYPE_RETURN_BTN;
            }
            else
            {
                // there is not return button,move to function button
                i_func_btn_total = p_CursorFuntion->get_totalNum();
                if (0 >= i_func_btn_total)
                {
                    //there is not any function button, cursor move to tab button
                    i_tab_btn_total = p_CursorFuntion->get_totalNumOfTabButton();

                    m_pageDate.icursorPos = i_tab_btn_total - 1;
                    m_iCursorPos_abs = m_pageDate.icursorPos;
                    m_iCursorPos_rel = m_pageDate.i_itemPerPage;

                    m_enCusorType = EN_CURSORCTRL_CURSOR_TYPE_TAB_BTN;

                    if (0 == p_CursorFuntion->get_btn_sta_by_index(EN_CURSORCTRL_CURSOR_TYPE_TAB_BTN,m_pageDate.icursorPos))
                    {
                        p_CursorFuntion->TabBtnFoucsed(EN_CURSORCTRL_CURSOR_TYPE_TAB_BTN, m_pageDate.icursorPos);
                    }
                    else
                    {
                        act_moveUp();
                    }

                }
                else
                {
                    MoveCursor2FuncBottom(CURSORCTRL_MOVETYPE_PAGE);
                }
            }
        }
        else
        {
            // move one up of tab button
            p_CursorFuntion->TabBtnUnfoucsed(EN_CURSORCTRL_CURSOR_TYPE_TAB_BTN);
            m_pageDate.icursorPos -= 1;
            m_iCursorPos_abs = m_pageDate.icursorPos;
            m_iCursorPos_rel = m_pageDate.icursorPos;
            if ( 0 < m_iTabCurSorPos_rel )
            {
                m_iTabCurSorPos_rel -= 1;
            }
            if (0 == p_CursorFuntion->get_btn_sta_by_index(EN_CURSORCTRL_CURSOR_TYPE_TAB_BTN,m_pageDate.icursorPos))
            {
                p_CursorFuntion->TabBtnFoucsed(EN_CURSORCTRL_CURSOR_TYPE_TAB_BTN, m_pageDate.icursorPos);
            }
            else
            {
                act_moveUp();
            }
        }
        break;
    case EN_CURSORCTRL_CURSOR_TYPE_FUNC_BTN:
        if (0 == m_pageDate.icursorPos )
        {
            //cursor is at the top of list,move cursor to bottom of tab button
            //p_CursorFuntion->hideCursorFrom();

            if (EN_CURSORCTRL_LAYER_TYPE_SETTING_LIST == m_pageDate.a_stLayerset[m_currentLayer].en_layerType)
            {
                //MoveCursor2FuncBottom(CURSORCTRL_MOVETYPE_TOTAL);
                break;
            }
            else if(EN_CURSORCTRL_LAYER_TYPE_EQ_LIST == m_pageDate.a_stLayerset[m_currentLayer].en_layerType)
            {
                MoveCursor2FuncBottom(CURSORCTRL_MOVETYPE_TOTAL);
                break;
            }
            else if(EN_CURSORCTRL_LAYER_TYPE_CHARGE_TIME == m_pageDate.a_stLayerset[m_currentLayer].en_layerType)
            {
                MoveCursor2FuncBottom(CURSORCTRL_MOVETYPE_TOTAL);
                break;
            }
            i_tab_btn_total = p_CursorFuntion->get_totalNumOfTabButton();
            if (0 >= i_tab_btn_total)
            {
                //there is not any tab button, cursor move to return button
                if (1 == m_pageDate.i_returnBtnExist)
                {
                    p_CursorFuntion->hideCursorFrom();
                    p_CursorFuntion->TabBtnFoucsed(EN_CURSORCTRL_CURSOR_TYPE_RETURN_BTN, 0);
                    m_enCusorType = EN_CURSORCTRL_CURSOR_TYPE_RETURN_BTN;
                }
                else
                {
                    //MoveCursor2FuncBottom(CURSORCTRL_MOVETYPE_TOTAL);
                }
            }
            else
            {
                p_CursorFuntion->hideCursorFrom();
                //cursor move to buttom of Tab button
                m_pageDate.icursorPos = i_tab_btn_total - 1;
                m_iCursorPos_abs = m_pageDate.icursorPos;
                m_iCursorPos_rel = m_pageDate.i_itemPerPage;
                m_iTabCurSorPos_rel = 3;
               m_enCusorType = EN_CURSORCTRL_CURSOR_TYPE_TAB_BTN;

                if (0 == p_CursorFuntion->get_btn_sta_by_index(EN_CURSORCTRL_CURSOR_TYPE_TAB_BTN,m_pageDate.icursorPos))
                {
                    p_CursorFuntion->TabBtnFoucsed(EN_CURSORCTRL_CURSOR_TYPE_TAB_BTN, m_pageDate.icursorPos);
                }
                else
                {
                    act_moveUp();
                }
            }
        }
        else
        {
            int loop = m_pageDate.icursorPos;
            for(; loop > 0; loop--)
            {
                if (0 == p_CursorFuntion->get_btn_sta_by_index(EN_CURSORCTRL_CURSOR_TYPE_FUNC_BTN,loop - 1))
                {
                    break;
                }
            }

            if(loop == 0)
            {
                i_tab_btn_total = p_CursorFuntion->get_totalNumOfTabButton();
                if (0 < i_tab_btn_total)
                {
                    p_CursorFuntion->hideCursorFrom();
                    //cursor move to buttom of Tab button
                    m_pageDate.icursorPos = i_tab_btn_total - 1;
                    m_iCursorPos_abs = m_pageDate.icursorPos;
                    m_iCursorPos_rel = m_pageDate.i_itemPerPage;

                    m_enCusorType = EN_CURSORCTRL_CURSOR_TYPE_TAB_BTN;

                    if (0 == p_CursorFuntion->get_btn_sta_by_index(EN_CURSORCTRL_CURSOR_TYPE_TAB_BTN,m_pageDate.icursorPos))
                    {
                        p_CursorFuntion->TabBtnFoucsed(EN_CURSORCTRL_CURSOR_TYPE_TAB_BTN, m_pageDate.icursorPos);
                    }
                    else
                    {
                        act_moveUp();
                    }
                }
                timerStart();
                return iret;
            }

            p_CursorFuntion->hideCursorFrom();
            m_pageDate.icursorPos -= 1;

            i_startIdx = p_CursorFuntion->get_curStartIndex();
            if ((0 == m_iCursorPos_rel) && (0 < i_startIdx))
            {

            }
            else
            {
                m_iCursorPos_rel = m_pageDate.icursorPos - i_startIdx;
            }
            m_iCursorPos_abs = m_pageDate.icursorPos;

            m_pageDate.a_stLayerset[m_currentLayer].i_startIndex = i_startIdx;
            m_pageDate.a_stLayerset[m_currentLayer].i_cursorIndex_abs = m_iCursorPos_abs;

            if (0 == p_CursorFuntion->get_btn_sta_by_index(EN_CURSORCTRL_CURSOR_TYPE_FUNC_BTN,m_pageDate.icursorPos))
            {
                p_CursorFuntion->moveCursorTo(m_pageDate.icursorPos);
            }
            else
            {
                act_moveUp();
            }
        }
        break;
    default:
        i_resetTimer = 0;
        break;
    }

    if (1 == i_resetTimer)
    {
        timerStart();
    }

    return iret;
}
int CursorCtrlClass::act_moveDown()
{
    if ( NULL == p_CursorFuntion )
    {
        HMI_LOG("NULL == p_CursorFuntion");
        return 0;
    }
    HMI_LOG("m_enCusorType = %d ",m_enCusorType);

    int iret = 0;
    int i_startIdx = 0;
    int i_func_btn_total = 0;
    int i_tab_btn_total = 0;
    int i_resetTimer = 1;

    switch (m_enCusorType)
    {
    case EN_CURSORCTRL_CURSOR_TYPE_NO_CURSOR:
        i_resetTimer = 0;
        break;
    case EN_CURSORCTRL_CURSOR_TYPE_RETURN_BTN:
        //cursor is at return button, move cursor to top of tab button
        p_CursorFuntion->TabBtnUnfoucsed(EN_CURSORCTRL_CURSOR_TYPE_RETURN_BTN);

        i_tab_btn_total = p_CursorFuntion->get_totalNumOfTabButton();
        if (0 >= i_tab_btn_total)
        {
            i_func_btn_total = p_CursorFuntion->get_totalNum();
            if (0 >= i_func_btn_total)
            {
                //there is not any function button, cursor move to home button
                p_CursorFuntion->TabBtnFoucsed(EN_CURSORCTRL_CURSOR_TYPE_HOME_BTN, 0);
                m_enCusorType = EN_CURSORCTRL_CURSOR_TYPE_HOME_BTN;
            }
            else
            {
                MoveCursor2FuncTop(CURSORCTRL_MOVETYPE_PAGE);
            }
        }
        else
        {
            m_pageDate.icursorPos = 0;
            m_enCusorType = EN_CURSORCTRL_CURSOR_TYPE_TAB_BTN;
            m_iCursorPos_abs = m_pageDate.icursorPos;
            m_iCursorPos_rel = m_pageDate.icursorPos;

            if (0 == p_CursorFuntion->get_btn_sta_by_index(EN_CURSORCTRL_CURSOR_TYPE_TAB_BTN,0))
            {
                p_CursorFuntion->TabBtnFoucsed(EN_CURSORCTRL_CURSOR_TYPE_TAB_BTN, m_pageDate.icursorPos);
            }
            else
            {
                act_moveDown();
            }
        }
        break;
    case EN_CURSORCTRL_CURSOR_TYPE_HOME_BTN:
        //cursor is on Home button, move cursor to return
        p_CursorFuntion->TabBtnUnfoucsed(EN_CURSORCTRL_CURSOR_TYPE_HOME_BTN);
        p_CursorFuntion->TabBtnFoucsed(EN_CURSORCTRL_CURSOR_TYPE_RETURN_BTN, 0);

        m_enCusorType = EN_CURSORCTRL_CURSOR_TYPE_RETURN_BTN;
        break;
    case EN_CURSORCTRL_CURSOR_TYPE_TAB_BTN:
        p_CursorFuntion->TabBtnUnfoucsed(EN_CURSORCTRL_CURSOR_TYPE_TAB_BTN);
        i_tab_btn_total = p_CursorFuntion->get_totalNumOfTabButton();
        // cursor is first button of tab button,move cursor to return
        if((i_tab_btn_total - 1) == m_pageDate.icursorPos)
        {
            i_func_btn_total = p_CursorFuntion->get_totalNum();
            if (0 >= i_func_btn_total)
            {
                //there is not any function button, cursor move to home button
                p_CursorFuntion->TabBtnFoucsed(EN_CURSORCTRL_CURSOR_TYPE_HOME_BTN, 0);
                m_enCusorType = EN_CURSORCTRL_CURSOR_TYPE_HOME_BTN;
            }
            else
            {
                MoveCursor2FuncTop(CURSORCTRL_MOVETYPE_PAGE);
            }
        }
        else
        {
            m_pageDate.icursorPos += 1;
            m_iCursorPos_abs = m_pageDate.icursorPos;
            m_iCursorPos_rel = m_pageDate.icursorPos;
            if ( 3 > m_iTabCurSorPos_rel )
            {
                m_iTabCurSorPos_rel += 1;
            }
            if (0 == p_CursorFuntion->get_btn_sta_by_index(EN_CURSORCTRL_CURSOR_TYPE_TAB_BTN,m_pageDate.icursorPos))
            {
                p_CursorFuntion->TabBtnFoucsed(EN_CURSORCTRL_CURSOR_TYPE_TAB_BTN, m_pageDate.icursorPos);
            }
            else
            {
                act_moveDown();
            }
        }

        break;
    case EN_CURSORCTRL_CURSOR_TYPE_FUNC_BTN:
        //p_CursorFuntion->hideCursorFrom();
        i_func_btn_total = p_CursorFuntion->get_totalNum();
        i_startIdx = p_CursorFuntion->get_curStartIndex();
        HMI_LOG("i_func_btn_total = %d",i_func_btn_total);
        if ((i_func_btn_total -1) == m_pageDate.icursorPos )
        {
            if (EN_CURSORCTRL_LAYER_TYPE_SETTING_LIST == m_pageDate.a_stLayerset[m_currentLayer].en_layerType)
            {
                //MoveCursor2FuncTop(CURSORCTRL_MOVETYPE_TOTAL);
                break;
            }
            else if(EN_CURSORCTRL_LAYER_TYPE_EQ_LIST == m_pageDate.a_stLayerset[m_currentLayer].en_layerType)
            {
                MoveCursor2FuncTop(CURSORCTRL_MOVETYPE_TOTAL);
                break;
            }
            else if(EN_CURSORCTRL_LAYER_TYPE_CHARGE_TIME == m_pageDate.a_stLayerset[m_currentLayer].en_layerType)
            {
                MoveCursor2FuncTop(CURSORCTRL_MOVETYPE_TOTAL);
                break;
            }
            if (1 == m_pageDate.i_returnBtnExist)
            {
                //cursor is at the top of list,move cursor to bottom of tab button
                p_CursorFuntion->hideCursorFrom();
                m_pageDate.icursorPos = 0;
                m_iCursorPos_abs = 0;
                m_iCursorPos_rel = 0;
                p_CursorFuntion->TabBtnFoucsed(EN_CURSORCTRL_CURSOR_TYPE_HOME_BTN, 0);
                m_enCusorType = EN_CURSORCTRL_CURSOR_TYPE_HOME_BTN;
            }
            else
            {
                i_tab_btn_total = p_CursorFuntion->get_totalNumOfTabButton();
                if (0 >= i_tab_btn_total)
                {
                    //cursor move to top of total of function button
                    //MoveCursor2FuncTop(CURSORCTRL_MOVETYPE_TOTAL);
                }
                else
                {
                    p_CursorFuntion->hideCursorFrom();
                    //cursor move to top of page of Tab button
                    m_iCursorPos_rel = 0;
                    m_iCursorPos_abs = 0;
                    m_pageDate.icursorPos = m_iCursorPos_abs;
                    m_iTabCurSorPos_rel = 0;
                   m_enCusorType = EN_CURSORCTRL_CURSOR_TYPE_TAB_BTN;

                    if (0 == p_CursorFuntion->get_btn_sta_by_index(EN_CURSORCTRL_CURSOR_TYPE_TAB_BTN,m_pageDate.icursorPos))
                    {
                        p_CursorFuntion->TabBtnFoucsed(EN_CURSORCTRL_CURSOR_TYPE_TAB_BTN, m_pageDate.icursorPos);
                    }
                    else
                    {
                        act_moveDown();
                    }
                }
            }
        }
        else
        {
            int loop = m_pageDate.icursorPos;
            for(; loop < i_func_btn_total -1; loop++)
            {
                if (0 == p_CursorFuntion->get_btn_sta_by_index(EN_CURSORCTRL_CURSOR_TYPE_FUNC_BTN,loop + 1))
                {
                    break;
                }
            }

            if(loop == i_func_btn_total -1)
            {
                i_tab_btn_total = p_CursorFuntion->get_totalNumOfTabButton();
                if (0 < i_tab_btn_total)
                {
                    p_CursorFuntion->hideCursorFrom();
                    //cursor move to buttom of Tab button
                    m_iCursorPos_rel = 0;
                    m_iCursorPos_abs = 0;
                    m_pageDate.icursorPos = m_iCursorPos_abs;
                    m_enCusorType = EN_CURSORCTRL_CURSOR_TYPE_TAB_BTN;

                    if (0 == p_CursorFuntion->get_btn_sta_by_index(EN_CURSORCTRL_CURSOR_TYPE_TAB_BTN,m_pageDate.icursorPos))
                    {
                        p_CursorFuntion->TabBtnFoucsed(EN_CURSORCTRL_CURSOR_TYPE_TAB_BTN, m_pageDate.icursorPos);
                    }
                    else
                    {
                        act_moveDown();
                    }
                }
                timerStart();
                return iret;
            }

            p_CursorFuntion->hideCursorFrom();
            m_pageDate.icursorPos += 1;


            if (m_iCursorPos_rel == m_pageDate.i_itemPerPage)
            {

            }
            else
            {
                m_iCursorPos_rel = m_pageDate.icursorPos - i_startIdx;
            }

            m_iCursorPos_abs = m_pageDate.icursorPos;
            m_pageDate.a_stLayerset[m_currentLayer].i_startIndex = i_startIdx;
            m_pageDate.a_stLayerset[m_currentLayer].i_cursorIndex_abs = m_iCursorPos_abs;

            if (0 == p_CursorFuntion->get_btn_sta_by_index(EN_CURSORCTRL_CURSOR_TYPE_FUNC_BTN,m_pageDate.icursorPos))
            {
                p_CursorFuntion->moveCursorTo(m_pageDate.icursorPos);
            }
            else
            {
                act_moveDown();
            }
        }
        break;
    default:
        i_resetTimer = 0;
        break;
    }

    if (1 == i_resetTimer)
    {
        timerStart();
    }

    return iret;
}
int CursorCtrlClass::act_hide()
{
    if ( NULL == p_CursorFuntion )
    {
        HMI_LOG("NULL == p_CursorFuntion");
        return 0;
    }

    int iret = 0;

    HMI_LOG("m_enCusorType = %d ",m_enCusorType);

    switch (m_enCusorType)
    {
    case EN_CURSORCTRL_CURSOR_TYPE_NO_CURSOR:
        break;
    case EN_CURSORCTRL_CURSOR_TYPE_RETURN_BTN:
    case EN_CURSORCTRL_CURSOR_TYPE_HOME_BTN:
        p_CursorFuntion->TabBtnUnfoucsed(m_enCusorType);
        break;
    case EN_CURSORCTRL_CURSOR_TYPE_TAB_BTN:
        p_CursorFuntion->TabBtnUnfoucsed(m_enCusorType);
        break;
    case EN_CURSORCTRL_CURSOR_TYPE_FUNC_BTN:
        p_CursorFuntion->hideCursorFrom();
        break;
    default:
        break;
    }

    timerStop();

    return iret;
}

int CursorCtrlClass::act_show()
{
    if ( NULL == p_CursorFuntion )
    {
        HMI_LOG("NULL == p_CursorFuntion");
        return 0;
    }
    int iret = 0;
    int i_resetTimer = 1;
    int i_startIdx = 0;
    int i_func_btn_total = 0;

    HMI_LOG("m_enCusorType = %d ",m_enCusorType);

    switch (m_enCusorType)
    {
    case EN_CURSORCTRL_CURSOR_TYPE_NO_CURSOR:
        i_resetTimer = 0;
        break;
    case EN_CURSORCTRL_CURSOR_TYPE_RETURN_BTN:
    case EN_CURSORCTRL_CURSOR_TYPE_HOME_BTN:
        p_CursorFuntion->TabBtnFoucsed(m_enCusorType, 0);
        break;
    case EN_CURSORCTRL_CURSOR_TYPE_TAB_BTN:
        m_pageDate.icursorPos = m_iTabCurSorPos_rel + i_startIdx;

        p_CursorFuntion->TabBtnFoucsed(m_enCusorType, m_pageDate.icursorPos);
        break;
    case EN_CURSORCTRL_CURSOR_TYPE_FUNC_BTN:
        i_startIdx = p_CursorFuntion->get_curStartIndex();

        // when change page ,the total number maybe different
        i_func_btn_total = p_CursorFuntion->get_totalNum();

        m_iCursorPos_abs = m_pageDate.a_stLayerset[m_currentLayer].i_cursorIndex_abs;

        HMI_LOG("m_iCursorPos_abs = %d ",m_iCursorPos_abs);
        HMI_LOG("m_iCursorPos_rel = %d ",m_iCursorPos_rel);
        if (m_iCursorPos_abs >= i_func_btn_total)
        {
            m_pageDate.icursorPos = i_func_btn_total -1 ;
            m_iCursorPos_rel = m_pageDate.icursorPos - i_startIdx;
            m_iCursorPos_abs = m_pageDate.icursorPos;
        }
        else
        {
            m_pageDate.icursorPos = m_iCursorPos_rel+i_startIdx;
        }

        HMI_LOG("m_pageDate.icursorPos = %d ",m_pageDate.icursorPos);
        p_CursorFuntion->moveCursorTo(m_pageDate.icursorPos);
        break;
    default:
        i_resetTimer = 0;
        break;
    }

    if (1 == i_resetTimer)
    {
        timerStart();
    }

    return iret;
}

int CursorCtrlClass::act_hideUpdate()
{
    if ( NULL == p_CursorFuntion )
    {
        HMI_LOG("NULL == p_CursorFuntion");
        return 0;
    }

    int iret = 0;
    int i_startIdx = 0;

    HMI_LOG("m_enCusorType = %d ",m_enCusorType);

    if (EN_CURSORCTRL_CURSOR_TYPE_FUNC_BTN == m_enCusorType)
    {
        i_startIdx = p_CursorFuntion->get_curStartIndex();

        m_iCursorPos_abs  = i_startIdx + m_iCursorPos_rel;
        m_pageDate.icursorPos =  m_iCursorPos_abs;
    }
    else
    {
        // do nothing
    }

    return iret;
}
int CursorCtrlClass::act_enter()
{
    if ( NULL == p_CursorFuntion )
    {
        HMI_LOG("NULL == p_CursorFuntion");
        return 0;
    }

    int iret = 0;
    int i_startIdx = 0;

    HMI_LOG("m_enCusorType = %d ",m_enCusorType);

    if (EN_CURSORCTRL_CURSOR_TYPE_FUNC_BTN != m_enCusorType)
    {
        timerStart();
        return iret;
    }

    if (m_currentLayer >= m_pageDate.ilayerCount - 1) // the lowest list press
    {
        //do nothing
        timerStart();
        return iret;
    }

    p_CursorFuntion->hideCursorFrom();

    m_currentLayer++;

    if (EN_CURSORCTRL_LAYER_TYPE_MEIDA_LIST == m_pageDate.a_stLayerset[m_currentLayer].en_layerType )
    {
        i_startIdx = p_CursorFuntion->get_curStartIndex();
    }

    HMI_LOG("i_startIdx = %d", i_startIdx);
    p_CursorFuntion->moveCursorTo(i_startIdx);
    m_iCursorPos_abs = i_startIdx;
    m_iCursorPos_rel = 0;
    m_pageDate.icursorPos = m_iCursorPos_abs;
    m_pageDate.a_stLayerset[m_currentLayer].i_startIndex = i_startIdx;
    m_pageDate.a_stLayerset[m_currentLayer].i_cursorIndex_abs = i_startIdx;

    timerStart();
    return iret;
}
int CursorCtrlClass::act_back()
{
    if ( NULL == p_CursorFuntion )
    {
        HMI_LOG("NULL == p_CursorFuntion");
        return 0;
    }

    int iret = 0;
    HMI_LOG("m_enCusorType = %d ",m_enCusorType);

    if (EN_CURSORCTRL_CURSOR_TYPE_FUNC_BTN != m_enCusorType)
    {
        timerStart();
        return iret;
    }

    if (0 == m_currentLayer)
    {
        timerStart();
        return iret;
    }

    p_CursorFuntion->hideCursorFrom();

    m_currentLayer--;
    m_iCursorPos_abs = m_pageDate.a_stLayerset[m_currentLayer].i_cursorIndex_abs;
    m_iCursorPos_rel = m_pageDate.a_stLayerset[m_currentLayer].i_cursorIndex_abs
            -m_pageDate.a_stLayerset[m_currentLayer].i_startIndex ;
    m_pageDate.icursorPos = m_iCursorPos_abs;
    p_CursorFuntion->moveCursorTo(m_iCursorPos_abs);

    timerStart();

    return iret;
}
int CursorCtrlClass::act_enterHide()
{
    int i_startIdx = p_CursorFuntion->get_curStartIndex();
    if ( NULL == p_CursorFuntion )
    {
        HMI_LOG("NULL == p_CursorFuntion");
        return 0;
    }

    int iret = 0;
    HMI_LOG("m_enCusorType = %d ",m_enCusorType);
    if (m_currentLayer >= m_pageDate.ilayerCount - 1) // the lowest list press
    {
        //do nothing
        return iret;
    }

    m_currentLayer++;
    m_iCursorPos_abs = m_pageDate.a_stLayerset[m_currentLayer].i_cursorIndex_abs;
    m_iCursorPos_rel = m_iCursorPos_abs - i_startIdx;
    return iret;
}
int CursorCtrlClass::act_backHide()
{
    int i_startIdx = p_CursorFuntion->get_curStartIndex();
    if ( NULL == p_CursorFuntion )
    {
        HMI_LOG("NULL == p_CursorFuntion");
        return 0;
    }

    int iret = 0;
    HMI_LOG("m_enCusorType = %d ",m_enCusorType);
    if (0 == m_currentLayer)
    {
        return iret;
    }

    m_currentLayer--;
    m_iCursorPos_abs = m_pageDate.a_stLayerset[m_currentLayer].i_cursorIndex_abs;
    m_iCursorPos_rel = m_iCursorPos_abs - i_startIdx;
    return iret;
}

void CursorCtrlClass::MoveCursor2FuncTop(int type)
{
    int i_func_btn_total = p_CursorFuntion->get_totalNum();
    int i_startIdx = 0;

    if (m_pageDate.i_itemPerPage >= i_func_btn_total)
    {
        //cursor move to bottom of Tab button, when function button is less than one page
        m_iCursorPos_rel = 0;
        m_iCursorPos_abs = 0;
        m_pageDate.icursorPos = m_iCursorPos_abs;

        //p_CursorFuntion->moveCursorTo(m_iCursorPos_abs);

        m_pageDate.a_stLayerset[m_currentLayer].i_startIndex = 0;
        m_pageDate.a_stLayerset[m_currentLayer].i_cursorIndex_abs = m_iCursorPos_abs;

        m_enCusorType = EN_CURSORCTRL_CURSOR_TYPE_FUNC_BTN;

        if (0 == p_CursorFuntion->get_btn_sta_by_index(EN_CURSORCTRL_CURSOR_TYPE_FUNC_BTN,m_pageDate.icursorPos))
        {
            p_CursorFuntion->moveCursorTo(m_pageDate.icursorPos);
        }
        else
        {
            act_moveDown();
        }
    }
    else
    {
        //cursor move to bottom of page of function button
        i_startIdx = p_CursorFuntion->get_curStartIndex();

        m_iCursorPos_rel = 0;
        if (CURSORCTRL_MOVETYPE_PAGE == type)
        {
            m_iCursorPos_abs = i_startIdx ;
        }
        else
        {
            m_iCursorPos_abs = 0 ;
        }
        m_pageDate.icursorPos = m_iCursorPos_abs;

        //p_CursorFuntion->moveCursorTo(m_iCursorPos_abs);

        m_enCusorType = EN_CURSORCTRL_CURSOR_TYPE_FUNC_BTN;

        if (0 == p_CursorFuntion->get_btn_sta_by_index(EN_CURSORCTRL_CURSOR_TYPE_FUNC_BTN,m_pageDate.icursorPos))
        {
            p_CursorFuntion->moveCursorTo(m_pageDate.icursorPos);
        }
        else
        {
            act_moveDown();
        }
    }
}
void CursorCtrlClass::MoveCursor2FuncBottom(int type)
{
    int i_func_btn_total = p_CursorFuntion->get_totalNum();
    int i_startIdx = 0;

    if (m_pageDate.i_itemPerPage >= i_func_btn_total)
    {
        //cursor move to bottom of Tab button, when function button is less than one page
        i_startIdx = p_CursorFuntion->get_curStartIndex();

        m_iCursorPos_rel = i_func_btn_total - 1;
        m_iCursorPos_abs = i_func_btn_total - 1 ;
        m_pageDate.icursorPos = m_iCursorPos_abs;

        //p_CursorFuntion->moveCursorTo(m_iCursorPos_abs);

        m_pageDate.a_stLayerset[m_currentLayer].i_startIndex = i_startIdx;
        m_pageDate.a_stLayerset[m_currentLayer].i_cursorIndex_abs = m_iCursorPos_abs;

        m_enCusorType = EN_CURSORCTRL_CURSOR_TYPE_FUNC_BTN;

        if (0 == p_CursorFuntion->get_btn_sta_by_index(EN_CURSORCTRL_CURSOR_TYPE_FUNC_BTN,m_pageDate.icursorPos))
        {
            p_CursorFuntion->moveCursorTo(m_pageDate.icursorPos);
        }
        else
        {
            act_moveUp();
        }
    }
    else
    {
        //cursor move to bottom of page of function button
        p_CursorFuntion->hideCursorFrom();

        i_startIdx = p_CursorFuntion->get_curStartIndex();

        m_iCursorPos_rel = m_pageDate.i_itemPerPage - 1;
        if (CURSORCTRL_MOVETYPE_PAGE == type)
        {
            m_iCursorPos_abs = i_startIdx + m_pageDate.i_itemPerPage - 1 ;
        }
        else
        {
            m_iCursorPos_abs = i_func_btn_total -1;
        }
        m_pageDate.icursorPos = m_iCursorPos_abs;

        //p_CursorFuntion->moveCursorTo(m_iCursorPos_abs);

        m_pageDate.a_stLayerset[m_currentLayer].i_startIndex = i_startIdx;
        m_pageDate.a_stLayerset[m_currentLayer].i_cursorIndex_abs = m_iCursorPos_abs;

        m_enCusorType = EN_CURSORCTRL_CURSOR_TYPE_FUNC_BTN;

        if (0 == p_CursorFuntion->get_btn_sta_by_index(EN_CURSORCTRL_CURSOR_TYPE_FUNC_BTN,m_pageDate.icursorPos))
        {
            p_CursorFuntion->moveCursorTo(m_pageDate.icursorPos);
        }
        else
        {
            act_moveUp();
        }
    }
}
void CursorCtrlClass::MoveCursor2TabTop()
{

}
void CursorCtrlClass::MoveCursor2TabBottom()
{

}

void CursorCtrlClass::KnobPressed()
{
    HMI_LOG(" KnobPressed  m_enCusorType = %d" ,m_enCusorType);
    if (EN_CURSORCTRL_CURSOR_TYPE_NO_CURSOR != m_enCusorType)
    {
        timerStop();
    }
}
void CursorCtrlClass::KnobReleased()
{
    HMI_LOG(" KnobReleased  m_enCusorType = %d" ,m_enCusorType);
    if (EN_CURSORCTRL_CURSOR_TYPE_NO_CURSOR != m_enCusorType)
    {
        timerStart();
    }
}

void CursorCtrlClass::StopTime()
{
    timerStop();

    return;
}

void CursorCtrlClass::ResetTimer()
{
    timerStart();
    return;
}
