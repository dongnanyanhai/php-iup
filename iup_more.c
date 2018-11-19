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

void config_register_callback(Ihandle* ih, zend_fcall_info * call_p)
{
    zend_string * event_key;

    zval event_val;

    intptr_t ih_p_int;

    char event_key_str[100];

    ih_p_int = (intptr_t)ih;

    sprintf(event_key_str,"IUP_%s_%"SCNiPTR,"RECENT_CB",ih_p_int);

    event_key = zend_string_init(event_key_str, strlen(event_key_str), 0);

    ZVAL_RES(&event_val,zend_register_resource(call_p, le_iup_event));

    zend_hash_update(iup_events, event_key, &event_val);
}

int config_recent_cb(Ihandle* ih)
{
  return event_common("RECENT_CB",ih);
}

/* {{{ proto void IupConfig()
    */
PHP_FUNCTION(IupConfig)
{
    Ihandle *re;

    if (zend_parse_parameters_none() == FAILURE) {
        return;
    }

    re = IupConfig();

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}
/* }}} */

/* {{{ proto int IupConfigLoad(resource ih)
   ;
 */
PHP_FUNCTION(IupConfigLoad)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih;

    int re;

    if (zend_parse_parameters(argc TSRMLS_DC,"r",&ihandle_res) == FAILURE) {
        return;
    }

    ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    re = IupConfigLoad(ih);

    RETURN_LONG(re);
}
/* }}} */

/* {{{ proto int IupConfigSave(resource ih)
   ;
 */
PHP_FUNCTION(IupConfigSave)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih;

    int re;

    if (zend_parse_parameters(argc TSRMLS_DC,"r",&ihandle_res) == FAILURE) {
        return;
    }

    ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    re = IupConfigSave(ih);

    RETURN_LONG(re);
}
/* }}} */


/* {{{ proto resource IupConfigSetVariableStr(resource ih, string group, string key, string value)
   ;
 */
PHP_FUNCTION(IupConfigSetVariableStr)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih;

    char *group = NULL;
    size_t group_len;

    char *key = NULL;
    size_t key_len;

    char *value = NULL;
    size_t value_len;

    if (zend_parse_parameters(argc TSRMLS_DC,"r!sss!",&ihandle_res,&group,&group_len,&key,&key_len,&value,&value_len) == FAILURE) {
        return;
    }

    if(ihandle_res != NULL){
        ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);
    }else{
        ih = NULL;
    }

    IupConfigSetVariableStr(ih,group,key,value);

    RETURN_BOOL(1);
}
/* }}} */

/* {{{ proto resource IupConfigSetVariableStrId(resource ih, string group, string key, int id, string value)
   ;
 */
PHP_FUNCTION(IupConfigSetVariableStrId)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih;

    char *group = NULL;
    size_t group_len;

    char *key = NULL;
    size_t key_len;

    zend_long id;

    char *value = NULL;
    size_t value_len;

    if (zend_parse_parameters(argc TSRMLS_DC,"r!ssls!",&ihandle_res,&group,&group_len,&key,&key_len,&id,&value,&value_len) == FAILURE) {
        return;
    }

    if(ihandle_res != NULL){
        ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);
    }else{
        ih = NULL;
    }

    IupConfigSetVariableStrId(ih,group,key,id,value);

    RETURN_BOOL(1);
}
/* }}} */

/* {{{ proto resource IupConfigSetVariableInt(resource ih, string group, string key, int value)
   ;
 */
PHP_FUNCTION(IupConfigSetVariableInt)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih;

    char *group = NULL;
    size_t group_len;

    char *key = NULL;
    size_t key_len;

    zend_long value;

    if (zend_parse_parameters(argc TSRMLS_DC,"r!ssl!",&ihandle_res,&group,&group_len,&key,&key_len,&value) == FAILURE) {
        return;
    }

    if(ihandle_res != NULL){
        ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);
    }else{
        ih = NULL;
    }

    IupConfigSetVariableInt(ih,group,key,value);

    RETURN_BOOL(1);
}
/* }}} */

/* {{{ proto resource IupConfigSetVariableIntId(resource ih, string group, string key, int id, int value)
   ;
 */
PHP_FUNCTION(IupConfigSetVariableIntId)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih;

    char *group = NULL;
    size_t group_len;

    char *key = NULL;
    size_t key_len;

    zend_long id,value;

    if (zend_parse_parameters(argc TSRMLS_DC,"r!ssll!",&ihandle_res,&group,&group_len,&key,&key_len,&id,&value) == FAILURE) {
        return;
    }

    if(ihandle_res != NULL){
        ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);
    }else{
        ih = NULL;
    }

    IupConfigSetVariableIntId(ih,group,key,id,value);

    RETURN_BOOL(1);
}
/* }}} */

/* {{{ proto resource IupConfigSetVariableDouble(resource ih, string group, string key, double value)
   ;
 */
PHP_FUNCTION(IupConfigSetVariableDouble)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih;

    char *group = NULL;
    size_t group_len;

    char *key = NULL;
    size_t key_len;

    double value;

    if (zend_parse_parameters(argc TSRMLS_DC,"r!ssd!",&ihandle_res,&group,&group_len,&key,&key_len,&value) == FAILURE) {
        return;
    }

    if(ihandle_res != NULL){
        ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);
    }else{
        ih = NULL;
    }

    IupConfigSetVariableDouble(ih,group,key,value);

    RETURN_BOOL(1);
}
/* }}} */

/* {{{ proto resource IupConfigSetVariableDoubleId(resource ih, string group, string key, int id, double value)
   ;
 */
PHP_FUNCTION(IupConfigSetVariableDoubleId)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih;

    char *group = NULL;
    size_t group_len;

    char *key = NULL;
    size_t key_len;

    zend_long id;
    double value;

    if (zend_parse_parameters(argc TSRMLS_DC,"r!ssld!",&ihandle_res,&group,&group_len,&key,&key_len,&id,&value) == FAILURE) {
        return;
    }

    if(ihandle_res != NULL){
        ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);
    }else{
        ih = NULL;
    }

    IupConfigSetVariableDoubleId(ih,group,key,id,value);

    RETURN_BOOL(1);
}
/* }}} */


/* {{{ proto resource IupConfigGetVariableStr(resource ih, string group, string key)
   ;
 */
PHP_FUNCTION(IupConfigGetVariableStr)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih;

    char *group = NULL;
    size_t group_len;

    char *key = NULL;
    size_t key_len;

    char * str;

    if (zend_parse_parameters(argc TSRMLS_DC,"r!ss",&ihandle_res,&group,&group_len,&key,&key_len) == FAILURE) {
        return;
    }

    if(ihandle_res != NULL){
        ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);
    }else{
        ih = NULL;
    }

    str = IupConfigGetVariableStr(ih,group,key);

    if(str != NULL){
        RETURN_STRING(str);
    }

    RETURN_NULL();
}
/* }}} */

/* {{{ proto resource IupConfigGetVariableStrId(resource ih, string group, string key, int id)
   ;
 */
PHP_FUNCTION(IupConfigGetVariableStrId)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih;

    char *group = NULL;
    size_t group_len;

    char *key = NULL;
    size_t key_len;

    zend_long id;

    char * str;

    if (zend_parse_parameters(argc TSRMLS_DC,"r!ssl",&ihandle_res,&group,&group_len,&key,&key_len,&id) == FAILURE) {
        return;
    }

    if(ihandle_res != NULL){
        ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);
    }else{
        ih = NULL;
    }

    str = IupConfigGetVariableStrId(ih,group,key,id);

    if(str != NULL){
        RETURN_STRING(str);
    }

    RETURN_NULL();
}
/* }}} */

/* {{{ proto resource IupConfigGetVariableInt(resource ih, string group, string key)
   ;
 */
PHP_FUNCTION(IupConfigGetVariableInt)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih;

    char *group = NULL;
    size_t group_len;

    char *key = NULL;
    size_t key_len;

    int re;

    if (zend_parse_parameters(argc TSRMLS_DC,"r!ss",&ihandle_res,&group,&group_len,&key,&key_len) == FAILURE) {
        return;
    }

    if(ihandle_res != NULL){
        ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);
    }else{
        ih = NULL;
    }

    re = IupConfigGetVariableInt(ih,group,key);

    RETURN_LONG(re);
}
/* }}} */

/* {{{ proto resource IupConfigGetVariableIntId(resource ih, string group, string key, int id)
   ;
 */
PHP_FUNCTION(IupConfigGetVariableIntId)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih;

    char *group = NULL;
    size_t group_len;

    char *key = NULL;
    size_t key_len;

    zend_long id;

    int re;

    if (zend_parse_parameters(argc TSRMLS_DC,"r!ssl",&ihandle_res,&group,&group_len,&key,&key_len,&id) == FAILURE) {
        return;
    }

    if(ihandle_res != NULL){
        ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);
    }else{
        ih = NULL;
    }

    re = IupConfigGetVariableIntId(ih,group,key,id);

    RETURN_LONG(re);
}
/* }}} */

/* {{{ proto resource IupConfigGetVariableDouble(resource ih, string group, string key)
   ;
 */
PHP_FUNCTION(IupConfigGetVariableDouble)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih;

    char *group = NULL;
    size_t group_len;

    char *key = NULL;
    size_t key_len;

    double d;

    if (zend_parse_parameters(argc TSRMLS_DC,"r!ss",&ihandle_res,&group,&group_len,&key,&key_len) == FAILURE) {
        return;
    }

    if(ihandle_res != NULL){
        ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);
    }else{
        ih = NULL;
    }

    d = IupConfigGetVariableDouble(ih,group,key);

    RETURN_DOUBLE(d);
}
/* }}} */

/* {{{ proto resource IupConfigGetVariableDoubleId(resource ih, string group, string key, int id)
   ;
 */
PHP_FUNCTION(IupConfigGetVariableDoubleId)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih;

    char *group = NULL;
    size_t group_len;

    char *key = NULL;
    size_t key_len;

    zend_long id;

    double d;

    if (zend_parse_parameters(argc TSRMLS_DC,"r!ssl",&ihandle_res,&group,&group_len,&key,&key_len,&id) == FAILURE) {
        return;
    }

    if(ihandle_res != NULL){
        ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);
    }else{
        ih = NULL;
    }

    d = IupConfigGetVariableDoubleId(ih,group,key,id);

    RETURN_DOUBLE(d);
}
/* }}} */



/* {{{ proto resource IupConfigGetVariableStrDef(resource ih, string group, string key, string def)
   ;
 */
PHP_FUNCTION(IupConfigGetVariableStrDef)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih;

    char *group = NULL;
    size_t group_len;

    char *key = NULL;
    size_t key_len;
    char *def = NULL;
    size_t def_len;

    char * str;

    if (zend_parse_parameters(argc TSRMLS_DC,"r!ssd",&ihandle_res,&group,&group_len,&key,&key_len,&def,&def_len) == FAILURE) {
        return;
    }

    if(ihandle_res != NULL){
        ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);
    }else{
        ih = NULL;
    }

    str = IupConfigGetVariableStrDef(ih,group,key,def);

    if(str != NULL){
        RETURN_STRING(str);
    }

    RETURN_NULL();
}
/* }}} */

/* {{{ proto resource IupConfigGetVariableStrIdDef(resource ih, string group, string key, int id, string def)
   ;
 */
PHP_FUNCTION(IupConfigGetVariableStrIdDef)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih;

    char *group = NULL;
    size_t group_len;

    char *key = NULL;
    size_t key_len;

    char *def = NULL;
    size_t def_len;

    zend_long id;

    char * str;

    if (zend_parse_parameters(argc TSRMLS_DC,"r!ssld",&ihandle_res,&group,&group_len,&key,&key_len,&id,&def,&def_len) == FAILURE) {
        return;
    }

    if(ihandle_res != NULL){
        ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);
    }else{
        ih = NULL;
    }

    str = IupConfigGetVariableStrIdDef(ih,group,key,id,def);

    if(str != NULL){
        RETURN_STRING(str);
    }

    RETURN_NULL();
}
/* }}} */

/* {{{ proto resource IupConfigGetVariableIntDef(resource ih, string group, string key, int def)
   ;
 */
PHP_FUNCTION(IupConfigGetVariableIntDef)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih;

    char *group = NULL;
    size_t group_len;

    char *key = NULL;
    size_t key_len;

    zend_long def;

    int re;

    if (zend_parse_parameters(argc TSRMLS_DC,"r!ssd",&ihandle_res,&group,&group_len,&key,&key_len,&def) == FAILURE) {
        return;
    }

    if(ihandle_res != NULL){
        ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);
    }else{
        ih = NULL;
    }

    re = IupConfigGetVariableIntDef(ih,group,key,def);

    RETURN_LONG(re);
}
/* }}} */

/* {{{ proto resource IupConfigGetVariableIntIdDef(resource ih, string group, string key, int id)
   ;
 */
PHP_FUNCTION(IupConfigGetVariableIntIdDef)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih;

    char *group = NULL;
    size_t group_len;

    char *key = NULL;
    size_t key_len;

    zend_long id,def;

    int re;

    if (zend_parse_parameters(argc TSRMLS_DC,"r!ssld",&ihandle_res,&group,&group_len,&key,&key_len,&id,&def) == FAILURE) {
        return;
    }

    if(ihandle_res != NULL){
        ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);
    }else{
        ih = NULL;
    }

    re = IupConfigGetVariableIntIdDef(ih,group,key,id,def);

    RETURN_LONG(re);
}
/* }}} */

/* {{{ proto resource IupConfigGetVariableDoubleDef(resource ih, string group, string key, double def)
   ;
 */
PHP_FUNCTION(IupConfigGetVariableDoubleDef)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih;

    char *group = NULL;
    size_t group_len;

    char *key = NULL;
    size_t key_len;

    double def,d;

    if (zend_parse_parameters(argc TSRMLS_DC,"r!ssd",&ihandle_res,&group,&group_len,&key,&key_len,&def) == FAILURE) {
        return;
    }

    if(ihandle_res != NULL){
        ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);
    }else{
        ih = NULL;
    }

    d = IupConfigGetVariableDoubleDef(ih,group,key,def);

    RETURN_DOUBLE(d);
}
/* }}} */

/* {{{ proto resource IupConfigGetVariableDoubleIdDef(resource ih, string group, string key, int id, double def)
   ;
 */
PHP_FUNCTION(IupConfigGetVariableDoubleIdDef)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih;

    char *group = NULL;
    size_t group_len;

    char *key = NULL;
    size_t key_len;

    zend_long id;

    double def,d;

    if (zend_parse_parameters(argc TSRMLS_DC,"r!ssld",&ihandle_res,&group,&group_len,&key,&key_len,&id,&def) == FAILURE) {
        return;
    }

    if(ihandle_res != NULL){
        ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);
    }else{
        ih = NULL;
    }

    d = IupConfigGetVariableDoubleIdDef(ih,group,key,id,def);

    RETURN_DOUBLE(d);
}
/* }}} */

/* {{{ proto resource IupConfigCopy(resource ih1, resource ih2, string exclude_prefix)
   ;
 */
PHP_FUNCTION(IupConfigCopy)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res_ih1 = NULL;
    zval *ihandle_res_ih2 = NULL;

    Ihandle *ih1,*ih2;

    char *exclude_prefix = NULL;
    size_t exclude_prefix_len;

    if (zend_parse_parameters(argc TSRMLS_DC,"r!r!s",&ihandle_res_ih1,&ihandle_res_ih2,&exclude_prefix,&exclude_prefix_len) == FAILURE) {
        return;
    }

    ih1 = zend_fetch_resource_ex(ihandle_res_ih1,"iup-handle",le_iup_ihandle);
    ih2 = zend_fetch_resource_ex(ihandle_res_ih2,"iup-handle",le_iup_ihandle);

    IupConfigCopy(ih1,ih2,exclude_prefix);

    RETURN_BOOL(1);
}
/* }}} */


/* {{{ proto resource IupConfigSetListVariable(resource ih, string group, string key, string value, int add)
   ;
 */
PHP_FUNCTION(IupConfigSetListVariable)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih;

    char *group = NULL;
    size_t group_len;

    char *key = NULL;
    size_t key_len;

    char *value = NULL;
    size_t value_len;

    zend_long add;

    if (zend_parse_parameters(argc TSRMLS_DC,"r!sss!l",&ihandle_res,&group,&group_len,&key,&key_len,&value,&value_len,&add) == FAILURE) {
        return;
    }

    if(ihandle_res != NULL){
        ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);
    }else{
        ih = NULL;
    }

    IupConfigSetListVariable(ih,group,key,value,add);

    RETURN_BOOL(1);
}
/* }}} */

/* {{{ proto resource IupConfigRecentInit(resource ih1, resource ih2, string exclude_prefix)
   ;
 */
PHP_FUNCTION(IupConfigRecentInit)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res_ih1 = NULL;
    zval *ihandle_res_ih2 = NULL;

    Ihandle *ih1,*ih2;

    zend_fcall_info callable;
    zend_fcall_info_cache call_cache;

    zend_fcall_info * call_p;

    call_p = (zend_fcall_info *)malloc(sizeof(zend_fcall_info));

    zend_long max_recent;

    if (zend_parse_parameters(argc TSRMLS_DC,"rrfl",&ihandle_res_ih1,&ihandle_res_ih2, &callable, &call_cache,&max_recent) == FAILURE) {
        return;
    }

    *call_p = callable;

    ih1 = zend_fetch_resource_ex(ihandle_res_ih1,"iup-handle",le_iup_ihandle);
    ih2 = zend_fetch_resource_ex(ihandle_res_ih2,"iup-handle",le_iup_ihandle);

    // 先注册
    config_register_callback(ih1,call_p);

    IupConfigRecentInit(ih1,ih2,config_recent_cb,max_recent);

    RETURN_BOOL(1);
}
/* }}} */


/* {{{ proto resource IupConfigRecentUpdate(resource ih, string filename)
   ;
 */
PHP_FUNCTION(IupConfigRecentUpdate)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    char *filename = NULL;
    size_t filename_len;

    Ihandle *ih;

    if (zend_parse_parameters(argc TSRMLS_DC,"r!s",&ihandle_res,&filename,&filename_len) == FAILURE) {
        return;
    }

    if(ihandle_res != NULL){
        ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);
    }else{
        ih = NULL;
    }

    IupConfigRecentUpdate(ih,filename);

    RETURN_BOOL(1);
}
/* }}} */


/* {{{ proto resource IupConfigDialogShow(resource ih1, resource ih2, string  name)
   ;
 */
PHP_FUNCTION(IupConfigDialogShow)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res_ih1 = NULL;
    zval *ihandle_res_ih2 = NULL;

    Ihandle *ih1,*ih2;

    char *name = NULL;
    size_t name_len;

    if (zend_parse_parameters(argc TSRMLS_DC,"r!r!s",&ihandle_res_ih1,&ihandle_res_ih2,&name,&name_len) == FAILURE) {
        return;
    }

    ih1 = zend_fetch_resource_ex(ihandle_res_ih1,"iup-handle",le_iup_ihandle);
    ih2 = zend_fetch_resource_ex(ihandle_res_ih2,"iup-handle",le_iup_ihandle);

    IupConfigDialogShow(ih1,ih2,name);

    RETURN_BOOL(1);
}
/* }}} */

/* {{{ proto resource IupConfigDialogClosed(resource ih1, resource ih2, string  name)
   ;
 */
PHP_FUNCTION(IupConfigDialogClosed)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res_ih1 = NULL;
    zval *ihandle_res_ih2 = NULL;

    Ihandle *ih1,*ih2;

    char *name = NULL;
    size_t name_len;

    if (zend_parse_parameters(argc TSRMLS_DC,"r!r!s",&ihandle_res_ih1,&ihandle_res_ih2,&name,&name_len) == FAILURE) {
        return;
    }

    ih1 = zend_fetch_resource_ex(ihandle_res_ih1,"iup-handle",le_iup_ihandle);
    ih2 = zend_fetch_resource_ex(ihandle_res_ih2,"iup-handle",le_iup_ihandle);

    IupConfigDialogClosed(ih1,ih2,name);

    RETURN_BOOL(1);
}
/* }}} */


/* {{{ proto void IupScintillaOpen()
    */
PHP_FUNCTION(IupScintillaOpen)
{
    if (zend_parse_parameters_none() == FAILURE) {
        return;
    }

    IupScintillaOpen();

    RETURN_BOOL(1);
}
/* }}} */

/* {{{ proto void IupScintilla()
    */
PHP_FUNCTION(IupScintilla)
{
    Ihandle *re;

    if (zend_parse_parameters_none() == FAILURE) {
        return;
    }

    re = IupScintilla();

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}
/* }}} */

/* {{{ proto void IupScintillaDlg()
    */
PHP_FUNCTION(IupScintillaDlg)
{
    Ihandle *re;
    if (zend_parse_parameters_none() == FAILURE) {
        return;
    }

    re = IupScintillaDlg();

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}
/* }}} */

/* {{{ proto void IupWebBrowserOpen()
    */
PHP_FUNCTION(IupWebBrowserOpen)
{

    int re;
    if (zend_parse_parameters_none() == FAILURE) {
        return;
    }

    re = IupWebBrowserOpen();

    RETURN_LONG(re);
}
/* }}} */

/* {{{ proto void IupWebBrowser()
    */
PHP_FUNCTION(IupWebBrowser)
{

    Ihandle *re;

    if (zend_parse_parameters_none() == FAILURE) {
        return;
    }

    re = IupWebBrowser();

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}
/* }}} */

/* {{{ proto void IupTuioOpen()
    */
/*PHP_FUNCTION(IupTuioOpen)
{

    int re;
    if (zend_parse_parameters_none() == FAILURE) {
        return;
    }

    re = IupTuioOpen();

    RETURN_LONG(re);
}*/
/* }}} */

/* {{{ proto void IupTuioClient()
    */
/*PHP_FUNCTION(IupTuioClient)
{

    int argc = ZEND_NUM_ARGS();
    zend_long port;

    Ihandle *re;

    if (zend_parse_parameters(argc, "l", &port) == FAILURE) 
        return;

    re = IupTuioClient(port);

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}*/
/* }}} */

/* {{{ proto void IupOleControlOpen()
    */
PHP_FUNCTION(IupOleControlOpen)
{

    int re;
    if (zend_parse_parameters_none() == FAILURE) {
        return;
    }

    re = IupOleControlOpen();

    RETURN_LONG(re);
}
/* }}} */

/* {{{ proto void IupOleControl()
    */
PHP_FUNCTION(IupOleControl)
{

    int argc = ZEND_NUM_ARGS();
    char *progid = NULL;
    size_t progid_len;

    Ihandle *re;

    if (zend_parse_parameters(argc, "s", &progid, &progid_len) == FAILURE) {
        return;
    }

    re = IupOleControl(progid);

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}
/* }}} */

/* {{{ proto string IupLoadImage(string file_name)
   ;
 */
PHP_FUNCTION(IupLoadImage)
{
    int argc = ZEND_NUM_ARGS();

    char *file_name = NULL;
    size_t file_name_len;

    Ihandle *re;

    if (zend_parse_parameters(argc, "s", &file_name, &file_name_len) == FAILURE) {
        return;
    }

    re = IupLoadImage(file_name);

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}
/* }}} */

/* {{{ proto resource IupSaveImage(resource ih, string file_name, string format)
   ;
 */
PHP_FUNCTION(IupSaveImage)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih;

    char *file_name = NULL;
    size_t file_name_len;

    char *format = NULL;
    size_t format_len;

    int re;

    if (zend_parse_parameters(argc TSRMLS_DC,"rss",&ihandle_res,&file_name, &file_name_len, &format, &format_len) == FAILURE) {
        return;
    }

    ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    re = IupSaveImage(ih,file_name,format);

    RETURN_LONG(re);
}
/* }}} */

/* {{{ proto string IupLoadAnimation(string file_name)
   ;
 */
PHP_FUNCTION(IupLoadAnimation)
{
    int argc = ZEND_NUM_ARGS();

    char *file_name = NULL;
    size_t file_name_len;

    Ihandle *re;

    if (zend_parse_parameters(argc, "s", &file_name, &file_name_len) == FAILURE) {
        return;
    }

    re = IupLoadAnimation(file_name);

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}
/* }}} */


/* {{{ proto resource IupLoadAnimationFrames(ref file_name_list, int file_count)
   ;
 */
PHP_FUNCTION(IupLoadAnimationFrames)
{
    int argc = ZEND_NUM_ARGS();

    // 用以遍历arr_list数组
    long num_key;
    zval *val;
    zend_string *key;

    HashTable *file_name_list_val;

    const char **file_name_list;

    zend_long file_count;

    int i;

    Ihandle *re;

    if (zend_parse_parameters(argc,"hl",&file_name_list_val,&file_count) == FAILURE) {
        return;
    }

    file_name_list = (char **)malloc(sizeof(char *)* file_count);

    i = 0;

    // 将php的字符串数组转换为c的字符串数组
    ZEND_HASH_FOREACH_KEY_VAL(file_name_list_val, num_key, key, val) {

        if(Z_TYPE_P(val) == IS_STRING && i < file_count) {

            file_name_list[i] = (char *)malloc(sizeof(char) * Z_STRLEN_P(val));

            file_name_list[i] = Z_STRVAL_P(val);

            i ++;
        }
    } ZEND_HASH_FOREACH_END();

    re = IupLoadAnimationFrames(file_name_list,file_count);

    free(file_name_list);

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}
/* }}} */

/* {{{ proto int IupGetNativeHandleImage(resource ih)
   ;
 */
PHP_FUNCTION(IupGetNativeHandleImage)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih;
    imImage *re;

    if (zend_parse_parameters(argc TSRMLS_DC,"r",&ihandle_res) == FAILURE) {
        return;
    }

    ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    re = IupGetNativeHandleImage(ih);

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}
/* }}} */

/* {{{ proto int IupGetImageNativeHandle(resource ih)
   ;
 */
PHP_FUNCTION(IupGetImageNativeHandle)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    imImage *ih;
    void *re;

    if (zend_parse_parameters(argc TSRMLS_DC,"r",&ihandle_res) == FAILURE) {
        return;
    }

    ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    re = IupGetImageNativeHandle(ih);

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}
/* }}} */

/* {{{ proto int IupImageFromImImage(resource ih)
   ;
 */
PHP_FUNCTION(IupImageFromImImage)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    imImage *ih;
    Ihandle *re;

    if (zend_parse_parameters(argc TSRMLS_DC,"r",&ihandle_res) == FAILURE) {
        return;
    }

    ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    re = IupImageFromImImage(ih);

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}
/* }}} */

/* {{{ proto int IupImageToImImage(resource ih)
   ;
 */
PHP_FUNCTION(IupImageToImImage)
{
    int argc = ZEND_NUM_ARGS();

    zval *ihandle_res = NULL;

    Ihandle *ih;
    imImage *re;

    if (zend_parse_parameters(argc TSRMLS_DC,"r",&ihandle_res) == FAILURE) {
        return;
    }

    ih = zend_fetch_resource_ex(ihandle_res,"iup-handle",le_iup_ihandle);

    re = IupImageToImImage(ih);

    RETURN_RES(zend_register_resource(re, le_iup_ihandle));
}
/* }}} */
