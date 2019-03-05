#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php_polarlog.h"

static int le_polarlog;
ZEND_DECLARE_MODULE_GLOBALS(polarlog)

PHP_INI_BEGIN()
    PHP_INI_ENTRY("polarlog.conf_name", "polarlog.conf", PHP_INI_ALL, NULL)
    PHP_INI_ENTRY("polarlog.conf_dir",  "./", PHP_INI_ALL, NULL)
PHP_INI_END()


PHP_FUNCTION(polarlog_version)
{
	zval ver;
	ZVAL_STRING(&ver, PHP_POLARLOG_VERSION);
	RETURN_ZVAL(&ver, true, true);
}

PHP_FUNCTION(polarlog_set_level)
{

	char *level;
	size_t level_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &level, &level_len) == FAILURE) {
		return;
	}

	if (strlen(level) == 0) {
		level = "DEBUG";
	}

	if (strcasecmp(level, "ERROR") == 0 ||
		strcasecmp(level, "WARN")  == 0 ||
		strcasecmp(level, "INFO")  == 0 ||
		strcasecmp(level, "DEBUG") == 0
	) {
		polarlog_set_level(level);
		RETURN_TRUE;
	} else {
		RETURN_FALSE;
	}
}

PHP_FUNCTION(polarlog_get_level)
{
	zend_string *result;
	const char *level_str = polarlog_get_level();
	RETURN_STRING(level_str);
}


PHP_FUNCTION(PLOG)
{

	char *msg, *level;
	size_t msg_len, level_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "ss", &level, &level_len, &msg, &msg_len) == FAILURE) {
		return;
	}

	if (msg_len >= LOG_MAX_SIZE) {
		RETURN_FALSE;
	}

	f_name_line tmp_nl = {0};
	self_get_code_filename_line(&tmp_nl TSRMLS_DC);

	tmp_nl.name[tmp_nl.len] = '\0';
	char *file_name = tmp_nl.name;

	int file_line = tmp_nl.line;

	if (strcasecmp(level,"ERROR") == 0) {
		PHP_LOG(ERROR, file_line, file_name,msg);
	} else if (strcasecmp(level,"WARN") == 0) {
		PHP_LOG(WARN, file_line, file_name,msg);
	} else if (strcasecmp(level,"INFO") == 0) {
		PHP_LOG(INFO, file_line, file_name,msg);
	} else {
		PHP_LOG(DEBUG, file_line, file_name,msg);
	}
	
	RETURN_LONG(msg_len);
}

PHP_FUNCTION(PLOG_ERROR)
{
	PHP_LOG_COMMON(ERROR);
}

PHP_FUNCTION(PLOG_WARN)
{
	PHP_LOG_COMMON(WARN);

}

PHP_FUNCTION(PLOG_INFO)
{
	PHP_LOG_COMMON(INFO);
}

PHP_FUNCTION(PLOG_DEBUG)
{
	PHP_LOG_COMMON(DEBUG);
}

PHP_MINIT_FUNCTION(polarlog)
{
	REGISTER_INI_ENTRIES();
	const char *conf_file = const_cast<char *>(INI_STR("polarlog.conf_name"));
	const char *conf_dir  = const_cast<char *>(INI_STR("polarlog.conf_dir"));

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
	PHP_FE(polarlog_version,	NULL)
	PHP_FE(polarlog_set_level,  NULL)
	PHP_FE(polarlog_get_level, NULL)
	PHP_FE(PLOG, NULL)
	PHP_FE(PLOG_ERROR, NULL)
	PHP_FE(PLOG_WARN, NULL)
	PHP_FE(PLOG_INFO, NULL)
	PHP_FE(PLOG_DEBUG, NULL)
	PHP_FE_END	/* Must be the last line in tou_functions[] */
};

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


#ifdef COMPILE_DL_POLARLOG
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(polarlog)
#endif
	