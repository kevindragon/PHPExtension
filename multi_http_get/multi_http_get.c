/*
  +----------------------------------------------------------------------+
  | PHP Version 4                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2006 The PHP Group                                |
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

/* $Id: multi_http_get.c,v 1.2 2012/05/25 02:09:12 kevin Exp $ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_multi_http_get.h"

#include <pthread.h>
#include <curl/curl.h>

/* If you declare any globals in php_multi_http_get.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(multi_http_get)
*/

/* True global resources - no need for thread safety here */
static int le_multi_http_get;

/* {{{ multi_http_get_functions[]
 *
 * Every user visible function must have an entry in multi_http_get_functions[].
 */
zend_function_entry multi_http_get_functions[] = {
	PHP_FE(confirm_multi_http_get_compiled,	NULL)		/* For testing, remove later. */
    PHP_FE(autn_multi_get,  NULL)                       /* Develop by LexisNexis */
	{NULL, NULL, NULL}	/* Must be the last line in multi_http_get_functions[] */
};
/* }}} */

/* {{{ multi_http_get_module_entry
 */
zend_module_entry multi_http_get_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"multi_http_get",
	multi_http_get_functions,
	PHP_MINIT(multi_http_get),
	PHP_MSHUTDOWN(multi_http_get),
	PHP_RINIT(multi_http_get),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(multi_http_get),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(multi_http_get),
#if ZEND_MODULE_API_NO >= 20010901
	"0.1", /* Replace with version number for your extension */
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_MULTI_HTTP_GET
ZEND_GET_MODULE(multi_http_get)
#endif

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("multi_http_get.global_value",      "42", PHP_INI_ALL, OnUpdateInt, global_value, zend_multi_http_get_globals, multi_http_get_globals)
    STD_PHP_INI_ENTRY("multi_http_get.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_multi_http_get_globals, multi_http_get_globals)
PHP_INI_END()
*/
/* }}} */

/* {{{ php_multi_http_get_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_multi_http_get_init_globals(zend_multi_http_get_globals *multi_http_get_globals)
{
	multi_http_get_globals->global_value = 0;
	multi_http_get_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(multi_http_get)
{
	/* If you have INI entries, uncomment these lines 
	ZEND_INIT_MODULE_GLOBALS(multi_http_get, php_multi_http_get_init_globals, NULL);
	REGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(multi_http_get)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(multi_http_get)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(multi_http_get)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(multi_http_get)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "multi_http_get support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */


/* Remove the following function when you have succesfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_multi_http_get_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(confirm_multi_http_get_compiled)
{
	char *arg = NULL;
	int arg_len, len;
	char string[256];

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &arg, &arg_len) == FAILURE) {
		return;
	}

	len = sprintf(string, "Congratulations! You have successfully modified ext/%.78s/config.m4. Module %.78s is now compiled into PHP.", "multi_http_get", arg);
	RETURN_STRINGL(string, len, 1);
}
/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and 
   unfold functions in source code. See the corresponding marks just before 
   function definition, where the functions purpose is also documented. Please 
   follow this convention for the convenience of others editing your code.
*/


/* Developed by LexisNexis start {{{ */
struct MemoryStruct {
    char *memory;
    size_t size;
};

struct ThreadStruct {
	char *url;
	struct MemoryStruct *xmldata;
};

static size_t WriteMemoryCallback(void *ptr, size_t size, size_t nmemb, void *data)
{
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)data;

    mem->memory = realloc(mem->memory, mem->size + realsize + 1);
    if (mem->memory == NULL) {
        /* out of memory! */ 
        printf("not enough memory (realloc returned NULL)\n");
        exit(EXIT_FAILURE);
    }

    memcpy(&(mem->memory[mem->size]), ptr, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}


static void *pull_one_url(void *args)
{
	/* Get the parameter from main thread */
	char *url;
	struct MemoryStruct *xmldata;
	struct ThreadStruct *ts = (struct ThreadStruct *)args;
	
	url = ts->url;
	xmldata = ts->xmldata;

    CURL *curl_handle;

    /* init the curl session */ 
    curl_handle = curl_easy_init();

    /* specify URL to get */ 
    curl_easy_setopt(curl_handle, CURLOPT_URL, url);

    /* send all data to this function  */ 
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

    /* we pass our 'chunk' struct to the callback function */ 
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)xmldata);

    /* some servers don't like requests that are made without a user-agent
       field, so we provide one */ 
    curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

    /* get it! */ 
    curl_easy_perform(curl_handle);

    /* cleanup curl stuff */ 
    curl_easy_cleanup(curl_handle);

    /*
     * Now, our chunk.memory points to a memory block that is chunk.size
     * bytes big and contains the remote file.
     *
     * Do something nice with it!
     *
     * You should be aware of the fact that at this point we might have an
     * allocated data block, and nothing has yet deallocated that data. So when
     * you're done with it, you should free() it as a nice application.
     */ 

    //printf("%lu bytes retrieved\n", (long)(*xmldata).size);
    //printf("%s", xmldata.memory);
}

/* {{{ This is my function */
PHP_FUNCTION(autn_multi_get)
{
	/* first get parameters from php script */
	zval *param;
	if (zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, ZEND_NUM_ARGS() TSRMLS_CC, 
				"a", &param) == FAILURE) {
		zend_error(E_WARNING, "Parameter was error");
	}

	/* read array data from param */
	zval **data;
	HashTable *arr_hash;
	HashPosition hp;
	int array_count;

	arr_hash = Z_ARRVAL_P(param);
	array_count = zend_hash_num_elements(arr_hash);

	char *urls[array_count];
	int urlidx = 0;
	
	/* get the url and add into curl multiple request queue {{{ */
	for (zend_hash_internal_pointer_reset_ex(arr_hash, &hp);
			zend_hash_get_current_data_ex(arr_hash, (void**) &data, &hp) == SUCCESS; 
			zend_hash_move_forward_ex(arr_hash, &hp)) {
		if (Z_TYPE_PP(data) == IS_STRING) {
			/* save url into urls variable */
			urls[urlidx++] = Z_STRVAL_PP(data);
		}
	}
	/* }}} */

	/* initialize xml container */
	struct MemoryStruct chunk[array_count], *xx[array_count];
	int j, thread_error;
	struct ThreadStruct ts[array_count];
	pthread_t tid[array_count];

    /* Must initialize libcurl before any threads are started */ 
    curl_global_init(CURL_GLOBAL_ALL);

	/* create multiple thread to get xml from Autonomy server */
    for (j = 0; j < array_count; j++) {
    	xx[j] = (chunk+j);
    	xx[j]->memory = malloc(1);
    	xx[j]->size = 0;

    	/* Transfer the thread parameters */
    	ts[j].url = urls[j];
    	ts[j].xmldata = xx[j];
    	
    	/* create child thread */
    	thread_error = pthread_create(&tid[j], NULL, 
    								pull_one_url, 
    								(void *)&ts[j]);
    }

    /* now wait for all threads to terminate */ 
	for (j = 0; j < array_count; j++) {
		thread_error = pthread_join(tid[j], NULL);
	}

    /* we're done with libcurl, so clean it up */ 
    curl_global_cleanup();

	zval *array;				/* put return data in this variable */
	MAKE_STD_ZVAL(array);		/* regist array variable in Zend hash table */
	array_init(array);			/* initialize this array */

	for (j = 0; j < array_count; j++) {
		add_index_string(array, j, chunk[j].memory, 1);
		if (xx[j]->memory) {
			free(xx[j]->memory);
            xx[j] = NULL;
		}
	}

	*return_value = *array;		/* the php script get return value from return_value */
}
/* Developed by LexisNexis end }}} */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
