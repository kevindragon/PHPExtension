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

/* $Id: php_multi_http_get.h,v 1.1 2011/09/23 09:43:32 kevin Exp $ */

#ifndef PHP_MULTI_HTTP_GET_H
#define PHP_MULTI_HTTP_GET_H

extern zend_module_entry multi_http_get_module_entry;
#define phpext_multi_http_get_ptr &multi_http_get_module_entry

#ifdef PHP_WIN32
#define PHP_MULTI_HTTP_GET_API __declspec(dllexport)
#else
#define PHP_MULTI_HTTP_GET_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

PHP_MINIT_FUNCTION(multi_http_get);
PHP_MSHUTDOWN_FUNCTION(multi_http_get);
PHP_RINIT_FUNCTION(multi_http_get);
PHP_RSHUTDOWN_FUNCTION(multi_http_get);
PHP_MINFO_FUNCTION(multi_http_get);

PHP_FUNCTION(confirm_multi_http_get_compiled);	/* For testing, remove later. */
PHP_FUNCTION(autn_multi_get);

/* 
  	Declare any global variables you may need between the BEGIN
	and END macros here:     

ZEND_BEGIN_MODULE_GLOBALS(multi_http_get)
	long  global_value;
	char *global_string;
ZEND_END_MODULE_GLOBALS(multi_http_get)
*/

/* In every utility function you add that needs to use variables 
   in php_multi_http_get_globals, call TSRMLS_FETCH(); after declaring other 
   variables used by that function, or better yet, pass in TSRMLS_CC
   after the last function argument and declare your utility function
   with TSRMLS_DC after the last declared argument.  Always refer to
   the globals in your function as MULTI_HTTP_GET_G(variable).  You are 
   encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/

#ifdef ZTS
#define MULTI_HTTP_GET_G(v) TSRMG(multi_http_get_globals_id, zend_multi_http_get_globals *, v)
#else
#define MULTI_HTTP_GET_G(v) (multi_http_get_globals.v)
#endif

#endif	/* PHP_MULTI_HTTP_GET_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * indent-tabs-mode: t
 * End:
 */
