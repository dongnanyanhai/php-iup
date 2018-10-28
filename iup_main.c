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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_iup.h"

extern int le_iup_ihandle;
extern int le_iup_event;

extern HashTable *iup_events;
extern HashTable *iup_callback;

/* {{{ proto void IupOpen()
    */
PHP_FUNCTION(IupOpen)
{
    if (zend_parse_parameters_none() == FAILURE) {
        return;
    }

    IupOpen(NULL, NULL);

    // init array
    ALLOC_HASHTABLE(iup_events);
    zend_hash_init(iup_events,512,NULL,NULL,0);

    // 注册回调函数
    ALLOC_HASHTABLE(iup_callback);
    zend_hash_init(iup_callback,512,NULL,NULL,0);
    event_register_callback();

    RETURN_BOOL(1);
}
/* }}} */

/* {{{ proto void IupClose()
    */
PHP_FUNCTION(IupClose)
{
    if (zend_parse_parameters_none() == FAILURE) {
        return;
    }

    zend_hash_destroy(iup_events);
    zend_hash_destroy(iup_callback);

    IupClose();

    RETURN_BOOL(1);
}
/* }}} */

/* {{{ proto void IupMainLoop()
    */
PHP_FUNCTION(IupMainLoop)
{
    int i;
    if (zend_parse_parameters_none() == FAILURE) {
        return;
    }

    i = IupMainLoop();

    RETURN_LONG(i);
}
/* }}} */

/* {{{ proto void IupLoopStep()
    */
PHP_FUNCTION(IupLoopStep)
{
    int i;
    if (zend_parse_parameters_none() == FAILURE) {
        return;
    }

    i = IupLoopStep();

    RETURN_LONG(i);
}
/* }}} */

/* {{{ proto void IupLoopStepWait()
    */
PHP_FUNCTION(IupLoopStepWait)
{
    int i;
    if (zend_parse_parameters_none() == FAILURE) {
        return;
    }

    i = IupLoopStepWait();

    RETURN_LONG(i);
}
/* }}} */

/* {{{ proto void IupMainLoopLevel()
    */
PHP_FUNCTION(IupMainLoopLevel)
{
    int i;
    if (zend_parse_parameters_none() == FAILURE) {
        return;
    }

    i = IupMainLoopLevel();

    RETURN_LONG(i);
}
/* }}} */

/* {{{ proto void IupImageLibOpen()
    */
PHP_FUNCTION(IupImageLibOpen)
{
    if (zend_parse_parameters_none() == FAILURE) {
        return;
    }

    IupImageLibOpen();

    RETURN_BOOL(1);
}
/* }}} */

/* {{{ proto void IupFlush()
    */
PHP_FUNCTION(IupFlush)
{
    if (zend_parse_parameters_none() == FAILURE) {
        return;
    }

    IupFlush();

    RETURN_BOOL(1);
}
/* }}} */

/* {{{ proto void IupExitLoop()
    */
PHP_FUNCTION(IupExitLoop)
{
    if (zend_parse_parameters_none() == FAILURE) {
        return;
    }

    IupExitLoop();

    RETURN_BOOL(1);
}
/* }}} */

/* {{{ proto string IupRecordInput(string filename, int mode)
   ;
 */
PHP_FUNCTION(IupRecordInput)
{
    int argc = ZEND_NUM_ARGS();

    char *filename = NULL;
    size_t filename_len;

    zend_long mode;

    int i;

    if (zend_parse_parameters(argc, "sl", &filename, &filename_len,&mode) == FAILURE) {
        return;
    }

    i = IupRecordInput(filename,mode);

    RETURN_LONG(i);
}
/* }}} */

/* {{{ proto string IupPlayInput(string filename)
   ;
 */
PHP_FUNCTION(IupPlayInput)
{
    int argc = ZEND_NUM_ARGS();

    char *filename = NULL;
    size_t filename_len;

    int i;

    if (zend_parse_parameters(argc, "s", &filename, &filename_len) == FAILURE) {
        return;
    }

    i = IupPlayInput(filename);

    RETURN_LONG(i);
}
/* }}} */

/* {{{ proto void IupUpdate(resource ih)
   ;
 */
PHP_FUNCTION(IupUpdate)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih;

    if (zend_parse_parameters(argc TSRMLS_DC,"r",&ihandle_res) == FAILURE) {
        return;
    }

    ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    IupUpdate(ih);

    RETURN_BOOL(1);
    
}
/* }}} */

/* {{{ proto void IupUpdateChildren(resource ih)
   ;
 */
PHP_FUNCTION(IupUpdateChildren)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih;

    if (zend_parse_parameters(argc TSRMLS_DC,"r",&ihandle_res) == FAILURE) {
        return;
    }

    ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    IupUpdateChildren(ih);

    RETURN_BOOL(1);
    
}
/* }}} */

/* {{{ proto void IupRedraw(resource ih, int children)
   ;
 */
PHP_FUNCTION(IupRedraw)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih;

    zend_long children;

    if (zend_parse_parameters(argc TSRMLS_DC,"rl",&ihandle_res,&children) == FAILURE) {
        return;
    }

    ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    IupRedraw(ih,children);

    RETURN_BOOL(1);
    
}
/* }}} */

/* {{{ proto void IupRefresh(resource ih)
   ;
 */
PHP_FUNCTION(IupRefresh)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih;

    if (zend_parse_parameters(argc TSRMLS_DC,"r",&ihandle_res) == FAILURE) {
        return;
    }

    ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    IupRefresh(ih);

    RETURN_BOOL(1);
    
}
/* }}} */

/* {{{ proto void IupRefreshChildren(resource ih)
   ;
 */
PHP_FUNCTION(IupRefreshChildren)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih;

    if (zend_parse_parameters(argc TSRMLS_DC,"r",&ihandle_res) == FAILURE) {
        return;
    }

    ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    IupRefreshChildren(ih);

    RETURN_BOOL(1);
    
}
/* }}} */


/* {{{ proto string IupExecute(string filename, string parameters)
   ;
 */
PHP_FUNCTION(IupExecute)
{
    int argc = ZEND_NUM_ARGS();

    char *filename = NULL;
    size_t filename_len;

    char *parameters = NULL;
    size_t parameters_len;

    int i;

    if (zend_parse_parameters(argc, "ss!", &filename, &filename_len, &parameters, &parameters_len) == FAILURE){
        return;
    }

    i = IupExecute(filename,parameters);

    RETURN_LONG(i);
}
/* }}} */

/* {{{ proto string IupExecuteWait(string filename, string parameters)
   ;
 */
PHP_FUNCTION(IupExecuteWait)
{
    int argc = ZEND_NUM_ARGS();
    
    char *filename = NULL;
    size_t filename_len;

    char *parameters = NULL;
    size_t parameters_len;

    int i;

    if (zend_parse_parameters(argc, "ss!", &filename, &filename_len, &parameters, &parameters_len) == FAILURE){
        return;
    }

    i = IupExecuteWait(filename,parameters);

    RETURN_LONG(i);
}
/* }}} */

/* {{{ proto string IupHelp(string url)
   ;
 */
PHP_FUNCTION(IupHelp)
{
    int argc = ZEND_NUM_ARGS();
    
    char *url = NULL;
    size_t url_len;

    int i;

    if (zend_parse_parameters(argc, "s", &url, &url_len) == FAILURE){
        return;
    }

    i = IupHelp(url);

    RETURN_LONG(i);
}
/* }}} */


// void IupLog(const char* type, const char* format, ...);
// 待完善
/* {{{ proto string IupLog(string type, string format)
   ;
 */
PHP_FUNCTION(IupLog)
{
    int argc = ZEND_NUM_ARGS();
    
    char *type = NULL;
    size_t type_len;

    char *format = NULL;
    size_t format_len;

    if (zend_parse_parameters(argc, "ss", &type, &type_len, &format, &format_len) == FAILURE){
        return;
    }

    IupLog(type,format,NULL);

    RETURN_BOOL(1);
}
/* }}} */

/* {{{ proto string IupLoad(string filename)
   ;
 */
PHP_FUNCTION(IupLoad)
{
    int argc = ZEND_NUM_ARGS();
    char *filename = NULL;
    size_t filename_len;

    char * str;

    if (zend_parse_parameters(argc, "s", &filename, &filename_len) == FAILURE){
        return;
    }

    str = IupLoad(filename);

    RETURN_STRING(str);

}
/* }}} */

/* {{{ proto string IupLoadBuffer(string buffer)
   ;
 */
PHP_FUNCTION(IupLoadBuffer)
{
    int argc = ZEND_NUM_ARGS();
    char *buffer = NULL;
    size_t buffer_len;

    char * str;

    if (zend_parse_parameters(argc, "s", &buffer, &buffer_len) == FAILURE){
        return;
    }

    str = IupLoadBuffer(buffer);

    RETURN_STRING(str);

}
/* }}} */

/* {{{ proto string IupVersion()
   ;
 */
PHP_FUNCTION(IupVersion)
{
    char * str;

    if (zend_parse_parameters_none() == FAILURE) {
        return;
    }

    str = IupVersion();

    RETURN_STRING(str);

}
/* }}} */

/* {{{ proto string IupVersionDate()
   ;
 */
PHP_FUNCTION(IupVersionDate)
{
    char * str;

    if (zend_parse_parameters_none() == FAILURE) {
        return;
    }

    str = IupVersionDate();

    RETURN_STRING(str);

}
/* }}} */

/* {{{ proto int IupVersionNumber()
   ;
 */
PHP_FUNCTION(IupVersionNumber)
{
    long l;

    if (zend_parse_parameters_none() == FAILURE) {
        return;
    }

    l = IupVersionNumber();

    RETURN_LONG(l);

}
/* }}} */

/* {{{ proto string IupSetLanguage(string lng)
   ;
 */
PHP_FUNCTION(IupSetLanguage)
{
    int argc = ZEND_NUM_ARGS();
    char *lng = NULL;
    size_t lng_len;

    if (zend_parse_parameters(argc, "s", &lng, &lng_len) == FAILURE) {
        return;
    }
    
    IupSetLanguage(lng);

    RETURN_BOOL(1);
}
/* }}} */

/* {{{ proto string IupGetLanguage()
   ;
 */
PHP_FUNCTION(IupGetLanguage)
{
    char * str;
    if (zend_parse_parameters_none() == FAILURE) {
        return;
    }

    str = IupGetLanguage();
    
    RETURN_STRING(str);
}
/* }}} */

/* {{{ proto string IupSetLanguageString(string name, string str)
   ;
 */
PHP_FUNCTION(IupSetLanguageString)
{
    int argc = ZEND_NUM_ARGS();
    char *name = NULL;
    size_t name_len;

    char *str = NULL;
    size_t str_len;

    if (zend_parse_parameters(argc, "ss", &name, &name_len, &str, &str_len) == FAILURE) {
        return;
    }
    
    IupSetLanguageString(name,str);

    RETURN_BOOL(1);
}
/* }}} */

/* {{{ proto string IupStoreLanguageString(string name, string str)
   ;
 */
PHP_FUNCTION(IupStoreLanguageString)
{
    int argc = ZEND_NUM_ARGS();
    char *name = NULL;
    size_t name_len;

    char *str = NULL;
    size_t str_len;

    if (zend_parse_parameters(argc, "ss", &name, &name_len, &str, &str_len) == FAILURE) {
        return;
    }
    
    IupStoreLanguageString(name,str);

    RETURN_BOOL(1);
}
/* }}} */

/* {{{ proto string IupGetLanguageString(string name)
   ;
 */
PHP_FUNCTION(IupGetLanguageString)
{
    int argc = ZEND_NUM_ARGS();
    char *name = NULL;
    size_t name_len;

    char *str;

    if (zend_parse_parameters(argc, "s", &name, &name_len) == FAILURE) {
        return;
    }

    str = IupGetLanguageString(name);
    
    RETURN_STRING(str);
}
/* }}} */

/* {{{ proto string IupSetLanguagePack(resource ih)
   ;
 */
PHP_FUNCTION(IupSetLanguagePack)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih;

    if (zend_parse_parameters(argc TSRMLS_DC,"r",&ihandle_res) == FAILURE) {
        return;
    }

    ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    IupSetLanguagePack(ih);

    RETURN_BOOL(1);
    
}
/* }}} */

/* {{{ proto string IupDestroy(resource ih)
   ;
 */
PHP_FUNCTION(IupDestroy)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih;

    if (zend_parse_parameters(argc TSRMLS_DC,"r",&ihandle_res) == FAILURE) {
        return;
    }

    ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    IupDestroy(ih);

    RETURN_BOOL(1);
    
}
/* }}} */

/* {{{ proto string IupDetach(resource child)
   ;
 */
PHP_FUNCTION(IupDetach)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *child;

    if (zend_parse_parameters(argc TSRMLS_DC,"r",&ihandle_res) == FAILURE) {
        return;
    }

    child = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    IupDetach(child);

    RETURN_BOOL(1);
    
}
/* }}} */

/* {{{ proto resource IupAppend(resource ih, resource child)
   ;
 */
PHP_FUNCTION(IupAppend)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res_ih = NULL;
    zval *ihandle_res_child = NULL;

    Ihandle *ih, *child, *re;

    if (zend_parse_parameters(argc TSRMLS_DC,"rr",&ihandle_res_ih,&ihandle_res_child) == FAILURE) {
        return;
    }

    ih = zend_fetch_resource_ex(ihandle_res_ih,"iup-handle",le_iup_ihandle);
    child = zend_fetch_resource_ex(ihandle_res_child,"iup-handle",le_iup_ihandle);

    re = IupAppend(ih,child);

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}
/* }}} */

/* {{{ proto resource IupInsert(resource ih, resource ref_child, resource child)
   ;
 */
PHP_FUNCTION(IupInsert)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res_ih = NULL;
    zval *ihandle_res_ref_child = NULL;
    zval *ihandle_res_child = NULL;

    Ihandle *ih, *ref_child, *child, *re;

    if (zend_parse_parameters(argc TSRMLS_DC,"rrr",&ihandle_res_ih,&ihandle_res_ref_child,&ihandle_res_child) == FAILURE) {
        return;
    }

    ih = zend_fetch_resource_ex(ihandle_res_ih,"iup-handle",le_iup_ihandle);
    ref_child = zend_fetch_resource_ex(ihandle_res_ref_child,"iup-handle",le_iup_ihandle);
    child = zend_fetch_resource_ex(ihandle_res_child,"iup-handle",le_iup_ihandle);

    re = IupInsert(ih,ref_child,child);

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}
/* }}} */

/* {{{ proto resource IupGetChild(resource ih, int pos)
   ;
 */
PHP_FUNCTION(IupGetChild)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih, *re;

    zend_long pos;

    if (zend_parse_parameters(argc TSRMLS_DC,"rl",&ihandle_res,&pos) == FAILURE) {
        return;
    }

    ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    re = IupGetChild(ih,pos);

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
    
}
/* }}} */

/* {{{ proto int IupGetChildPos(resource ih, resource child)
   ;
 */
PHP_FUNCTION(IupGetChildPos)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res_ih = NULL;
    zval *ihandle_res_child = NULL;

    Ihandle *ih, *child;

    int i;

    if (zend_parse_parameters(argc TSRMLS_DC,"rr",&ihandle_res_ih,&ihandle_res_child) == FAILURE) {
        return;
    }

    ih = zend_fetch_resource_ex(ihandle_res_ih,"iup-handle",le_iup_ihandle);
    child = zend_fetch_resource_ex(ihandle_res_child,"iup-handle",le_iup_ihandle);

    i = IupGetChildPos(ih,child);

    RETURN_LONG(i);
}
/* }}} */

/* {{{ proto int IupGetChildCount(resource ih)
   ;
 */
PHP_FUNCTION(IupGetChildCount)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih;

    int i;

    if (zend_parse_parameters(argc TSRMLS_DC,"r",&ihandle_res) == FAILURE) {
        return;
    }

    ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    i = IupGetChildCount(ih);

    RETURN_LONG(i);
}
/* }}} */

/* {{{ proto resource IupGetNextChild(resource ih, resource child)
   ;
 */
PHP_FUNCTION(IupGetNextChild)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res_ih = NULL;
    zval *ihandle_res_child = NULL;

    Ihandle *ih, *child, *re;

    if (zend_parse_parameters(argc TSRMLS_DC,"rr",&ihandle_res_ih,&ihandle_res_child) == FAILURE) {
        return;
    }

    ih = zend_fetch_resource_ex(ihandle_res_ih,"iup-handle",le_iup_ihandle);
    child = zend_fetch_resource_ex(ihandle_res_child,"iup-handle",le_iup_ihandle);

    re = IupGetNextChild(ih,child);

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}
/* }}} */

/* {{{ proto resource IupGetBrother(resource ih)
   ;
 */
PHP_FUNCTION(IupGetBrother)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih, *re;

    if (zend_parse_parameters(argc TSRMLS_DC,"r",&ihandle_res) == FAILURE) {
        return;
    }

    ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    re = IupGetBrother(ih);

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
    
}
/* }}} */

/* {{{ proto resource IupGetParent(resource ih)
   ;
 */
PHP_FUNCTION(IupGetParent)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih, *re;

    if (zend_parse_parameters(argc TSRMLS_DC,"r",&ihandle_res) == FAILURE) {
        return;
    }

    ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    re = IupGetParent(ih);

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
    
}
/* }}} */

/* {{{ proto resource IupGetDialog(resource ih)
   ;
 */
PHP_FUNCTION(IupGetDialog)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih, *re;

    if (zend_parse_parameters(argc TSRMLS_DC,"r",&ihandle_res) == FAILURE) {
        return;
    }

    ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    re = IupGetDialog(ih);

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
    
}
/* }}} */

/* {{{ proto resource IupGetDialogChild(resource ih, string name)
   ;
 */
PHP_FUNCTION(IupGetDialogChild)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih, *re;

    char *name = NULL;
    size_t name_len;

    if (zend_parse_parameters(argc TSRMLS_DC,"rs",&ihandle_res,&name,&name_len) == FAILURE) {
        return;
    }

    ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    re = IupGetDialogChild(ih,name);

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
    
}
/* }}} */

/* {{{ proto resource IupReparent(resource ih, resource new_parent, resource ref_child)
   ;
 */
PHP_FUNCTION(IupReparent)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res_ih = NULL;
    zval *ihandle_res_new_parent = NULL;
    zval *ihandle_res_ref_child = NULL;

    Ihandle *ih, *new_parent, *ref_child;

    int i;

    if (zend_parse_parameters(argc TSRMLS_DC,"rrr",&ihandle_res_ih,&ihandle_res_new_parent,&ihandle_res_ref_child) == FAILURE) {
        return;
    }

    ih = zend_fetch_resource_ex(ihandle_res_ih,"iup-handle",le_iup_ihandle);
    new_parent = zend_fetch_resource_ex(ihandle_res_new_parent,"iup-handle",le_iup_ihandle);
    ref_child = zend_fetch_resource_ex(ihandle_res_ref_child,"iup-handle",le_iup_ihandle);

    i = IupReparent(ih,new_parent,ref_child);

    RETURN_LONG(i);
}
/* }}} */

/* {{{ proto string IupPopup(resource ih, int x, int y)
   ;
 */
PHP_FUNCTION(IupPopup)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih;

    zend_long x,y;

    int i;

    if (zend_parse_parameters(argc TSRMLS_DC,"rll",&ihandle_res,&x,&y) == FAILURE) {
        return;
    }

    ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    i = IupPopup(ih,x,y);

    RETURN_LONG(i);
}
/* }}} */

/* {{{ proto int IupShow(resource ih)
   ;
 */
PHP_FUNCTION(IupShow)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih;

    int i;

    if (zend_parse_parameters(argc TSRMLS_DC,"r",&ihandle_res) == FAILURE) {
        return;
    }

    ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    i = IupShow(ih);

    RETURN_LONG(i);
}
/* }}} */

/* {{{ proto string IupShowXY(resource ih, int x, int y)
   ;
 */
PHP_FUNCTION(IupShowXY)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih;

    zend_long x,y;

    int i;

    if (zend_parse_parameters(argc TSRMLS_DC,"rll",&ihandle_res,&x,&y) == FAILURE) {
        return;
    }

    ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    i = IupShowXY(ih,x,y);

    RETURN_LONG(i);
}
/* }}} */

/* {{{ proto int IupHide(resource ih)
   ;
 */
PHP_FUNCTION(IupHide)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih;

    int i;

    if (zend_parse_parameters(argc TSRMLS_DC,"r",&ihandle_res) == FAILURE) {
        return;
    }

    ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    i = IupHide(ih);

    RETURN_LONG(i);
}
/* }}} */

/* {{{ proto int IupMap(resource ih)
   ;
 */
PHP_FUNCTION(IupMap)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih;

    int i;

    if (zend_parse_parameters(argc TSRMLS_DC,"r",&ihandle_res) == FAILURE) {
        return;
    }

    ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    i = IupMap(ih);

    RETURN_LONG(i);
}
/* }}} */

/* {{{ proto int IupUnmap(resource ih)
   ;
 */
PHP_FUNCTION(IupUnmap)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih;

    if (zend_parse_parameters(argc TSRMLS_DC,"r",&ihandle_res) == FAILURE) {
        return;
    }

    ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    IupUnmap(ih);

    RETURN_BOOL(1);
}
/* }}} */


/* {{{ proto resource IupResetAttribute(resource ih, string name)
   ;
 */
PHP_FUNCTION(IupResetAttribute)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih;

    char *name = NULL;
    size_t name_len;

    if (zend_parse_parameters(argc TSRMLS_DC,"rs",&ihandle_res,&name,&name_len) == FAILURE) {
        return;
    }

    ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    IupResetAttribute(ih,name);

    RETURN_BOOL(1);
    
}
/* }}} */

/* {{{ proto resource IupGetAllAttributes(resource ih, string name, int n)
   ;
 */
PHP_FUNCTION(IupGetAllAttributes)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih;

    char *name = NULL;
    size_t name_len;

    zend_long n;

    int i;

    if (zend_parse_parameters(argc TSRMLS_DC,"rsl",&ihandle_res,&name,&name_len,&n) == FAILURE) {
        return;
    }

    ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    i = IupGetAllAttributes(ih,&name,n);

    RETURN_LONG(i);
}
/* }}} */

/* {{{ proto resource IupSetAtt(string handle_name, resource ih, string name, string value)
   ;
 */
PHP_FUNCTION(IupSetAtt)
{
    int argc = ZEND_NUM_ARGS();

    char *handle_name = NULL;
    size_t handle_name_len;


    zval *ihandle_res = NULL;

    Ihandle *ih,*re;

    char *name = NULL;
    size_t name_len;

    char *value = NULL;
    size_t value_len;

    if (zend_parse_parameters(argc TSRMLS_DC,"srss",&handle_name,&handle_name_len,&ihandle_res,&name,&name_len,&value,&value_len) == FAILURE) {
        return;
    }

    ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    re = IupSetAtt(handle_name,ih,name,value);

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}
/* }}} */

/* {{{ proto resource IupSetAttributes(resource ih, string str)
   ;
 */
PHP_FUNCTION(IupSetAttributes)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih, *re;

    char *str = NULL;
    size_t str_len;

    if (zend_parse_parameters(argc TSRMLS_DC,"r!s",&ihandle_res,&str,&str_len) == FAILURE) {
        return;
    }

    if(ihandle_res != NULL){
        ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);
    }else{
        ih = NULL;
    }

    re = IupSetAttributes(ih,str);

    if(re != NULL){
        RETURN_RES(zend_register_resource(re, le_iup_ihandle));
    }

    RETURN_NULL();
}
/* }}} */

/* {{{ proto resource IupGetAttributes(resource ih)
   ;
 */
PHP_FUNCTION(IupGetAttributes)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih;

    char * str;

    if (zend_parse_parameters(argc TSRMLS_DC,"r!",&ihandle_res) == FAILURE) {
        return;
    }

    if(ihandle_res != NULL){
        ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);
    }else{
        ih = NULL;
    }

    str = IupGetAttributes(ih);

    if(str != NULL){
        RETURN_STRING(str);
    }

    RETURN_NULL();
}
/* }}} */


/* {{{ proto resource IupSetAttribute(resource ih, string name, string value)
   ;
 */
PHP_FUNCTION(IupSetAttribute)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih;

    char *name = NULL;
    size_t name_len;

    char *value = NULL;
    size_t value_len;

    if (zend_parse_parameters(argc TSRMLS_DC,"r!ss!",&ihandle_res,&name,&name_len,&value,&value_len) == FAILURE) {
        return;
    }

    if(ihandle_res != NULL){
        ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);
    }else{
        ih = NULL;
    }

    IupSetAttribute(ih,name,value);

    RETURN_BOOL(1);
}
/* }}} */

/* {{{ proto resource IupSetStrAttribute(resource ih, string name, string value)
   ;
 */
PHP_FUNCTION(IupSetStrAttribute)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih;

    char *name = NULL;
    size_t name_len;

    char *value = NULL;
    size_t value_len;

    if (zend_parse_parameters(argc TSRMLS_DC,"rss",&ihandle_res,&name,&name_len,&value,&value_len) == FAILURE) {
        return;
    }

    ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    IupSetStrAttribute(ih,name,value);

    RETURN_BOOL(1);
}
/* }}} */

/* {{{ proto resource IupSetStrf(resource ih, string name, string format)
   ;
 */
PHP_FUNCTION(IupSetStrf)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih;

    char *name = NULL;
    size_t name_len;

    char *format = NULL;
    size_t format_len;

    if (zend_parse_parameters(argc TSRMLS_DC,"rss",&ihandle_res,&name,&name_len,&format,&format_len) == FAILURE) {
        return;
    }

    ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    IupSetStrf(ih,name,format,NULL);

    RETURN_BOOL(1);
}
/* }}} */

/* {{{ proto resource IupSetInt(resource ih, string name, int value)
   ;
 */
PHP_FUNCTION(IupSetInt)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih;

    char *name = NULL;
    size_t name_len;

    zend_long value;

    if (zend_parse_parameters(argc TSRMLS_DC,"rsl",&ihandle_res,&name,&name_len,&value) == FAILURE) {
        return;
    }

    ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    IupSetInt(ih,name,value);

    RETURN_BOOL(1);
}
/* }}} */

/* {{{ proto resource IupSetFloat(resource ih, string name, float value)
   ;
 */
PHP_FUNCTION(IupSetFloat)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih;

    char *name = NULL;
    size_t name_len;

    double value;

    if (zend_parse_parameters(argc TSRMLS_DC,"rsd",&ihandle_res,&name,&name_len,&value) == FAILURE) {
        return;
    }

    ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    IupSetFloat(ih,name,(float)value);

    RETURN_BOOL(1);
}
/* }}} */

/* {{{ proto resource IupSetDouble(resource ih, string name, double value)
   ;
 */
PHP_FUNCTION(IupSetDouble)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih;

    char *name = NULL;
    size_t name_len;

    double value;

    if (zend_parse_parameters(argc TSRMLS_DC,"rsd",&ihandle_res,&name,&name_len,&value) == FAILURE) {
        return;
    }

    ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    IupSetDouble(ih,name,value);

    RETURN_BOOL(1);
}
/* }}} */


/* {{{ proto resource IupSetRGB(resource ih, string name, int r, int g, int b)
   ;
 */
PHP_FUNCTION(IupSetRGB)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih;

    char *name = NULL;
    size_t name_len;

    zend_long r,g,b;

    unsigned char rr,gg,bb;

    if (zend_parse_parameters(argc TSRMLS_DC,"rslll",&ihandle_res,&name,&name_len,&r,&g,&b) == FAILURE) {
        return;
    }

    ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    rr = (unsigned char)r;
    gg = (unsigned char)g;
    bb = (unsigned char)b;

    IupSetRGB(ih,name,rr,gg,bb);

    RETURN_BOOL(1);

    // php_error(E_ERROR, "IupSetRGB: this function requested is not supported");
}
/* }}} */

/* {{{ proto resource IupGetAttribute(resource ih, string name)
   ;
 */
PHP_FUNCTION(IupGetAttribute)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih;

    char *name = NULL;
    size_t name_len;

    char * str;

    if (zend_parse_parameters(argc TSRMLS_DC,"r!s",&ihandle_res,&name,&name_len) == FAILURE) {
        return;
    }

    if(ihandle_res != NULL){
        ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);
    }else{
        ih = NULL;
    }

    str = IupGetAttribute(ih,name);

    if(str != NULL){
        RETURN_STRING(str);
    }

    RETURN_NULL();
}
/* }}} */

/* {{{ proto resource IupGetInt(resource ih, string name)
   ;
 */
PHP_FUNCTION(IupGetInt)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih;

    char *name = NULL;
    size_t name_len;

    int i;

    if (zend_parse_parameters(argc TSRMLS_DC,"rs",&ihandle_res,&name,&name_len) == FAILURE) {
        return;
    }

    ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    i = IupGetInt(ih,name);

    RETURN_LONG(i);
}
/* }}} */

/* {{{ proto resource IupGetInt2(resource ih, string name)
   ;
 */
PHP_FUNCTION(IupGetInt2)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih;

    char *name = NULL;
    size_t name_len;

    int i;

    if (zend_parse_parameters(argc TSRMLS_DC,"rs",&ihandle_res,&name,&name_len) == FAILURE) {
        return;
    }

    ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    i = IupGetInt2(ih,name);

    RETURN_LONG(i);
}
/* }}} */

/* {{{ proto string IupGetIntInt(resource ih, string name, int i1, int i2)
   ;
 */
PHP_FUNCTION(IupGetIntInt)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih;

    char *name = NULL;
    size_t name_len;

    zend_long i1,i2;

    int i,p1,p2;

    if (zend_parse_parameters(argc TSRMLS_DC,"rsll",&ihandle_res,&name,&name_len,&i1,&i2) == FAILURE) {
        return;
    }

    ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    p1 = (int)i1;
    p2 = (int)i2;

    i = IupGetIntInt(ih,name,&p1,&p2);

    RETURN_LONG(i);
}
/* }}} */

/* {{{ proto resource IupGetFloat(resource ih, string name)
   ;
 */
PHP_FUNCTION(IupGetFloat)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih;

    char *name = NULL;
    size_t name_len;

    double d;

    if (zend_parse_parameters(argc TSRMLS_DC,"rs",&ihandle_res,&name,&name_len) == FAILURE) {
        return;
    }

    ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    d = IupGetFloat(ih,name);

    RETURN_DOUBLE(d);
}
/* }}} */

/* {{{ proto resource IupGetDouble(resource ih, string name)
   ;
 */
PHP_FUNCTION(IupGetDouble)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih;

    char *name = NULL;
    size_t name_len;

    double d;

    if (zend_parse_parameters(argc TSRMLS_DC,"rs",&ihandle_res,&name,&name_len) == FAILURE) {
        return;
    }

    ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    d = IupGetDouble(ih,name);

    RETURN_DOUBLE(d);
}
/* }}} */

/* {{{ proto resource IupGetRGB(resource ih, string name)
   ;
 */
PHP_FUNCTION(IupGetRGB)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih;

    char *name = NULL;
    size_t name_len;

    unsigned char r, g, b;
    zval rr,gg,bb;

    HashTable *arr = NULL;

    if (zend_parse_parameters(argc TSRMLS_DC,"rs",&ihandle_res,&name,&name_len) == FAILURE) {
        return;
    }

    ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    IupGetRGB(ih,name,&r,&g,&b);

    ALLOC_HASHTABLE(arr);
    zend_hash_init(arr,8,NULL,NULL,0);

    ZVAL_LONG(&rr,r);
    ZVAL_LONG(&gg,g);
    ZVAL_LONG(&bb,b);

    zend_hash_str_add_new(arr,"r",2,&rr);
    zend_hash_str_add_new(arr,"g",2,&gg);
    zend_hash_str_add_new(arr,"b",2,&bb);

    RETURN_ARR(arr);
}
/* }}} */

/* {{{ proto resource IupSetAttributeId(resource ih, string name, int id, string value)
   ;
 */
PHP_FUNCTION(IupSetAttributeId)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih;

    char *name = NULL;
    size_t name_len;

    zend_long id;

    char *value = NULL;
    size_t value_len;

    if (zend_parse_parameters(argc TSRMLS_DC,"r!sls!",&ihandle_res,&name,&name_len,&id,&value,&value_len) == FAILURE) {
        return;
    }

    if(ihandle_res != NULL){
        ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);
    }else{
        ih = NULL;
    }

    IupSetAttributeId(ih,name,id,value);

    RETURN_BOOL(1);
}
/* }}} */

/* {{{ proto resource IupSetStrAttributeId(resource ih, string name, int id, string value)
   ;
 */
PHP_FUNCTION(IupSetStrAttributeId)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih;

    char *name = NULL;
    size_t name_len;

    zend_long id;

    char *value = NULL;
    size_t value_len;

    if (zend_parse_parameters(argc TSRMLS_DC,"rsls",&ihandle_res,&name,&name_len,&id,&value,&value_len) == FAILURE) {
        return;
    }

    ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    IupSetStrAttributeId(ih,name,id,value);

    RETURN_BOOL(1);
}
/* }}} */

/* {{{ proto resource IupSetStrfId(resource ih, string name, int id, string format)
   ;
 */
PHP_FUNCTION(IupSetStrfId)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih;

    char *name = NULL;
    size_t name_len;

    zend_long id;

    char *format = NULL;
    size_t format_len;

    if (zend_parse_parameters(argc TSRMLS_DC,"rsls",&ihandle_res,&name,&name_len,&id,&format,&format_len) == FAILURE) {
        return;
    }

    ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    IupSetStrfId(ih,name,id,format,NULL);

    RETURN_BOOL(1);
}
/* }}} */

/* {{{ proto resource IupSetIntId(resource ih, string name, int id, int value)
   ;
 */
PHP_FUNCTION(IupSetIntId)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih;

    char *name = NULL;
    size_t name_len;

    zend_long id;

    zend_long value;

    if (zend_parse_parameters(argc TSRMLS_DC,"rsll",&ihandle_res,&name,&name_len,&id,&value) == FAILURE) {
        return;
    }

    ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    IupSetIntId(ih,name,id,value);

    RETURN_BOOL(1);
}
/* }}} */

/* {{{ proto resource IupSetFloatId(resource ih, string name, int id, float value)
   ;
 */
PHP_FUNCTION(IupSetFloatId)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih;

    char *name = NULL;
    size_t name_len;

    zend_long id;

    double value;

    if (zend_parse_parameters(argc TSRMLS_DC,"rsld",&ihandle_res,&name,&name_len,&id,&value) == FAILURE) {
        return;
    }

    ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    IupSetFloatId(ih,name,id,(float)value);

    RETURN_BOOL(1);
}
/* }}} */

/* {{{ proto resource IupSetDoubleId(resource ih, string name, int id, double value)
   ;
 */
PHP_FUNCTION(IupSetDoubleId)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih;

    char *name = NULL;
    size_t name_len;

    zend_long id;

    double value;

    if (zend_parse_parameters(argc TSRMLS_DC,"rsld",&ihandle_res,&name,&name_len,&id,&value) == FAILURE) {
        return;
    }

    ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    IupSetDoubleId(ih,name,id,value);

    RETURN_BOOL(1);
}
/* }}} */


/* {{{ proto resource IupSetRGBId(resource ih, string name, int id, int r, int g, int b)
   ;
 */
PHP_FUNCTION(IupSetRGBId)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih;

    char *name = NULL;
    size_t name_len;

    zend_long id;

    zend_long r;
    zend_long g;
    zend_long b;

    unsigned char rr;
    unsigned char gg;
    unsigned char bb;

    if (zend_parse_parameters(argc TSRMLS_DC,"rsllll",&ihandle_res,&name,&name_len,&id,&r,&g,&b) == FAILURE) {
        return;
    }

    ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    rr = (unsigned char)r;
    gg = (unsigned char)g;
    bb = (unsigned char)b;

    IupSetRGBId(ih,name,id,rr,gg,bb);

    RETURN_BOOL(1);

    // php_error(E_ERROR, "IupSetRGB: this function requested is not supported");
}
/* }}} */

/* {{{ proto resource IupGetAttributeId(resource ih, string name, int id)
   ;
 */
PHP_FUNCTION(IupGetAttributeId)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih;

    char *name = NULL;
    size_t name_len;

    zend_long id;

    char * str;

    if (zend_parse_parameters(argc TSRMLS_DC,"r!sl",&ihandle_res,&name,&name_len,&id) == FAILURE) {
        return;
    }

    if(ihandle_res != NULL){
        ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);
    }else{
        ih = NULL;
    }

    str = IupGetAttributeId(ih,name,id);

    if(str != NULL){
        RETURN_STRING(str);
    }

    RETURN_NULL();
}
/* }}} */

/* {{{ proto resource IupGetIntId(resource ih, string name, int id)
   ;
 */
PHP_FUNCTION(IupGetIntId)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih;

    char *name = NULL;
    size_t name_len;

    zend_long id;

    int i;

    if (zend_parse_parameters(argc TSRMLS_DC,"rsl",&ihandle_res,&name,&name_len,&id) == FAILURE) {
        return;
    }

    ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    i = IupGetIntId(ih,name,id);

    RETURN_LONG(i);
}
/* }}} */

/* {{{ proto resource IupGetFloatId(resource ih, string name, int id)
   ;
 */
PHP_FUNCTION(IupGetFloatId)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih;

    char *name = NULL;
    size_t name_len;

    zend_long id;

    double d;

    if (zend_parse_parameters(argc TSRMLS_DC,"rsl",&ihandle_res,&name,&name_len,&id) == FAILURE) {
        return;
    }

    ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    d = IupGetFloatId(ih,name,id);

    RETURN_DOUBLE(d);
}
/* }}} */

/* {{{ proto resource IupGetDoubleId(resource ih, string name, int id)
   ;
 */
PHP_FUNCTION(IupGetDoubleId)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih;

    char *name = NULL;
    size_t name_len;

    zend_long id;

    double d;

    if (zend_parse_parameters(argc TSRMLS_DC,"rsl",&ihandle_res,&name,&name_len,&id) == FAILURE) {
        return;
    }

    ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    d = IupGetDoubleId(ih,name,id);

    RETURN_DOUBLE(d);
}
/* }}} */

/* {{{ proto resource IupGetRGBId(resource ih, string name,int id)
   ;
 */
PHP_FUNCTION(IupGetRGBId)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih;

    char *name = NULL;
    size_t name_len;

    zend_long id;

    unsigned char r, g, b;
    zval rr,gg,bb;

    HashTable *arr = NULL;

    if (zend_parse_parameters(argc TSRMLS_DC,"rsl",&ihandle_res,&name,&name_len,&id) == FAILURE) {
        return;
    }

    ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    IupGetRGBId(ih,name,id,&r,&g,&b);

    ALLOC_HASHTABLE(arr);
    zend_hash_init(arr,8,NULL,NULL,0);

    ZVAL_LONG(&rr,r);
    ZVAL_LONG(&gg,g);
    ZVAL_LONG(&bb,b);

    zend_hash_str_add_new(arr,"r",2,&rr);
    zend_hash_str_add_new(arr,"g",2,&gg);
    zend_hash_str_add_new(arr,"b",2,&bb);

    RETURN_ARR(arr);
}
/* }}} */

/* {{{ proto resource IupSetAttributeId2(resource ih, string name, int lin, int col, string value)
   ;
 */
PHP_FUNCTION(IupSetAttributeId2)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih;

    char *name = NULL;
    size_t name_len;

    zend_long lin,col;

    char *value = NULL;
    size_t value_len;

    if (zend_parse_parameters(argc TSRMLS_DC,"r!slls!",&ihandle_res,&name,&name_len,&lin,&col,&value,&value_len) == FAILURE) {
        return;
    }

    if(ihandle_res != NULL){
        ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);
    }else{
        ih = NULL;
    }

    IupSetAttributeId2(ih,name,lin,col,value);

    RETURN_BOOL(1);
}
/* }}} */

/* {{{ proto resource IupSetStrAttributeId2(resource ih, string name, int lin, int col, string value)
   ;
 */
PHP_FUNCTION(IupSetStrAttributeId2)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih;

    char *name = NULL;
    size_t name_len;

    zend_long lin,col;

    char *value = NULL;
    size_t value_len;

    if (zend_parse_parameters(argc TSRMLS_DC,"rslls",&ihandle_res,&name,&name_len,&lin,&col,&value,&value_len) == FAILURE) {
        return;
    }

    ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    IupSetStrAttributeId2(ih,name,lin,col,value);

    RETURN_BOOL(1);
}
/* }}} */

/* {{{ proto resource IupSetStrfId2(resource ih, string name, int lin, int col, string format)
   ;
 */
PHP_FUNCTION(IupSetStrfId2)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih;

    char *name = NULL;
    size_t name_len;

    zend_long lin,col;

    char *format = NULL;
    size_t format_len;

    if (zend_parse_parameters(argc TSRMLS_DC,"rslls",&ihandle_res,&name,&name_len,&lin,&col,&format,&format_len) == FAILURE) {
        return;
    }

    ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    IupSetStrfId2(ih,name,lin,col,format,NULL);

    RETURN_BOOL(1);
}
/* }}} */

/* {{{ proto resource IupSetIntId2(resource ih, string name, int lin, int col, int value)
   ;
 */
PHP_FUNCTION(IupSetIntId2)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih;

    char *name = NULL;
    size_t name_len;

    zend_long lin,col;

    zend_long value;

    if (zend_parse_parameters(argc TSRMLS_DC,"rslll",&ihandle_res,&name,&name_len,&lin,&col,&value) == FAILURE) {
        return;
    }

    ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    IupSetIntId2(ih,name,lin,col,value);

    RETURN_BOOL(1);
}
/* }}} */

/* {{{ proto resource IupSetFloatId2(resource ih, string name, int lin, int col, float value)
   ;
 */
PHP_FUNCTION(IupSetFloatId2)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih;

    char *name = NULL;
    size_t name_len;

    zend_long lin,col;

    double value;

    if (zend_parse_parameters(argc TSRMLS_DC,"rslld",&ihandle_res,&name,&name_len,&lin,&col,&value) == FAILURE) {
        return;
    }

    ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    IupSetFloatId2(ih,name,lin,col,(float)value);

    RETURN_BOOL(1);
}
/* }}} */

/* {{{ proto resource IupSetDoubleId2(resource ih, string name, int lin, int col, double value)
   ;
 */
PHP_FUNCTION(IupSetDoubleId2)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih;

    char *name = NULL;
    size_t name_len;

    zend_long lin,col;

    double value;

    if (zend_parse_parameters(argc TSRMLS_DC,"rslld",&ihandle_res,&name,&name_len,&lin,&col,&value) == FAILURE) {
        return;
    }

    ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    IupSetDoubleId2(ih,name,lin,col,value);

    RETURN_BOOL(1);
}
/* }}} */


/* {{{ proto resource IupSetRGBId2(resource ih, string name, int lin, int col, int r, int g, int b)
   ;
 */
PHP_FUNCTION(IupSetRGBId2)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih;

    char *name = NULL;
    size_t name_len;

    zend_long lin,col;

    zend_long r;
    zend_long g;
    zend_long b;

    unsigned char rr;
    unsigned char gg;
    unsigned char bb;

    if (zend_parse_parameters(argc TSRMLS_DC,"rslllll",&ihandle_res,&name,&name_len,&lin,&col,&r,&g,&b) == FAILURE) {
        return;
    }

    ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    rr = (unsigned char)r;
    gg = (unsigned char)g;
    bb = (unsigned char)b;

    IupSetRGBId2(ih,name,lin,col,rr,gg,bb);

    RETURN_BOOL(1);

    // php_error(E_ERROR, "IupSetRGB: this function requested is not supported");
}
/* }}} */

/* {{{ proto resource IupGetAttributeId2(resource ih, string name, int lin, int col)
   ;
 */
PHP_FUNCTION(IupGetAttributeId2)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih;

    char *name = NULL;
    size_t name_len;

    zend_long lin, col;

    char * str;

    if (zend_parse_parameters(argc TSRMLS_DC,"r!sll",&ihandle_res,&name,&name_len,&lin,&col) == FAILURE) {
        return;
    }

    if(ihandle_res != NULL){
        ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);
    }else{
        ih = NULL;
    }

    str = IupGetAttributeId2(ih,name,lin,col);

    if(str != NULL){
        RETURN_STRING(str);
    }

    RETURN_NULL();
}
/* }}} */

/* {{{ proto resource IupGetIntId2(resource ih, string name, int lin, int col)
   ;
 */
PHP_FUNCTION(IupGetIntId2)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih;

    char *name = NULL;
    size_t name_len;

    zend_long lin, col;

    int i;

    if (zend_parse_parameters(argc TSRMLS_DC,"rsll",&ihandle_res,&name,&name_len,&lin,&col) == FAILURE) {
        return;
    }

    ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    i = IupGetIntId2(ih,name,lin,col);

    RETURN_LONG(i);
}
/* }}} */

/* {{{ proto resource IupGetFloatId2(resource ih, string name, int lin, int col)
   ;
 */
PHP_FUNCTION(IupGetFloatId2)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih;

    char *name = NULL;
    size_t name_len;

    zend_long lin, col;

    double d;

    if (zend_parse_parameters(argc TSRMLS_DC,"rsll",&ihandle_res,&name,&name_len,&lin,&col) == FAILURE) {
        return;
    }

    ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    d = IupGetFloatId2(ih,name,lin,col);

    RETURN_DOUBLE(d);
}
/* }}} */

/* {{{ proto resource IupGetDoubleId2(resource ih, string name, int lin, int col)
   ;
 */
PHP_FUNCTION(IupGetDoubleId2)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih;

    char *name = NULL;
    size_t name_len;

    zend_long lin, col;

    double d;

    if (zend_parse_parameters(argc TSRMLS_DC,"rsll",&ihandle_res,&name,&name_len,&lin,&col) == FAILURE) {
        return;
    }

    ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    d = IupGetDoubleId2(ih,name,lin,col);

    RETURN_DOUBLE(d);
}
/* }}} */

/* {{{ proto resource IupGetRGBId2(resource ih, string name, int lin, int col)
   ;
 */
PHP_FUNCTION(IupGetRGBId2)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih;

    char *name = NULL;
    size_t name_len;

    zend_long lin, col;

    unsigned char r, g, b;
    zval rr,gg,bb;

    HashTable *arr = NULL;

    if (zend_parse_parameters(argc TSRMLS_DC,"rsll",&ihandle_res,&name,&name_len,&lin,&col) == FAILURE) {
        return;
    }

    ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    IupGetRGBId2(ih,name,lin,col,&r,&g,&b);

    ALLOC_HASHTABLE(arr);
    zend_hash_init(arr,8,NULL,NULL,0);

    ZVAL_LONG(&rr,r);
    ZVAL_LONG(&gg,g);
    ZVAL_LONG(&bb,b);

    zend_hash_str_add_new(arr,"r",2,&rr);
    zend_hash_str_add_new(arr,"g",2,&gg);
    zend_hash_str_add_new(arr,"b",2,&bb);

    RETURN_ARR(arr);
}
/* }}} */

/* {{{ proto void IupSetGlobal(string name, string value)
   ;
 */
PHP_FUNCTION(IupSetGlobal)
{
    int argc = ZEND_NUM_ARGS();
    char *name = NULL;
    size_t name_len;

    char *value = NULL;
    size_t value_len;

    if (zend_parse_parameters(argc, "ss", &name, &name_len, &value, &value_len) == FAILURE) {
        return;
    }

    IupSetGlobal(name,value);

    RETURN_BOOL(1);
}
/* }}} */

/* {{{ proto void IupSetStrGlobal(string name, string value)
   ;
 */
PHP_FUNCTION(IupSetStrGlobal)
{
    int argc = ZEND_NUM_ARGS();
    char *name = NULL;
    size_t name_len;

    char *value = NULL;
    size_t value_len;

    if (zend_parse_parameters(argc, "ss", &name, &name_len, &value, &value_len) == FAILURE) {
        return;
    }

    IupSetStrGlobal(name,value);

    RETURN_BOOL(1);
}
/* }}} */

/* {{{ proto string IupGetGlobal(string name)
   ;
 */
PHP_FUNCTION(IupGetGlobal)
{
    int argc = ZEND_NUM_ARGS();
    char *name = NULL;
    size_t name_len;

    char *str;

    if (zend_parse_parameters(argc, "s", &name, &name_len) == FAILURE) {
        return;
    }

    str = IupGetGlobal(name);

    RETURN_STRING(str);
}
/* }}} */

/* {{{ proto resource IupSetFocus(resource ih)
   ;
 */
PHP_FUNCTION(IupSetFocus)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih,*re;

    if (zend_parse_parameters(argc TSRMLS_DC,"r",&ihandle_res) == FAILURE) {
        return;
    }

    ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    re = IupSetFocus(ih);

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}
/* }}} */

/* {{{ proto resource IupGetFocus()
   ;
 */
PHP_FUNCTION(IupGetFocus)
{
    int argc = ZEND_NUM_ARGS();

    if (zend_parse_parameters_none() == FAILURE) {
        return;
    }

    Ihandle *re;

    re = IupGetFocus();

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}
/* }}} */

/* {{{ proto resource IupPreviousField(resource ih)
   ;
 */
PHP_FUNCTION(IupPreviousField)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih,*re;

    if (zend_parse_parameters(argc TSRMLS_DC,"r",&ihandle_res) == FAILURE) {
        return;
    }

    ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    re = IupPreviousField(ih);

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}
/* }}} */

/* {{{ proto resource IupNextField(resource ih)
   ;
 */
PHP_FUNCTION(IupNextField)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih,*re;

    if (zend_parse_parameters(argc TSRMLS_DC,"r",&ihandle_res) == FAILURE) {
        return;
    }

    ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    re = IupNextField(ih);

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}
/* }}} */

/* {{{ proto string IupSetCallback(resource ih, string name, string fun_name)
   ;
 */
PHP_FUNCTION(IupSetCallback)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih;

    char *event_name = NULL;
    size_t event_name_len;

    zend_fcall_info callable;
    zend_fcall_info_cache call_cache;

    zend_fcall_info * call_p;

    call_p = (zend_fcall_info *)malloc(sizeof(zend_fcall_info));

    intptr_t ih_p_int;

    char event_key_str[100];

    zend_string * event_key;

    zval event_val;

    zval * event_val_old;
    zend_fcall_info * callable_old;

    if (zend_parse_parameters(argc TSRMLS_DC,"rsf",&ihandle_res, &event_name, &event_name_len, &callable, &call_cache) == FAILURE) {
        return;
    }

    *call_p = callable;

    ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    ih_p_int = (intptr_t)ih;

    sprintf(event_key_str,"IUP_%s_%"SCNiPTR,event_name,ih_p_int);

    event_key = zend_string_init(event_key_str, strlen(event_key_str), 0);

    // 判断事件数组中是否存有相同事件id
    if(!zend_hash_exists(iup_events,event_key)){
        // 绑定事件
        event_set_callback(ih, event_name);
    }else{
        // 释放旧事件方法占用的内容
        event_val_old = zend_hash_find(iup_events,event_key);

        if(event_val_old != NULL){

            callable_old = zend_fetch_resource_ex(event_val_old,"iup-event",le_iup_event);

            if(callable_old != NULL){
                free(callable_old);
            }
        }

    }

    ZVAL_RES(&event_val,zend_register_resource(call_p, le_iup_event));
    zend_hash_update(iup_events, event_key, &event_val);

    RETURN_BOOL(1);
    
}
/* }}} */

/* {{{ proto int IupGetCallback()
   ;
 */
PHP_FUNCTION(IupGetCallback)
{

    php_error(E_WARNING, "IupGetCallback1: not yet implemented");

}
/* }}} */

/* {{{ proto int IupSetCallbacks()
   ;
 */
PHP_FUNCTION(IupSetCallbacks)
{

    php_error(E_WARNING, "IupSetCallbacks: not yet implemented");

}
/* }}} */

/* {{{ proto int IupGetFunction()
   ;
 */
PHP_FUNCTION(IupGetFunction)
{

    php_error(E_WARNING, "IupGetFunction1: not yet implemented");

}
/* }}} */

/* {{{ proto int IupSetFunction()
   ;
 */
PHP_FUNCTION(IupSetFunction)
{

    php_error(E_WARNING, "IupSetFunction1: not yet implemented");

}
/* }}} */

/* {{{ proto resource IupGetHandle(string name)
   ;
 */
PHP_FUNCTION(IupGetHandle)
{
    int argc = ZEND_NUM_ARGS();

    char *name = NULL;
    size_t name_len;

    Ihandle *re;

    if (zend_parse_parameters(argc, "s", &name, &name_len) == FAILURE) {
        return;
    }

    re = IupGetHandle(name);

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));

}
/* }}} */

/* {{{ proto resource IupSetHandle(string name, resource ih)
   ;
 */
PHP_FUNCTION(IupSetHandle)
{
    int argc = ZEND_NUM_ARGS();

    char *name = NULL;
    size_t name_len;

    zval *ihandle_res = NULL;
    
    Ihandle *ih, *re;

    if (zend_parse_parameters(argc, "sr!", &name, &name_len,&ihandle_res) == FAILURE) {
        return;
    }

    ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    re = IupSetHandle(name,ih);

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));

}
/* }}} */

/* {{{ proto int IupGetAllNames(string name, int n)
   ;
 */
PHP_FUNCTION(IupGetAllNames)
{
    int argc = ZEND_NUM_ARGS();

    char *name = NULL;
    size_t name_len;

    zend_long n;

    int i;

    if (zend_parse_parameters(argc, "sl", &name, &name_len,&n) == FAILURE) {
        return;
    }

    i = IupGetAllNames(&name,n);

    RETURN_LONG(i);
}
/* }}} */

/* {{{ proto int IupGetAllDialogs(string name, int n)
   ;
 */
PHP_FUNCTION(IupGetAllDialogs)
{
    int argc = ZEND_NUM_ARGS();

    char *name = NULL;
    size_t name_len;

    zend_long n;

    int i;

    if (zend_parse_parameters(argc, "sl", &name, &name_len,&n) == FAILURE) {
        return;
    }

    i = IupGetAllDialogs(&name,n);

    RETURN_LONG(i);
}
/* }}} */

/* {{{ proto string IupGetName(resource ih)
   ;
 */
PHP_FUNCTION(IupGetName)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih;

    char * str;

    if (zend_parse_parameters(argc TSRMLS_DC,"r",&ihandle_res) == FAILURE) {
        return;
    }

    ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    str = IupGetName(ih);

    RETURN_STRING(str);
}
/* }}} */


/* {{{ proto resource IupSetAttributeHandle(resource ih, string name, resource ih_named)
   ;
 */
PHP_FUNCTION(IupSetAttributeHandle)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;
    zval *ihandle_res_named = NULL;

    char *name = NULL;
    size_t name_len;

    Ihandle *ih,*ih_named;

    if (zend_parse_parameters(argc TSRMLS_DC,"r!sr!",&ihandle_res,&name,&name_len,&ihandle_res_named) == FAILURE) {
        return;
    }

    if(ihandle_res != NULL){
        ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);
    }else{
        ih = NULL;
    }

    ih_named = zend_fetch_resource_ex(ihandle_res_named,"iup-handle",le_iup_ihandle);

    IupSetAttributeHandle(ih,name,ih_named);

    RETURN_BOOL(1);
}
/* }}} */

/* {{{ proto resource IupGetAttributeHandle(resource ih, string name)
   ;
 */
PHP_FUNCTION(IupGetAttributeHandle)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    char *name = NULL;
    size_t name_len;

    Ihandle *ih,*re;

    if (zend_parse_parameters(argc TSRMLS_DC,"r!s",&ihandle_res,&name,&name_len) == FAILURE) {
        return;
    }

    if(ihandle_res != NULL){
        ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);
    }else{
        ih = NULL;
    }

    re = IupGetAttributeHandle(ih,name);

    if(re != NULL){
        RETURN_RES(zend_register_resource(re, le_iup_ihandle));
    }

    RETURN_NULL();
}
/* }}} */

/* {{{ proto resource IupSetAttributeHandleId(resource ih, string name, int id, resource ih_named)
   ;
 */
PHP_FUNCTION(IupSetAttributeHandleId)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;
    zval *ihandle_res_named = NULL;

    char *name = NULL;
    size_t name_len;

    zend_long id;

    Ihandle *ih,*ih_named;

    if (zend_parse_parameters(argc TSRMLS_DC,"r!slr!",&ihandle_res,&name,&name_len,&id,&ihandle_res_named) == FAILURE) {
        return;
    }

    if(ihandle_res != NULL){
        ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);
    }else{
        ih = NULL;
    }

    ih_named = zend_fetch_resource_ex(ihandle_res_named,"iup-handle",le_iup_ihandle);

    IupSetAttributeHandleId(ih,name,id,ih_named);

    RETURN_BOOL(1);
}
/* }}} */

/* {{{ proto resource IupGetAttributeHandleId(resource ih, string name, int id)
   ;
 */
PHP_FUNCTION(IupGetAttributeHandleId)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    char *name = NULL;
    size_t name_len;

    zend_long id;

    Ihandle *ih,*re;

    if (zend_parse_parameters(argc TSRMLS_DC,"r!sl",&ihandle_res,&name,&name_len,&id) == FAILURE) {
        return;
    }

    if(ihandle_res != NULL){
        ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);
    }else{
        ih = NULL;
    }

    re = IupGetAttributeHandleId(ih,name,id);

    if(re != NULL){
        RETURN_RES(zend_register_resource(re, le_iup_ihandle));
    }

    RETURN_NULL();

}
/* }}} */


/* {{{ proto resource IupSetAttributeHandleId2(resource ih, string name, int lin, int col, resource ih_named)
   ;
 */
PHP_FUNCTION(IupSetAttributeHandleId2)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;
    zval *ihandle_res_named = NULL;

    char *name = NULL;
    size_t name_len;

    zend_long lin,col;

    Ihandle *ih,*ih_named;

    if (zend_parse_parameters(argc TSRMLS_DC,"r!sllr!",&ihandle_res,&name,&name_len,&lin,&col,&ihandle_res_named) == FAILURE) {
        return;
    }

    if(ihandle_res != NULL){
        ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);
    }else{
        ih = NULL;
    }

    ih_named = zend_fetch_resource_ex(ihandle_res_named,"iup-handle",le_iup_ihandle);

    IupSetAttributeHandleId2(ih,name,lin,col,ih_named);

    RETURN_BOOL(1);
}
/* }}} */


/* {{{ proto resource IupGetAttributeHandleId2(resource ih, string name, int lin, int col)
   ;
 */
PHP_FUNCTION(IupGetAttributeHandleId2)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    char *name = NULL;
    size_t name_len;

    zend_long lin,col;

    Ihandle *ih,*re;

    if (zend_parse_parameters(argc TSRMLS_DC,"r!sll",&ihandle_res,&name,&name_len,&lin,&col) == FAILURE) {
        return;
    }

    if(ihandle_res != NULL){
        ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);
    }else{
        ih = NULL;
    }

    re = IupGetAttributeHandleId2(ih,name,lin,col);

    if(re != NULL){
        RETURN_RES(zend_register_resource(re, le_iup_ihandle));
    }

    RETURN_NULL();
}
/* }}} */
/* {{{ proto string IupGetClassName(resource ih)
   ;
 */
PHP_FUNCTION(IupGetClassName)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih;

    char * str;

    if (zend_parse_parameters(argc TSRMLS_DC,"r",&ihandle_res) == FAILURE) {
        return;
    }

    ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    str = IupGetClassName(ih);

    RETURN_STRING(str);
}
/* }}} */

/* {{{ proto string IupGetClassType(resource ih)
   ;
 */
PHP_FUNCTION(IupGetClassType)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih;

    char * str;

    if (zend_parse_parameters(argc TSRMLS_DC,"r",&ihandle_res) == FAILURE) {
        return;
    }

    ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    str = IupGetClassType(ih);

    RETURN_STRING(str);
}
/* }}} */

/* {{{ proto int IupGetAllClasses(string name, int n)
   ;
 */
PHP_FUNCTION(IupGetAllClasses)
{
    int argc = ZEND_NUM_ARGS();

    char *name = NULL;
    size_t name_len;

    zend_long n;

    int i;

    if (zend_parse_parameters(argc, "sl", &name, &name_len,&n) == FAILURE) {
        return;
    }

    i = IupGetAllClasses(&name,n);

    RETURN_LONG(i);
}
/* }}} */

/* {{{ proto int IupGetClassAttributes(string classname, string name, int n)
   ;
 */
PHP_FUNCTION(IupGetClassAttributes)
{
    int argc = ZEND_NUM_ARGS();

    char *classname = NULL;
    size_t classname_len;

    char *name = NULL;
    size_t name_len;

    zend_long n;

    int i;

    if (zend_parse_parameters(argc, "ssl", &classname, &classname_len,&name, &name_len,&n) == FAILURE) {
        return;
    }

    i = IupGetClassAttributes(classname,&name,n);

    RETURN_LONG(i);
}
/* }}} */

/* {{{ proto int IupGetClassCallbacks(string classname, string name, int n)
   ;
 */
PHP_FUNCTION(IupGetClassCallbacks)
{
    int argc = ZEND_NUM_ARGS();

    char *classname = NULL;
    size_t classname_len;

    char *name = NULL;
    size_t name_len;

    zend_long n;

    int i;

    if (zend_parse_parameters(argc, "ssl", &classname, &classname_len,&name, &name_len,&n) == FAILURE) {
        return;
    }

    i = IupGetClassCallbacks(classname,&name,n);

    RETURN_LONG(i);
}
/* }}} */

/* {{{ proto string IupSaveClassAttributes(resource ih)
   ;
 */
PHP_FUNCTION(IupSaveClassAttributes)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih;

    if (zend_parse_parameters(argc TSRMLS_DC,"r",&ihandle_res) == FAILURE) {
        return;
    }

    ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    IupSaveClassAttributes(ih);

    RETURN_BOOL(1);
}
/* }}} */

/* {{{ proto string IupCopyClassAttributes(resource ih, resource det_ih)
   ;
 */
PHP_FUNCTION(IupCopyClassAttributes)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;
    zval *ihandle_res_det = NULL;

    Ihandle *ih,*dst_ih;

    if (zend_parse_parameters(argc TSRMLS_DC,"rr",&ihandle_res,&ihandle_res_det) == FAILURE) {
        return;
    }

    ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);
    dst_ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    IupCopyClassAttributes(ih,dst_ih);

    RETURN_BOOL(1);
}
/* }}} */


/* {{{ proto string IupSetClassDefaultAttribute(string classname, string name, string value)
   ;
 */
PHP_FUNCTION(IupSetClassDefaultAttribute)
{
    int argc = ZEND_NUM_ARGS();
    char *classname = NULL;
    size_t classname_len;

    char *name = NULL;
    size_t name_len;

    char *value = NULL;
    size_t value_len;
    if (zend_parse_parameters(argc, "sss", &classname, &classname_len,&name, &name_len,&value, &value_len) == FAILURE) {
        return;
    }

    IupSetClassDefaultAttribute(classname,name,value);

    RETURN_BOOL(1);
}
/* }}} */

/* {{{ proto int IupClassMatch(resource ih, string classname)
   ;
 */
PHP_FUNCTION(IupClassMatch)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    char *classname = NULL;
    size_t classname_len;

    Ihandle *ih;

    int i;

    if (zend_parse_parameters(argc TSRMLS_DC,"rs",&ihandle_res,&classname,&classname_len) == FAILURE) {
        return;
    }

    ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    i = IupClassMatch(ih,classname);

    RETURN_LONG(i);
}
/* }}} */

/* {{{ proto int IupCreate()
   ;
 */
PHP_FUNCTION(IupCreatek)
{

    php_error(E_WARNING, "IupCreate: not yet implemented");

}
/* }}} */

/* {{{ proto int IupCreatev()
   ;
 */
PHP_FUNCTION(IupCreatev)
{

    php_error(E_WARNING, "IupCreatev: not yet implemented");

}
/* }}} */

/* {{{ proto int IupCreatep()
   ;
 */
PHP_FUNCTION(IupCreatep)
{

    php_error(E_WARNING, "IupCreatep: not yet implemented");

}
/* }}} */

/* {{{ proto resource IupFill()
   ;
 */
PHP_FUNCTION(IupFill)
{
    Ihandle *re;

    if (zend_parse_parameters_none() == FAILURE) {
        return;
    }

    re = IupFill();

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}
/* }}} */

/* {{{ proto resource IupSpace()
   ;
 */
PHP_FUNCTION(IupSpace)
{
    Ihandle *re;

    if (zend_parse_parameters_none() == FAILURE) {
        return;
    }

    re = IupSpace();

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}
/* }}} */

/* {{{ proto resource IupRadio(resource child)
   ;
 */
PHP_FUNCTION(IupRadio)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *child,*re;

    if (zend_parse_parameters(argc TSRMLS_DC,"r!",&ihandle_res) == FAILURE) {
        return;
    }

    child = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    re = IupRadio(child);

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}
/* }}} */

/* {{{ proto resource IupVbox(resource child)
   ;
 */
PHP_FUNCTION(IupVbox)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *child,*re;

    if (zend_parse_parameters(argc TSRMLS_DC,"r!",&ihandle_res) == FAILURE) {
        return;
    }

    child = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    re = IupVbox(child,NULL);

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}
/* }}} */


/* {{{ proto resource IupVboxv(resource children)
   ;
 */
PHP_FUNCTION(IupVboxv)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *children,*re;

    if (zend_parse_parameters(argc TSRMLS_DC,"r!",&ihandle_res) == FAILURE) {
        return;
    }

    children = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    re = IupVboxv(&children);

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}
/* }}} */


/* {{{ proto resource IupZbox(resource child)
   ;
 */
PHP_FUNCTION(IupZbox)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *child,*re;

    if (zend_parse_parameters(argc TSRMLS_DC,"r!",&ihandle_res) == FAILURE) {
        return;
    }

    child = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    re = IupZbox(child,NULL);

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}
/* }}} */


/* {{{ proto resource IupZboxv(resource children)
   ;
 */
PHP_FUNCTION(IupZboxv)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *children,*re;

    if (zend_parse_parameters(argc TSRMLS_DC,"r!",&ihandle_res) == FAILURE) {
        return;
    }

    children = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    re = IupZboxv(&children);

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}
/* }}} */


/* {{{ proto resource IupHbox(resource child)
   ;
 */
PHP_FUNCTION(IupHbox)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *child,*re;

    if (zend_parse_parameters(argc TSRMLS_DC,"r!",&ihandle_res) == FAILURE) {
        return;
    }

    child = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    re = IupHbox(child,NULL);

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}
/* }}} */


/* {{{ proto resource IupHboxv(resource children)
   ;
 */
PHP_FUNCTION(IupHboxv)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *children,*re;

    if (zend_parse_parameters(argc TSRMLS_DC,"r!",&ihandle_res) == FAILURE) {
        return;
    }

    children = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    re = IupHboxv(&children);

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}
/* }}} */

/* {{{ proto resource IupNormalizer(resource ih_first)
   ;
 */
PHP_FUNCTION(IupNormalizer)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih_first,*re;

    if (zend_parse_parameters(argc TSRMLS_DC,"r!",&ihandle_res) == FAILURE) {
        return;
    }

    ih_first = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    re = IupNormalizer(ih_first,NULL);

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}
/* }}} */


/* {{{ proto resource IupNormalizerv(resource ih_list)
   ;
 */
PHP_FUNCTION(IupNormalizerv)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih_list,*re;

    if (zend_parse_parameters(argc TSRMLS_DC,"r!",&ihandle_res) == FAILURE) {
        return;
    }

    ih_list = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    re = IupNormalizerv(&ih_list);

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}
/* }}} */

/* {{{ proto resource IupCbox(resource child)
   ;
 */
PHP_FUNCTION(IupCbox)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *child,*re;

    if (zend_parse_parameters(argc TSRMLS_DC,"r!",&ihandle_res) == FAILURE) {
        return;
    }

    child = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    re = IupCbox(child,NULL);

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}
/* }}} */

/* {{{ proto resource IupCboxv(resource children)
   ;
 */
PHP_FUNCTION(IupCboxv)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *children,*re;

    if (zend_parse_parameters(argc TSRMLS_DC,"r!",&ihandle_res) == FAILURE) {
        return;
    }

    children = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    re = IupCboxv(&children);

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}
/* }}} */

/* {{{ proto resource IupSbox(resource child)
   ;
 */
PHP_FUNCTION(IupSbox)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *child,*re;

    if (zend_parse_parameters(argc TSRMLS_DC,"r!",&ihandle_res) == FAILURE) {
        return;
    }

    child = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    re = IupSbox(child);

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}
/* }}} */

/* {{{ proto resource IupSplit(resource child1, resource child2)
   ;
 */
PHP_FUNCTION(IupSplit)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res_child1 = NULL;
    zval *ihandle_res_child2 = NULL;

    Ihandle *child1,*child2,*re;

    if (zend_parse_parameters(argc TSRMLS_DC,"r!r!",&ihandle_res_child1,&ihandle_res_child2) == FAILURE) {
        return;
    }

    child1 = zend_fetch_resource_ex(ihandle_res_child1,"iup-handle",le_iup_ihandle);
    child2 = zend_fetch_resource_ex(ihandle_res_child2,"iup-handle",le_iup_ihandle);

    re = IupSplit(child1,child2);

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}
/* }}} */

/* {{{ proto resource IupScrollBox(resource child)
   ;
 */
PHP_FUNCTION(IupScrollBox)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *child,*re;

    if (zend_parse_parameters(argc TSRMLS_DC,"r!",&ihandle_res) == FAILURE) {
        return;
    }

    child = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    re = IupScrollBox(child);

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}
/* }}} */

/* {{{ proto resource IupFlatScrollBox(resource child)
   ;
 */
PHP_FUNCTION(IupFlatScrollBox)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *child,*re;

    if (zend_parse_parameters(argc TSRMLS_DC,"r!",&ihandle_res) == FAILURE) {
        return;
    }

    child = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    re = IupFlatScrollBox(child);

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}
/* }}} */

/* {{{ proto resource IupGridBox(resource child)
   ;
 */
PHP_FUNCTION(IupGridBox)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *child,*re;

    if (zend_parse_parameters(argc TSRMLS_DC,"r!",&ihandle_res) == FAILURE) {
        return;
    }

    child = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    re = IupGridBox(child,NULL);

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}
/* }}} */

/* {{{ proto resource IupGridBoxv(resource children)
   ;
 */
PHP_FUNCTION(IupGridBoxv)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *children,*re;

    if (zend_parse_parameters(argc TSRMLS_DC,"r!",&ihandle_res) == FAILURE) {
        return;
    }

    children = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    re = IupGridBoxv(&children);

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}
/* }}} */

/* {{{ proto resource IupExpander(resource child)
   ;
 */
PHP_FUNCTION(IupExpander)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *child,*re;

    if (zend_parse_parameters(argc TSRMLS_DC,"r!",&ihandle_res) == FAILURE) {
        return;
    }

    child = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    re = IupExpander(child);

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}
/* }}} */

/* {{{ proto resource IupDetachBox(resource child)
   ;
 */
PHP_FUNCTION(IupDetachBox)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *child,*re;

    if (zend_parse_parameters(argc TSRMLS_DC,"r!",&ihandle_res) == FAILURE) {
        return;
    }

    child = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    re = IupDetachBox(child);

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}
/* }}} */

/* {{{ proto resource IupBackgroundBox(resource child)
   ;
 */
PHP_FUNCTION(IupBackgroundBox)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *child,*re;

    if (zend_parse_parameters(argc TSRMLS_DC,"r!",&ihandle_res) == FAILURE) {
        return;
    }

    child = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    re = IupBackgroundBox(child);

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}
/* }}} */

/* {{{ proto resource IupFrame(resource child)
   ;
 */
PHP_FUNCTION(IupFrame)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *child,*re;

    if (zend_parse_parameters(argc TSRMLS_DC,"r!",&ihandle_res) == FAILURE) {
        return;
    }

    child = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    re = IupFrame(child);

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}
/* }}} */

/* {{{ proto resource IupFlatFrame(resource child)
   ;
 */
PHP_FUNCTION(IupFlatFrame)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *child,*re;

    if (zend_parse_parameters(argc TSRMLS_DC,"r!",&ihandle_res) == FAILURE) {
        return;
    }

    child = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    re = IupFlatFrame(child);

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}
/* }}} */

/* {{{ proto int IupImage()
   ;
 */
PHP_FUNCTION(IupImage)
{

    php_error(E_WARNING, "IupImage: not yet implemented");

}
/* }}} */

/* {{{ proto int IupImageRGB()
   ;
 */
PHP_FUNCTION(IupImageRGB)
{

    php_error(E_WARNING, "IupImageRGB: not yet implemented");

}
/* }}} */

/* {{{ proto int IupImageRGBA()
   ;
 */
PHP_FUNCTION(IupImageRGBA)
{

    php_error(E_WARNING, "IupImageRGBA: not yet implemented");

}
/* }}} */

/* {{{ proto string IupItem(string name, string action)
   ;
 */
PHP_FUNCTION(IupItem)
{
    int argc = ZEND_NUM_ARGS();

    char *name = NULL;
    size_t name_len;

    char *action = NULL;
    size_t action_len;

    Ihandle *re;

    if (zend_parse_parameters(argc, "s!s!", &name, &name_len, &action, &action_len) == FAILURE) {
        return;
    }

    re = IupItem(name,action);

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}
/* }}} */

/* {{{ proto string IupSubmenu(string name, string child)
   ;
 */
PHP_FUNCTION(IupSubmenu)
{
    int argc = ZEND_NUM_ARGS();

    char *name = NULL;
    size_t name_len;

    zval *ihandle_res = NULL;

    Ihandle *child,*re;

    if (zend_parse_parameters(argc TSRMLS_DC,"s!r!", &name, &name_len, &ihandle_res) == FAILURE) {
        return;
    }

    child = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    re = IupSubmenu(name,child);

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}
/* }}} */

/* {{{ proto string IupSeparator()
   ;
*/
PHP_FUNCTION(IupSeparator)
{
    Ihandle *re;
    if (zend_parse_parameters_none() == FAILURE) {
        return;
    }

    re = IupSeparator();

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}
/* }}} */

/* {{{ proto resource IupMenu(resource child)
   ;
 */
PHP_FUNCTION(IupMenu)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *child,*re;

    if (zend_parse_parameters(argc TSRMLS_DC,"r!",&ihandle_res) == FAILURE) {
        return;
    }

    child = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    re = IupMenu(child,NULL);

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}
/* }}} */

/* {{{ proto resource IupMenuv(resource children)
   ;
 */
PHP_FUNCTION(IupMenuv)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *children,*re;

    if (zend_parse_parameters(argc TSRMLS_DC,"r",&ihandle_res) == FAILURE) {
        return;
    }

    children = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    re = IupMenuv(&children);

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}
/* }}} */

/* {{{ proto string IupButton(string title, string action)
   ;
 */
PHP_FUNCTION(IupButton)
{
    int argc = ZEND_NUM_ARGS();

    char *title = NULL;
    size_t title_len;

    char *action = NULL;
    size_t action_len;

    Ihandle *re;

    if (zend_parse_parameters(argc, "s!s!", &title, &title_len, &action, &action_len) == FAILURE) {
        return;
    }

    re = IupButton(title,action);

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}
/* }}} */

/* {{{ proto string IupFlatButton(string title)
   ;
 */
PHP_FUNCTION(IupFlatButton)
{
    int argc = ZEND_NUM_ARGS();

    char *title = NULL;
    size_t title_len;

    Ihandle *re;

    if (zend_parse_parameters(argc, "s!", &title, &title_len) == FAILURE) {
        return;
    }

    re = IupFlatButton(title);

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}
/* }}} */

/* {{{ proto string IupFlatToggle(string title)
   ;
 */
PHP_FUNCTION(IupFlatToggle)
{
    int argc = ZEND_NUM_ARGS();

    char *title = NULL;
    size_t title_len;

    Ihandle *re;

    if (zend_parse_parameters(argc, "s!", &title, &title_len) == FAILURE) {
        return;
    }

    re = IupFlatToggle(title);

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}
/* }}} */

/* {{{ proto resource IupDropButton(resource dropchild)
   ;
 */
PHP_FUNCTION(IupDropButton)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *dropchild,*re;

    if (zend_parse_parameters(argc TSRMLS_DC,"r!",&ihandle_res) == FAILURE) {
        return;
    }

    dropchild = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    re = IupDropButton(dropchild);

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}
/* }}} */

/* {{{ proto string IupFlatLabel(string title)
   ;
 */
PHP_FUNCTION(IupFlatLabel)
{
    int argc = ZEND_NUM_ARGS();

    char *title = NULL;
    size_t title_len;

    Ihandle *re;

    if (zend_parse_parameters(argc, "s!", &title, &title_len) == FAILURE) {
        return;
    }

    re = IupFlatLabel(title);

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}
/* }}} */

/* {{{ proto string IupFlatSeparator()
   ;
*/
PHP_FUNCTION(IupFlatSeparator)
{
    Ihandle *re;
    if (zend_parse_parameters_none() == FAILURE) {
        return;
    }

    re = IupFlatSeparator();

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}
/* }}} */

/* {{{ proto string IupCanvas(string action)
   ;
 */
PHP_FUNCTION(IupCanvas)
{
    int argc = ZEND_NUM_ARGS();

    char *action = NULL;
    size_t action_len;

    Ihandle *re;

    if (zend_parse_parameters(argc, "s!", &action, &action_len) == FAILURE) {
        return;
    }

    re = IupCanvas(action);

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}
/* }}} */

/* {{{ proto resource IupDialog(resource child)
   ;
 */
PHP_FUNCTION(IupDialog)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *child,*re;

    if (zend_parse_parameters(argc TSRMLS_DC,"r!",&ihandle_res) == FAILURE) {
        return;
    }

    child = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    re = IupDialog(child);

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}
/* }}} */

/* {{{ proto string IupUser()
   ;
*/
PHP_FUNCTION(IupUser)
{
    Ihandle *re;
    if (zend_parse_parameters_none() == FAILURE) {
        return;
    }

    re = IupUser();

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}
/* }}} */

/* {{{ proto string IupLabel(string title)
   ;
 */
PHP_FUNCTION(IupLabel)
{
    int argc = ZEND_NUM_ARGS();

    char *title = NULL;
    size_t title_len;

    Ihandle *re;

    if (zend_parse_parameters(argc, "s!", &title, &title_len) == FAILURE) {
        return;
    }

    re = IupLabel(title);

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}
/* }}} */


/* {{{ proto string IupList(string action)
   ;
 */
PHP_FUNCTION(IupList)
{
    int argc = ZEND_NUM_ARGS();

    char *action = NULL;
    size_t action_len;

    Ihandle *re;

    if (zend_parse_parameters(argc, "s!", &action, &action_len) == FAILURE) {
        return;
    }

    re = IupList(action);

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}
/* }}} */

/* {{{ proto string IupText(string action)
   ;
 */
PHP_FUNCTION(IupText)
{
    int argc = ZEND_NUM_ARGS();

    char *action = NULL;
    size_t action_len;

    Ihandle *re;

    if (zend_parse_parameters(argc, "s!", &action, &action_len) == FAILURE) {
        return;
    }

    re = IupText(action);

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}
/* }}} */

/* {{{ proto string IupMultiLine(string action)
   ;
 */
PHP_FUNCTION(IupMultiLine)
{
    int argc = ZEND_NUM_ARGS();

    char *action = NULL;
    size_t action_len;

    Ihandle *re;

    if (zend_parse_parameters(argc, "s", &action, &action_len) == FAILURE) {
        return;
    }

    re = IupMultiLine(action);

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}
/* }}} */

/* {{{ proto string IupToggle(string title, string action)
   ;
 */
PHP_FUNCTION(IupToggle)
{
    int argc = ZEND_NUM_ARGS();

    char *title = NULL;
    size_t title_len;

    char *action = NULL;
    size_t action_len;

    Ihandle *re;

    if (zend_parse_parameters(argc, "s!s!", &title, &title_len, &action, &action_len) == FAILURE) {
        return;
    }

    re = IupToggle(title,action);

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}
/* }}} */

/* {{{ proto string IupTimer()
   ;
*/
PHP_FUNCTION(IupTimer)
{
    Ihandle *re;
    if (zend_parse_parameters_none() == FAILURE) {
        return;
    }

    re = IupTimer();

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}
/* }}} */


/* {{{ proto string IupClipboard()
   ;
*/
PHP_FUNCTION(IupClipboard)
{
    Ihandle *re;
    if (zend_parse_parameters_none() == FAILURE) {
        return;
    }

    re = IupClipboard();

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}
/* }}} */


/* {{{ proto string IupProgressBar()
   ;
*/
PHP_FUNCTION(IupProgressBar)
{
    Ihandle *re;
    if (zend_parse_parameters_none() == FAILURE) {
        return;
    }

    re = IupProgressBar();

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}
/* }}} */

/* {{{ proto string IupVal(string type)
   ;
 */
PHP_FUNCTION(IupVal)
{
    int argc = ZEND_NUM_ARGS();

    char *type = NULL;
    size_t type_len;

    Ihandle *re;

    if (zend_parse_parameters(argc, "s!", &type, &type_len) == FAILURE) {
        return;
    }

    re = IupVal(type);

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}
/* }}} */

/* {{{ proto resource IupTabs(resource child)
   ;
 */
PHP_FUNCTION(IupTabs)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *child,*re;

    if (zend_parse_parameters(argc TSRMLS_DC,"r!",&ihandle_res) == FAILURE) {
        return;
    }

    child = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    re = IupTabs(child,NULL);

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}
/* }}} */

/* {{{ proto resource IupTabsv(resource children)
   ;
 */
PHP_FUNCTION(IupTabsv)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *children,*re;

    if (zend_parse_parameters(argc TSRMLS_DC,"r!",&ihandle_res) == FAILURE) {
        return;
    }

    children = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    re = IupTabsv(&children);

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}
/* }}} */

/* {{{ proto resource IupFlatTabs(resource first)
   ;
 */
PHP_FUNCTION(IupFlatTabs)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *first,*re;

    if (zend_parse_parameters(argc TSRMLS_DC,"r!",&ihandle_res) == FAILURE) {
        return;
    }

    first = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    re = IupFlatTabs(first,NULL);

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}
/* }}} */

/* {{{ proto resource IupFlatTabsv(resource children)
   ;
 */
PHP_FUNCTION(IupFlatTabsv)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *children,*re;

    if (zend_parse_parameters(argc TSRMLS_DC,"r!",&ihandle_res) == FAILURE) {
        return;
    }

    children = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    re = IupFlatTabsv(&children);

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}
/* }}} */

/* {{{ proto string IupTree()
   ;
*/
PHP_FUNCTION(IupTree)
{
    Ihandle *re;
    if (zend_parse_parameters_none() == FAILURE) {
        return;
    }

    re = IupTree();

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}
/* }}} */

/* {{{ proto string IupLink(string url, string title)
   ;
 */
PHP_FUNCTION(IupLink)
{
    int argc = ZEND_NUM_ARGS();

    char *url = NULL;
    size_t url_len;

    char *title = NULL;
    size_t title_len;

    Ihandle *re;

    if (zend_parse_parameters(argc, "s!s!", &url, &url_len, &title, &title_len) == FAILURE) {
        return;
    }

    re = IupLink(url,title);

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}
/* }}} */

/* {{{ proto resource IupAnimatedLabel(resource animation)
   ;
 */
PHP_FUNCTION(IupAnimatedLabel)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *animation,*re;

    if (zend_parse_parameters(argc TSRMLS_DC,"r!",&ihandle_res) == FAILURE) {
        return;
    }

    animation = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    re = IupAnimatedLabel(animation);

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}
/* }}} */

/* {{{ proto string IupDatePick()
   ;
*/
PHP_FUNCTION(IupDatePick)
{
    Ihandle *re;
    if (zend_parse_parameters_none() == FAILURE) {
        return;
    }

    re = IupDatePick();

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}
/* }}} */


/* {{{ proto string IupCalendar()
   ;
*/
PHP_FUNCTION(IupCalendar)
{
    Ihandle *re;
    if (zend_parse_parameters_none() == FAILURE) {
        return;
    }

    re = IupCalendar();

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}
/* }}} */

/* {{{ proto string IupColorbar()
   ;
*/
PHP_FUNCTION(IupColorbar)
{
    Ihandle *re;
    if (zend_parse_parameters_none() == FAILURE) {
        return;
    }

    re = IupColorbar();

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}
/* }}} */

/* {{{ proto string IupGauge()
   ;
*/
PHP_FUNCTION(IupGauge)
{
    Ihandle *re;
    if (zend_parse_parameters_none() == FAILURE) {
        return;
    }

    re = IupGauge();

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}
/* }}} */

/* {{{ proto string IupDial(string type)
   ;
 */
PHP_FUNCTION(IupDial)
{
    int argc = ZEND_NUM_ARGS();

    char *type = NULL;
    size_t type_len;

    Ihandle *re;

    if (zend_parse_parameters(argc, "s!", &type, &type_len) == FAILURE) {
        return;
    }

    re = IupDial(type);

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}
/* }}} */

/* {{{ proto string IupColorBrowser()
   ;
*/
PHP_FUNCTION(IupColorBrowser)
{
    Ihandle *re;
    if (zend_parse_parameters_none() == FAILURE) {
        return;
    }

    re = IupColorBrowser();

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}
/* }}} */

/* {{{ proto string IupSpin()
   ;
*/
PHP_FUNCTION(IupSpin)
{
    Ihandle *re;
    if (zend_parse_parameters_none() == FAILURE) {
        return;
    }

    re = IupSpin();

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}
/* }}} */

/* {{{ proto resource IupSpinbox(resource child)
   ;
 */
PHP_FUNCTION(IupSpinbox)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *child,*re;

    if (zend_parse_parameters(argc TSRMLS_DC,"r",&ihandle_res) == FAILURE) {
        return;
    }

    child = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    re = IupSpinbox(child);

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}
/* }}} */


/* {{{ proto string IupStringCompare(string str1, string str2, int casesensitive, int lexicographic)
   ;
 */
PHP_FUNCTION(IupStringCompare)
{
    int argc = ZEND_NUM_ARGS();

    char *str1 = NULL;
    size_t str1_len;

    char *str2 = NULL;
    size_t str2_len;

    zend_long casesensitive,lexicographic;

    int i;

    if (zend_parse_parameters(argc, "ssll", &str1, &str1_len, &str2, &str2_len, &casesensitive, &lexicographic) == FAILURE) {
        return;
    }

    i = IupStringCompare(str1,str2,casesensitive,lexicographic);

    RETURN_LONG(i);
}
/* }}} */

/* {{{ proto resource IupSaveImageAsText(resource ih, string file_name, string format, string name)
   ;
 */
PHP_FUNCTION(IupSaveImageAsText)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih;

    char *file_name = NULL;
    size_t file_name_len;

    char *format = NULL;
    size_t format_len;

    char *name = NULL;
    size_t name_len;

    int i;

    if (zend_parse_parameters(argc TSRMLS_DC,"rsss",&ihandle_res,&file_name, &file_name_len, &format, &format_len, &name, &name_len) == FAILURE) {
        return;
    }

    ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    i = IupSaveImageAsText(ih,file_name,format,name);

    RETURN_LONG(i);
    
}
/* }}} */

/* {{{ proto resource IupTextConvertLinColToPos(resource ih, int lin, int col)
   ;
 */
PHP_FUNCTION(IupTextConvertLinColToPos)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih;

    zend_long lin,col;

    int pos;

    if (zend_parse_parameters(argc TSRMLS_DC,"rll",&ihandle_res,&lin,&col) == FAILURE) {
        return;
    }

    ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    IupTextConvertLinColToPos(ih,lin,col,&pos);

    RETURN_LONG(pos);
}
/* }}} */

/* {{{ proto resource IupTextConvertPosToLinCol(resource ih, int pos)
   ;
 */
PHP_FUNCTION(IupTextConvertPosToLinCol)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih;

    zend_long pos;

    int lin,col;

    zval llin,lcol;

    HashTable *arr;

    if (zend_parse_parameters(argc TSRMLS_DC,"rl",&ihandle_res,&pos) == FAILURE) {
        return;
    }

    ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    IupTextConvertPosToLinCol(ih,pos,&lin,&col);

    ALLOC_HASHTABLE(arr);
    zend_hash_init(arr, 8, NULL, ZVAL_PTR_DTOR, 0);

    ZVAL_LONG(&llin,lin);
    ZVAL_LONG(&lcol,col);

    zend_hash_str_add_new(arr,"lin",4,&llin);
    zend_hash_str_add_new(arr,"col",4,&lcol);

    RETURN_ARR(arr);
}
/* }}} */

/* {{{ proto string IupConvertXYToPos(resource ih, int x, int y)
   ;
 */
PHP_FUNCTION(IupConvertXYToPos)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih;

    zend_long x,y;

    int i;

    if (zend_parse_parameters(argc TSRMLS_DC,"rll",&ihandle_res,&x,&y) == FAILURE) {
        return;
    }

    ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    i = IupConvertXYToPos(ih,x,y);

    RETURN_LONG(i);
}
/* }}} */
/* {{{ proto int IupStoreGlobal()
   ;
 */
PHP_FUNCTION(IupStoreGlobal)
{

    php_error(E_WARNING, "IupStoreGlobal: not yet implemented");

}
/* }}} */

/* {{{ proto int IupStoreAttribute()
   ;
 */
PHP_FUNCTION(IupStoreAttribute)
{

    php_error(E_WARNING, "IupStoreAttribute: not yet implemented");

}
/* }}} */

/* {{{ proto int IupSetfAttribute()
   ;
 */
PHP_FUNCTION(IupSetfAttribute)
{

    php_error(E_WARNING, "IupSetfAttribute: not yet implemented");

}
/* }}} */

/* {{{ proto int IupStoreAttributeId()
   ;
 */
PHP_FUNCTION(IupStoreAttributeId)
{

    php_error(E_WARNING, "IupStoreAttributeId: not yet implemented");

}
/* }}} */

/* {{{ proto int IupSetfAttributeId()
   ;
 */
PHP_FUNCTION(IupSetfAttributeId)
{

    php_error(E_WARNING, "IupSetfAttributeId: not yet implemented");

}
/* }}} */

/* {{{ proto int IupStoreAttributeId2()
   ;
 */
PHP_FUNCTION(IupStoreAttributeId2)
{

    php_error(E_WARNING, "IupStoreAttributeId2: not yet implemented");

}
/* }}} */

/* {{{ proto int IupSetfAttributeId2()
   ;
 */
PHP_FUNCTION(IupSetfAttributeId2)
{

    php_error(E_WARNING, "IupSetfAttributeId2: not yet implemented");

}
/* }}} */
/* {{{ proto int IupTreeSetUserId()
   ;
 */
PHP_FUNCTION(IupTreeSetUserId)
{

    php_error(E_WARNING, "IupTreeSetUserId: not yet implemented");

}
/* }}} */

/* {{{ proto int IupTreeGetUserId()
   ;
 */
PHP_FUNCTION(IupTreeGetUserId)
{

    php_error(E_WARNING, "IupTreeGetUserId: not yet implemented");

}
/* }}} */

/* {{{ proto int IupTreeGetId()
   ;
 */
PHP_FUNCTION(IupTreeGetId)
{

    php_error(E_WARNING, "IupTreeGetId: not yet implemented");

}
/* }}} */

/* {{{ proto int IupTreeSetAttributeHandle()
   ;
 */
PHP_FUNCTION(IupTreeSetAttributeHandle)
{

    php_error(E_WARNING, "IupTreeSetAttributeHandle: not yet implemented");

}
/* }}} */
/* {{{ proto string IupFileDlg()
   ;
*/
PHP_FUNCTION(IupFileDlg)
{
    Ihandle *re;
    if (zend_parse_parameters_none() == FAILURE) {
        return;
    }

    re = IupFileDlg();

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}
/* }}} */

/* {{{ proto string IupMessageDlg()
   ;
*/
PHP_FUNCTION(IupMessageDlg)
{
    Ihandle *re;
    if (zend_parse_parameters_none() == FAILURE) {
        return;
    }

    re = IupMessageDlg();

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}
/* }}} */

/* {{{ proto string IupColorDlg()
   ;
*/
PHP_FUNCTION(IupColorDlg)
{
    Ihandle *re;
    if (zend_parse_parameters_none() == FAILURE) {
        return;
    }

    re = IupColorDlg();

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}
/* }}} */

/* {{{ proto string IupFontDlg()
   ;
*/
PHP_FUNCTION(IupFontDlg)
{
    Ihandle *re;
    if (zend_parse_parameters_none() == FAILURE) {
        return;
    }

    re = IupFontDlg();

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}
/* }}} */

/* {{{ proto string IupProgressDlg()
   ;
*/
PHP_FUNCTION(IupProgressDlg)
{
    Ihandle *re;
    if (zend_parse_parameters_none() == FAILURE) {
        return;
    }

    re = IupProgressDlg();

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}
/* }}} */

/* {{{ proto int IupGetFile(string arq)
   ;
 */
PHP_FUNCTION(IupGetFile)
{
    int argc = ZEND_NUM_ARGS();

    char *arq = NULL;
    size_t arq_len;

    int i;

    if (zend_parse_parameters(argc, "s", &arq, &arq_len) == FAILURE) {
        return;
    }

    i = IupGetFile(arq);

    RETURN_LONG(i);
}
/* }}} */

/* {{{ proto string IupMessage(string title, string msg)
   ;
 */
PHP_FUNCTION(IupMessage)
{
    int argc = ZEND_NUM_ARGS();

    char *title = NULL;
    size_t title_len;

    char *msg = NULL;
    size_t msg_len;

    if (zend_parse_parameters(argc, "ss", &title, &title_len, &msg, &msg_len) == FAILURE) {
        return;
    }

    IupMessage(title,msg);

    RETURN_BOOL(1);
}
/* }}} */

/* {{{ proto string IupMessagef(string title, string format, string msg)
   ;
 */
PHP_FUNCTION(IupMessagef)
{
    int argc = ZEND_NUM_ARGS();

    char *title = NULL;
    size_t title_len;

    char *format = NULL;
    size_t format_len;

    char *msg = NULL;
    size_t msg_len;

    if (zend_parse_parameters(argc, "sss", &title, &title_len, &format, &format_len, &msg, &msg_len) == FAILURE) {
        return;
    }

    IupMessagef(title,format,msg);

    RETURN_BOOL(1);
}
/* }}} */

/* {{{ proto string IupMessageError(string parent, string message)
   ;
 */
PHP_FUNCTION(IupMessageError)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *parent;

    char *message = NULL;
    size_t message_len;

    if (zend_parse_parameters(argc TSRMLS_DC,"rs",&ihandle_res, &message, &message_len) == FAILURE) {
        return;
    }

    parent = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    IupMessageError(parent,message);

    RETURN_BOOL(1);
}
/* }}} */

/* {{{ proto string IupMessageAlarm(string parent, string title, string message, string buttons)
   ;
 */
PHP_FUNCTION(IupMessageAlarm)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *parent;

    char *title = NULL;
    size_t title_len;

    char *message = NULL;
    size_t message_len;

    char *buttons = NULL;
    size_t buttons_len;

    int i;

    if (zend_parse_parameters(argc TSRMLS_DC,"rsss",&ihandle_res, &title, &title_len, &message, &message_len, &buttons, &buttons_len) == FAILURE) {
        return;
    }

    i = IupMessageAlarm(parent,title,message,buttons);

    RETURN_LONG(i);
}
/* }}} */

/* {{{ proto string IupAlarm(string title, string msg, string b1, string b2, string b3)
   ;
 */
PHP_FUNCTION(IupAlarm)
{
    int argc = ZEND_NUM_ARGS();

    char *title = NULL;
    size_t title_len;

    char *msg = NULL;
    size_t msg_len;

    char *b1 = NULL;
    size_t b1_len;

    char *b2 = NULL;
    size_t b2_len;

    char *b3 = NULL;
    size_t b3_len;

    int i;
    

    if (zend_parse_parameters(argc, "sssss", &title, &title_len, &msg, &msg_len, &b1, &b1_len, &b2, &b2_len, &b3, &b3_len) == FAILURE) {
        return;
    }

    i = IupAlarm(title,msg,b1,b2,b3);

    RETURN_LONG(i);
}
/* }}} */

/* {{{ proto int IupScanf()
   ;
 */
PHP_FUNCTION(IupScanf)
{

    php_error(E_WARNING, "IupScanf: not yet implemented");

}
/* }}} */

/* {{{ proto int IupListDialog()
   ;
 */
PHP_FUNCTION(IupListDialog)
{

    php_error(E_WARNING, "IupListDialog: not yet implemented");

}
/* }}} */

/* {{{ proto string IupGetText(string title, string text, int maxsize)
   ;
 */
PHP_FUNCTION(IupGetText)
{
    int argc = ZEND_NUM_ARGS();

    char *title = NULL;
    size_t title_len;

    char *text = NULL;
    size_t text_len;

    zend_long maxsize;

    int i;

    if (zend_parse_parameters(argc, "ssl", &title, &title_len, &text, &text_len,&maxsize) == FAILURE) {
        return;
    }

    i = IupGetText(title,text,maxsize);

    RETURN_LONG(i);
}
/* }}} */

/* {{{ proto resource IupGetColor(int x, int y)
   ;
 */
PHP_FUNCTION(IupGetColor)
{
    int argc = ZEND_NUM_ARGS();

    zend_long x,y;

    unsigned char r, g, b;
    zval rr,gg,bb;

    HashTable *arr = NULL;

    if (zend_parse_parameters(argc,"ll",&x,&y) == FAILURE) {
        return;
    }

    IupGetColor(x,y,&r,&g,&b);

    ALLOC_HASHTABLE(arr);
    zend_hash_init(arr,8,NULL,NULL,0);

    ZVAL_LONG(&rr,r);
    ZVAL_LONG(&gg,g);
    ZVAL_LONG(&bb,b);

    zend_hash_str_add_new(arr,"r",2,&rr);
    zend_hash_str_add_new(arr,"g",2,&gg);
    zend_hash_str_add_new(arr,"b",2,&bb);

    RETURN_ARR(arr);
}
/* }}} */

/* {{{ proto int IupGetParam()
   ;
 */
PHP_FUNCTION(IupGetParam)
{

    php_error(E_WARNING, "IupGetParam: not yet implemented");

}
/* }}} */

/* {{{ proto int IupGetParamv()
   ;
 */
PHP_FUNCTION(IupGetParamv)
{

    php_error(E_WARNING, "IupGetParamv: not yet implemented");

}
/* }}} */

/* {{{ proto int IupParam()
   ;
 */
PHP_FUNCTION(IupParam)
{

    php_error(E_WARNING, "IupParam: not yet implemented");

}
/* }}} */

/* {{{ proto int IupParamBox()
   ;
 */
PHP_FUNCTION(IupParamBox)
{

    php_error(E_WARNING, "IupParamBox: not yet implemented");

}
/* }}} */

/* {{{ proto int IupParamBoxv()
   ;
 */
PHP_FUNCTION(IupParamBoxv)
{

    php_error(E_WARNING, "IupParamBoxv: not yet implemented");

}
/* }}} */

/* {{{ proto resource IupLayoutDialog(resource dialog)
   ;
 */
PHP_FUNCTION(IupLayoutDialog)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *dialog,*re;

    if (zend_parse_parameters(argc TSRMLS_DC,"r!",&ihandle_res) == FAILURE) {
        return;
    }

    dialog = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    re = IupLayoutDialog(dialog);

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}
/* }}} */

/* {{{ proto resource IupElementPropertiesDialog(resource elem)
   ;
 */
PHP_FUNCTION(IupElementPropertiesDialog)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *elem,*re;

    if (zend_parse_parameters(argc TSRMLS_DC,"r",&ihandle_res) == FAILURE) {
        return;
    }

    elem = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    re = IupElementPropertiesDialog(elem);

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}
/* }}} */
