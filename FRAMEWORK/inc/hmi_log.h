/**********************************************************************************/
/* log.h                                                                          */
/* created by Wang Xiaofeng                                                       */
/**********************************************************************************/

#pragma once

#include <stdio.h>

#ifndef HMI_API_STUB
#include <dlt.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif


#define HMI_VER "2019.01.17 14:55:43"

#ifndef HMI_API_STUB

#define HMI_TRACE(format, ...) hmi_dlt_log(DLT_LOG_DEBUG, "[%s][%s][%d]"format"", __FILE__,__func__, __LINE__, ##__VA_ARGS__)

#define HMI_TRACE_IN HMI_TRACE("IN")

#define HMI_LOG(format, ...) hmi_dlt_log(DLT_LOG_INFO, "[%s][%s][%d]"format"", __FILE__,__func__, __LINE__, ##__VA_ARGS__)

#define HMI_WARNING(format, ...) hmi_dlt_log(DLT_LOG_WARN, "[%s][%s][%d]"format"", __FILE__,__func__, __LINE__, ##__VA_ARGS__)

#define HMI_ERROR(format, ...) hmi_dlt_log(DLT_LOG_ERROR, "[%s][%s][%d]"format"", __FILE__,__func__, __LINE__, ##__VA_ARGS__)

void hmi_dlt_log_init();
void hmi_dlt_log(DltLogLevelType level, const char *format, ...);

#else

#define HMI_TRACE(format, ...) //qDebug("[%s][%s][%d]"format"", __FILE__,__func__, __LINE__, ##__VA_ARGS__)

#define HMI_TRACE_IN HMI_TRACE("IN")

#define HMI_LOG(format, ...) //qDebug("[%s][%s][%d]"format"", __FILE__,__func__, __LINE__, ##__VA_ARGS__)

#define HMI_WARNING(format, ...) //qDebug("[%s][%s][%d]"format"", __FILE__,__func__, __LINE__, ##__VA_ARGS__)

#define HMI_ERROR(format, ...) //qDebug("[%s][%s][%d]"format"", __FILE__,__func__, __LINE__, ##__VA_ARGS__)
#endif



#ifdef __cplusplus
}
#endif


