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

int event_set_callback(Ihandle *ih , char * event_name)
{
    Icallback cb;

    char full_name[80];

    char *class_name = IupGetClassName(ih);

    sprintf(full_name,"%s_%s",class_name,event_name);

    cb = zend_hash_str_find_ptr(iup_callback,full_name,strlen(full_name));

    if(cb == NULL){
        sprintf(full_name,"%s: no callback function registered.",full_name);
        php_error(E_WARNING, full_name);
        return 0;
    }

    IupSetCallback(ih, event_name, cb);

    return 1;
}

void event_get_callinfo( Ihandle *self , char * event_name, zend_fcall_info **callable)
{
    intptr_t ih_p_int;

    char event_key_str[100];

    zend_string * event_key;

    zval * event_val;

    ih_p_int = (intptr_t)self;

    sprintf(event_key_str,"IUP_%s_%"SCNiPTR,event_name,ih_p_int);

    event_key = zend_string_init(event_key_str, strlen(event_key_str), 0);

    // 判断事件数组中是否存有相同事件id
    event_val = zend_hash_find(iup_events,event_key);

    if(event_val == NULL){
        // 没有相应的事件，直接返回
        *callable = NULL;
    }

    *callable = zend_fetch_resource_ex(event_val,"iup-event",le_iup_event);
}

int event_call_function( Ihandle *self , char * event_name, zend_fcall_info *callable)
{

    int call_result;

    zval func_result;

    zend_long func_result_value;

    callable->retval = &func_result;

    call_result = zend_call_function(callable, NULL);

    func_result_value = Z_LVAL(func_result);

    if(
        func_result_value == IUP_IGNORE || 
        func_result_value == IUP_DEFAULT || 
        func_result_value == IUP_CLOSE || 
        func_result_value == IUP_CONTINUE
        )
    {

        return (int)func_result_value;
    }

    return IUP_DEFAULT;
}

int event_base( Ihandle *self , char * event_name, int param_count, zval *call_params)
{
    char warning[80];

    zend_fcall_info *callable;

    event_get_callinfo(self,event_name,&callable);

    if(callable == NULL){
        sprintf(warning,"%s: no user function set.",event_name);
        php_error(E_WARNING, warning);
        return IUP_DEFAULT;
    }

    ZVAL_RES(&call_params[0],zend_register_resource(self, le_iup_ihandle));

    callable->param_count = param_count;

    callable->params = call_params;

    return event_call_function(self , event_name, callable);
}

// =============================================================================

int event_common( Ihandle *self , char * event_name)
{
    zval call_params[1];
    return event_base(self,event_name,1,&call_params);
}

int event_common_i( Ihandle *self , char * event_name, int i)
{
    zval call_params[2];

    ZVAL_LONG(&call_params[1],i);

    return event_base(self,event_name,2,&call_params);
}

// =============================================================================

int event_common_action( Ihandle *self ){
    return event_common(self,"ACTION");
}

// common events
int event_common_destroy_cb( Ihandle *self ){
    return event_common(self,"DESTROY_CB");
}

int event_common_ldestroy_cb( Ihandle *self ){
    return event_common(self,"LDESTROY_CB");
}

int event_common_map_cb( Ihandle *self ){
    return event_common(self,"MAP_CB");
}

int event_common_unmap_cb( Ihandle *self ){
    return event_common(self,"UNMAP_CB");
}

int event_common_getfocus_cb( Ihandle *self ){
    return event_common(self,"GETFOCUS_CB");
}

int event_common_killfocus_cb( Ihandle *self ){
    return event_common(self,"KILLFOCUS_CB");
}

int event_common_enterwindow_cb( Ihandle *self ){
    return event_common(self,"ENTERWINDOW_CB");
}

int event_common_leavewindow_cb( Ihandle *self ){
    return event_common(self,"LEAVEWINDOW_CB");
}

int event_common_help_cb( Ihandle *self ){
    return event_common(self,"HELP_CB");
}

int event_common_k_any( Ihandle *self ,int i){
    return event_common_i(self,"K_ANY",i);
}

// =============================================================================
void event_register_callback()
{
    zval event_callback;

    Icallback cb;
    
    cb = (Icallback) event_common_action;

    ZVAL_PTR(&event_callback,cb);
    zend_hash_str_add_new(iup_callback,"button_ACTION",strlen("button_ACTION"),&event_callback);

    // cb = (Icallback) event_common_action;
    // ZVAL_PTR(&event_callback,cb);
    zend_hash_str_add_new(iup_callback,"item_ACTION",strlen("item_ACTION"),&event_callback);

    cb = (Icallback) event_common_k_any;
    ZVAL_PTR(&event_callback,cb);
    zend_hash_str_add_new(iup_callback,"text_K_ANY",strlen("text_K_ANY"),&event_callback);

}