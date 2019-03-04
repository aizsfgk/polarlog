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

#ifdef __cplusplus
extern "C" {
#endif

#include "php.h"
#include "php_ini.h"
#include "zend_ini.h"
#include "ext/standard/info.h"
#include "SAPI.h"
#include "zend_globals_macros.h"


/**
 * php 日志宏
 */

PHP_LOG_ERROR(format, args...) \
  polarlog_write(LEVEL_ERROR, CG(lineno), CG(filename), format, ##args);

PHP_LOG_WARN(format, args...) \
  polarlog_write(LEVEL_WARN, CG(lineno), CG(filename), format, ##args);

PHP_LOG_INFO(format, args...) \
  polarlog_write(LEVEL_INFO, CG(lineno), CG(filename), format, ##args);

PHP_LOG_DEBUG(format, args...) \
  polarlog_write(LEVEL_DEBUG, CG(lineno), CG(filename), format, ##args);

PHP_LOG(level, msg) \
  PHP_LOG_##level("%s", msg) 




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
