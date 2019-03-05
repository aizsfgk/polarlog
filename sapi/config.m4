dnl $Id$
dnl m4是一种宏语言
dnl config.m4 for extension tou


PHP_ARG_ENABLE(tou, whether to enable tou support,
dnl Make sure that the comment is aligned:
[  --enable-polarlog           Enable tou support])

if test "$PHP_POLARLOG" != "no"; then
  
  BUILD_PATH="/Users/zhangshifeng01/app/polarlog"
  ROOT_PATH=$BUILD_PATH


  AC_MSG_CHECKING($ROOT_PATH)

  if test -r $ROOT_PATH/include/polarlog.h; then
    AC_MSG_CHECKING([for polarlog include file found])
  else
    AC_MSG_CHECKING([for polarlog include file not found])
    AC_MSG_ERROR([please install polarlog files])
  fi

 
  dnl 添加头文件
  PHP_ADD_INCLUDE($ROOT_PATH/include)
  PHP_ADD_INCLUDE($ROOT_PATH/sapi)
  PHP_ADD_INCLUDE($ROOT_PATH)

  dnl 需要C++编译器
  PHP_REQUIRE_CXX()
  
  CXXFLAGS="-g -O0 -DDEBUG -shared -fPIC -rdynamic -finline-functions -pipe -std=c++11 $CPPFLAGS"
  LDFLAGS="-Bstatic $LDFLAGS"

  dnl 库的链接顺序，也会报找不到符号的错误
  dnl 依赖关系的顺序很重要
  dnl 

  PHP_ADD_LIBPATH($ROOT_PATH/lib, POLARLOG_SHARED_LIBADD)
  PHP_ADD_LIBRARY(polarlog, 1, POLARLOG_SHARED_LIBADD)

  PHP_ADD_LIBRARY(stdc++, 1, POLARLOG_SHARED_LIBADD)
  PHP_ADD_LIBRARY(pthread, 1, POLARLOG_SHARED_LIBADD)
  PHP_ADD_LIBRARY(dl, 1, POLARLOG_SHARED_LIBADD)


  
  AC_MSG_CHECKING([variables])


  AC_MSG_RESULT([PHP_POLARLOG=$PHP_POLARLOG POLARLOG_SHARED_LIBADD=$POLARLOG_SHARED_LIBADD])
  PHP_SUBST(POLARLOG_SHARED_LIBADD)
  PHP_NEW_EXTENSION(polarlog, [php_polarlog.cpp], $ext_shared)
fi
