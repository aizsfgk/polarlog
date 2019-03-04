#include "php_polarlog.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

/*
	zend_string *compiled_filename;

	int zend_lineno;

	CG(zend_lineno)


	CG(compiled_filename);
 */


#include "php_polarlog.h"
#include <string>

extern "C" {
	#include "php.h"
	#include "php_ini.h"
	#include "zend_ini.h"
	#include "ext/standard/info.h"
	#include "SAPI.h"
}

static int le_polarlog;

ZEND_DECLARE_MODULE_GLOBALS(polarlog)

// polarlog::Polarlog *g_polarlog_logger = NULL;
// polarlog::FileAppender g_file_appender;


/*
	如何将这两个值设置为常量值
 */

PHP_INI_BEGIN()
    PHP_INI_ENTRY("polarlog.conf_name", "polarlog.conf", PHP_INI_ALL, NULL)
    PHP_INI_ENTRY("polarlog.conf_dir",  "/Users/zhangshifeng01/app/polarlog/conf", PHP_INI_ALL, NULL)
PHP_INI_END()


PHP_FUNCTION(polarlog_version)
{
	zval ver;
	ZVAL_STRING(&ver, PHP_POLARLOG_VERSION);
	RETURN_ZVAL(&ver, true, true);
}

PHP_FUNCTION(polarlog_set_level)
{
	zval zv_level;

}

PHP_FUNCTION(polarlog_get_level)
{

}


PHP_FUNCTION(PLOG)
{

}

PHP_FUNCTION(PLOG_WARN)
{

}

PHP_FUNCTION(PLOG_ERROR)
{

}

PHP_FUNCTION(PLOG_INFO)
{

}

PHP_FUNCTION(PLOG_DEBUG)
{

}



PHP_MINIT_FUNCTION(polarlog)
{

	REGISTER_INI_ENTRIES();

	char *conf_file = INI_STR("polarlog.conf_name");
	char *conf_dir  = INI_STR("polarlog.conf_dir");

	std::string file(conf_file);
	std::string dir(conf_dir);

	if (polarlog_init(file, dir) != 0) {
		// TODO
	}
	return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(polarlog)
{
	UNREGISTER_INI_ENTRIES();
	polarlog_end();
	return SUCCESS;
}

PHP_RINIT_FUNCTION(polarlog)
{
	return SUCCESS;
}


PHP_RSHUTDOWN_FUNCTION(polarlog)
{
	return SUCCESS;
}

PHP_MINFO_FUNCTION(polarlog)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "polarlog support", "enabled");
	php_info_print_table_row(2, "author", "twelve12代码家");
	php_info_print_table_end();

	DISPLAY_INI_ENTRIES();
}

const zend_function_entry polarlog_functions[] = {
	PHP_FE(polarlog_version,	NULL)		/* For testing, remove later. */
	PHP_FE(polarlog_set_level,  NULL)
	PHP_FE(polarlog_get_level, NULL)
	PHP_FE(PLOG, NULL)
	PHP_FE(PLOG_ERROR, NULL)
	PHP_FE(PLOG_WARN, NULL)
	PHP_FE(PLOG_INFO, NULL)
	PHP_FE(PLOG_DEBUG, NULL)
	PHP_FE_END	/* Must be the last line in tou_functions[] */
};
/* }}} */

/* {{{ tou_module_entry
 */
zend_module_entry polarlog_module_entry = {
	STANDARD_MODULE_HEADER,
	"polarlog",
	polarlog_functions,
	PHP_MINIT(polarlog),
	PHP_MSHUTDOWN(polarlog),
	PHP_RINIT(polarlog),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(polarlog),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(polarlog),
	PHP_POLARLOG_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_POLARLOG
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(polarlog)
#endif
