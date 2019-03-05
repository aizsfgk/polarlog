/*
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2018 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifndef PHP_POLARLOG_H
#define PHP_POLARLOG_H

#include "polarlog.h"
#include "php.h"
#include "php_ini.h"
#include "zend_ini.h"
#include "ext/standard/info.h"
#include "SAPI.h"
#include "zend_globals_macros.h"
#include "zend_API.h"
#include <string>

/**
 * php 日志宏
 */

#define PHP_LOG_ERROR(line, filename, format, args...) \
  polarlog_write(LEVEL_ERROR, line, filename, format, ##args);

#define PHP_LOG_WARN(line, filename, format, args...) \
  polarlog_write(LEVEL_WARN, line, filename, format, ##args);

#define PHP_LOG_INFO(line, filename, format, args...) \
  polarlog_write(LEVEL_INFO, line, filename, format, ##args);

#define PHP_LOG_DEBUG(line, filename, format, args...) \
  polarlog_write(LEVEL_DEBUG, line, filename, format, ##args);

#define PHP_LOG(level, line, name, msg) \
  PHP_LOG_##level(line, name, "%s", msg) 


typedef struct _file_line {
  char *name;
  long len;
  long line;
} f_name_line;

static void self_get_code_filename_line(f_name_line *nl TSRMLS_DC)
{
    char *ret;
    long code_line = 0;
    size_t filename_len;
    int recall_depth = 0;

#if PHP_VERSION_ID >= 70000
 
    zend_execute_data *ptr = EG(current_execute_data);
    if (!ptr)
    {
        return;
    }

    while(recall_depth >= 0)
    {
        if (ptr->prev_execute_data != NULL && ptr->prev_execute_data->func &&
                ZEND_USER_CODE(ptr->prev_execute_data->func->common.type)
           )
        {
            ptr = ptr->prev_execute_data;
        }
        else
        {
            break;
        }
        recall_depth--;
    }

    if (ptr->func && ZEND_USER_CODE(ptr->func->type))
    {
        ret = ZSTR_VAL(ptr->func->op_array.filename);
        code_line = ptr->opline->lineno;
    }
    
    nl->name =ret;
    nl->len  = (long)strlen(ret);
    nl->line = (long)code_line;
#endif

}

#define PHP_LOG_COMMON(level) \
 do { \
  char *msg; \
  size_t msg_len; \
  if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &msg, &msg_len) == FAILURE) { \
    return; \
  } \
  if (msg_len >= LOG_MAX_SIZE) { \
    RETURN_FALSE; \
  } \
  f_name_line tmp_nl = {0}; \
  self_get_code_filename_line(&tmp_nl TSRMLS_DC); \
  tmp_nl.name[tmp_nl.len] = '\0'; \
  char *file_name = tmp_nl.name; \
  int file_line = tmp_nl.line; \
  PHP_LOG_##level(file_line, file_name, "%s", msg); \
  RETURN_LONG(msg_len); \
} while (0)


#ifdef __cplusplus
extern "C" {
#endif


// 声明模块
extern zend_module_entry polarlog_module_entry;
#define phpext_tou_ptr &polarlog_module_entry

#define PHP_POLARLOG_VERSION "0.1.0" /* Replace with version number for your extension */

#ifdef PHP_WIN32
# define PHP_POLARLOG_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
# define PHP_POLARLOG_API __attribute__ ((visibility("default")))
#else
# define PHP_POLARLOG_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

ZEND_BEGIN_MODULE_GLOBALS(polarlog)
  zend_string *conf_file;
  zend_string *conf_dir;
  int level;
ZEND_END_MODULE_GLOBALS(polarlog);

#ifdef __cplusplus
}
#endif

#define POLARLOG_G(v) ZEND_MODULE_GLOBALS_ACCESSOR(polarlog, v)

#if defined(ZTS) && defined(COMPILE_DL_TOU)
ZEND_TSRMLS_CACHE_EXTERN()
#endif

#endif  /* PHP_POLARLOG_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
