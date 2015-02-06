dnl $Id: config.m4,v 1.1 2011/09/23 09:43:32 kevin Exp $
dnl config.m4 for extension multi_http_get

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(multi_http_get, for multi_http_get support,
dnl Make sure that the comment is aligned:
dnl [  --with-multi_http_get             Include multi_http_get support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(multi_http_get, whether to enable multi_http_get support,
Make sure that the comment is aligned:
[  --enable-multi_http_get           Enable multi_http_get support])

if test "$PHP_MULTI_HTTP_GET" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-multi_http_get -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/multi_http_get.h"  # you most likely want to change this
  dnl if test -r $PHP_MULTI_HTTP_GET/; then # path given as parameter
  dnl   MULTI_HTTP_GET_DIR=$PHP_MULTI_HTTP_GET
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for multi_http_get files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       MULTI_HTTP_GET_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$MULTI_HTTP_GET_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the multi_http_get distribution])
  dnl fi

  dnl # --with-multi_http_get -> add include path
  dnl PHP_ADD_INCLUDE($MULTI_HTTP_GET_DIR/include)

  dnl # --with-multi_http_get -> check for lib and symbol presence
  dnl LIBNAME=multi_http_get # you may want to change this
  dnl LIBSYMBOL=multi_http_get # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $MULTI_HTTP_GET_DIR/lib, MULTI_HTTP_GET_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_MULTI_HTTP_GETLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong multi_http_get lib version or lib not found])
  dnl ],[
  dnl   -L$MULTI_HTTP_GET_DIR/lib -lm -ldl
  dnl ])
  dnl
  dnl PHP_SUBST(MULTI_HTTP_GET_SHARED_LIBADD)

  PHP_NEW_EXTENSION(multi_http_get, multi_http_get.c, $ext_shared)
fi
