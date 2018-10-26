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

void event_get_callinfo(char * event_name, Ihandle *ih , zend_fcall_info **callable)
{
    intptr_t ih_p_int;

    char event_key_str[100];

    zend_string * event_key;

    zval * event_val;

    ih_p_int = (intptr_t)ih;

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

int event_call_function(zend_fcall_info *callable)
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

int event_base(char * event_name, Ihandle *ih , int param_count, zval *call_params)
{
    char warning[80];

    zend_fcall_info *callable;

    event_get_callinfo(event_name,ih,&callable);

    if(callable == NULL){
        sprintf(warning,"%s: no user function set.",event_name);
        php_error(E_WARNING, warning);
        return IUP_DEFAULT;
    }

    ZVAL_RES(&call_params[0],zend_register_resource(ih, le_iup_ihandle));

    callable->param_count = param_count;

    callable->params = call_params;

    return event_call_function(callable);
}

// =============================================================================

int event_common(char * event_name, Ihandle *ih)
{
    zval call_params[1];
    return event_base(event_name,ih,1,&call_params);
}

int event_common_i(char * event_name, Ihandle *ih , int i)
{
    zval call_params[2];

    ZVAL_LONG(&call_params[1],i);

    return event_base(event_name,ih,2,&call_params);
}

int event_common_s(char * event_name, Ihandle *ih , char *str)
{
    zval call_params[2];

    ZVAL_STR(&call_params[1],str);

    return event_base(event_name,ih,2,&call_params);
}

int event_common_ii(char * event_name, Ihandle *ih , int i1, int i2)
{
    zval call_params[3];

    ZVAL_LONG(&call_params[1],i1);
    ZVAL_LONG(&call_params[2],i2);

    return event_base(event_name,ih,3,&call_params);
}

int event_common_iis(char * event_name, Ihandle *ih , int i1, int i2, char * str)
{
    zval call_params[4];

    ZVAL_LONG(&call_params[1],i1);
    ZVAL_LONG(&call_params[2],i2);

    ZVAL_STR(&call_params[3],str);

    return event_base(event_name,ih,4,&call_params);
}

int event_common_iff(char * event_name, Ihandle *ih , int i1, float f1, float f2)
{
    zval call_params[4];

    ZVAL_LONG(&call_params[1],i1);

    ZVAL_DOUBLE(&call_params[2],f1);
    ZVAL_DOUBLE(&call_params[3],f2);

    return event_base(event_name,ih,4,&call_params);
}

int event_common_nii(char * event_name, Ihandle *ih , Ihandle *ih2, int i1, int i2)
{
    zval call_params[4];

    ZVAL_RES(&call_params[1],zend_register_resource(ih2, le_iup_ihandle));
    ZVAL_LONG(&call_params[2],i1);
    ZVAL_LONG(&call_params[3],i2);

    return event_base(event_name,ih,4,&call_params);
}


int event_common_sCi(char * event_name, Ihandle *ih , char *str, void* data, int i)
{
    zval call_params[4];

    ZVAL_STR(&call_params[1],str);
    ZVAL_STR(&call_params[2],(char *)data);
    ZVAL_LONG(&call_params[3],i);

    return event_base(event_name,ih,4,&call_params);
}

int event_common_siii(char * event_name, Ihandle *ih , char *str, int i1, int i2, int i3)
{
    zval call_params[5];

    ZVAL_STR(&call_params[1],str);

    ZVAL_LONG(&call_params[2],i1);
    ZVAL_LONG(&call_params[3],i2);
    ZVAL_LONG(&call_params[4],i3);

    return event_base(event_name,ih,5,&call_params);
}

int event_common_fiis(char * event_name, Ihandle *ih , float f1, int i1, int i2, char *str)
{
    zval call_params[5];

    ZVAL_DOUBLE(&call_params[1],f1);

    ZVAL_LONG(&call_params[2],i1);
    ZVAL_LONG(&call_params[3],i2);

    ZVAL_STR(&call_params[4],str);

    return event_base(event_name,ih,5,&call_params);
}



int event_common_iiis(char * event_name, Ihandle *ih , int i1, int i2, int i3, char * str)
{
    zval call_params[5];

    ZVAL_LONG(&call_params[1],i1);
    ZVAL_LONG(&call_params[2],i2);
    ZVAL_LONG(&call_params[3],i3);

    ZVAL_STR(&call_params[4],str);

    return event_base(event_name,ih,5,&call_params);
}

int event_common_iIII(char * event_name, Ihandle *ih , int i1, int *pi1, int *pi2, int *pi3)
{
    zval call_params[5];

    int i;

    zend_long index;

    HashTable *arr1, *arr2, *arr3;

    ALLOC_HASHTABLE(arr1);
    zend_hash_init(arr1,i1,NULL,NULL,0);

    ALLOC_HASHTABLE(arr2);
    zend_hash_init(arr2,i1,NULL,NULL,0);

    ALLOC_HASHTABLE(arr3);
    zend_hash_init(arr3,i1,NULL,NULL,0);

    for (int i = 0; i < i1; ++i)
    {
        ZVAL_LONG(&index,i);
        zend_hash_index_add(arr1,index,pi1[i]);
        zend_hash_index_add(arr2,index,pi2[i]);
        zend_hash_index_add(arr3,index,pi3[i]);
    }

    ZVAL_LONG(&call_params[1],i1);

    ZVAL_ARR(&call_params[2],arr1);
    ZVAL_ARR(&call_params[3],arr2);
    ZVAL_ARR(&call_params[4],arr3);

    return event_base(event_name,ih,5,&call_params);
}


int event_common_iiiis(char * event_name, Ihandle *ih , int i1, int i2, int i3, int i4, char * str)
{
    zval call_params[6];

    ZVAL_LONG(&call_params[1],i1);
    ZVAL_LONG(&call_params[2],i2);
    ZVAL_LONG(&call_params[3],i3);
    ZVAL_LONG(&call_params[4],i4);

    ZVAL_STR(&call_params[5],str);

    return event_base(event_name,ih,6,&call_params);
}

// =============================================================================

int event_common_action( Ihandle *ih ){
    return event_common("ACTION",ih);
}

// common events
int event_common_destroy_cb( Ihandle *ih ){
    return event_common("DESTROY_CB",ih);
}

int event_common_ldestroy_cb( Ihandle *ih ){
    return event_common("LDESTROY_CB",ih);
}

int event_common_map_cb( Ihandle *ih ){
    return event_common("MAP_CB",ih);
}

int event_common_unmap_cb( Ihandle *ih ){
    return event_common("UNMAP_CB",ih);
}

int event_common_getfocus_cb( Ihandle *ih ){
    return event_common("GETFOCUS_CB",ih);
}

int event_common_killfocus_cb( Ihandle *ih ){
    return event_common("KILLFOCUS_CB",ih);
}

int event_common_enterwindow_cb( Ihandle *ih ){
    return event_common("ENTERWINDOW_CB",ih);
}

int event_common_leavewindow_cb( Ihandle *ih ){
    return event_common("LEAVEWINDOW_CB",ih);
}

int event_common_help_cb( Ihandle *ih ){
    return event_common("HELP_CB",ih);
}

int event_common_k_any( Ihandle *ih ,int i){
    return event_common_i("K_ANY",ih,i);
}

// =============================================================================

int event_flat_action( Ihandle *ih ){
    return event_common("FLAT_ACTION",ih);
}

// common events
int event_flat_destroy_cb( Ihandle *ih ){
    return event_common("FLAT_DESTROY_CB",ih);
}

int event_flat_ldestroy_cb( Ihandle *ih ){
    return event_common("FLAT_LDESTROY_CB",ih);
}

int event_flat_map_cb( Ihandle *ih ){
    return event_common("FLAT_MAP_CB",ih);
}

int event_flat_unmap_cb( Ihandle *ih ){
    return event_common("FLAT_UNMAP_CB",ih);
}

int event_flat_getfocus_cb( Ihandle *ih ){
    return event_common("FLAT_GETFOCUS_CB",ih);
}

int event_flat_killfocus_cb( Ihandle *ih ){
    return event_common("FLAT_KILLFOCUS_CB",ih);
}

int event_flat_enterwindow_cb( Ihandle *ih ){
    return event_common("FLAT_ENTERWINDOW_CB",ih);
}

int event_flat_leavewindow_cb( Ihandle *ih ){
    return event_common("FLAT_LEAVEWINDOW_CB",ih);
}

int event_flat_help_cb( Ihandle *ih ){
    return event_common("FLAT_HELP_CB",ih);
}

int event_flat_k_any( Ihandle *ih ,int i){
    return event_common_i("FLAT_K_ANY",ih,i);
}


int event_flat_button_cb( Ihandle *ih, int button, int pressed, int x, int y, char* status){
    return event_common_iiiis("FLAT_BUTTON_CB",ih,button,pressed,x,y,status);
}

// =============================================================================
// 各个控件的事件
int event_elements_valuechanged_cb( Ihandle *ih ){
    return event_common("VALUECHANGED_CB",ih);
}

int event_elements_layoutupdate_cb( Ihandle *ih ){
    return event_common("LAYOUTUPDATE_CB",ih);
}

int event_elements_openclose_cb( Ihandle *ih, int state){
    return event_common_i("OPENCLOSE_CB",ih,state);
}

int event_elements_extrabutton_cb( Ihandle *ih, int button, int pressed){
    return event_common_ii("EXTRABUTTON_CB",ih,button,pressed);
}

int event_elements_detached_cb( Ihandle *ih, Ihandle *new_parent, int x, int y){
    return event_common_nii("DETACHED_CB",ih,new_parent,x,y);
}

int event_elements_restored_cb( Ihandle *ih, Ihandle *old_parent, int x, int y){
    return event_common_nii("RESTORED_CB",ih,old_parent,x,y);
}

int event_elements_focus_cb( Ihandle *ih, int focus){
    return event_common_i("FOCUS_CB",ih,focus);
}

int event_flat_focus_cb( Ihandle *ih, int focus){
    return event_common_i("FLAT_FOCUS_CB",ih,focus);
}

int event_elements_highlight_cb( Ihandle *ih ){
    return event_common("HIGHLIGHT_CB",ih);
}

int event_elements_open_cb( Ihandle *ih ){
    return event_common("OPEN_CB",ih);
}

int event_elements_menuclose_cb( Ihandle *ih ){
    return event_common("MENUCLOSE_CB",ih);
}

int event_elements_button_cb( Ihandle *ih, int button, int pressed, int x, int y, char* status){
    return event_common_iiiis("BUTTON_CB",ih,button,pressed,x,y,status);
}

int event_elements_dropdown_cb( Ihandle *ih, int state){
    return event_common_i("DROPDOWN_CB",ih,state);
}

int event_elements_dropshow_cb( Ihandle *ih, int state){
    return event_common_i("DROPSHOW_CB",ih,state);
}

int event_elements_motion_cb( Ihandle *ih, int x, int y, char *status){
    return event_common_iis("MOTION_CB",ih,x,y,status);
}

int event_elements_keypress_cb( Ihandle *ih, int c, int press){
    return event_common_ii("KEYPRESS_CB",ih,c,press);
}

int event_elements_resize_cb( Ihandle *ih, int width, int height){
    return event_common_ii("RESIZE_CB",ih,width,height);
}

int event_elements_scroll_cb( Ihandle *ih, int op, float posx, float posy){
    return event_common_iff("SCROLL_CB",ih,op,posx,posy);
}

int event_elements_touch_cb( Ihandle *ih, int id, int x, int y, char* state){
    return event_common_iiis("TOUCH_CB",ih,id,x,y,state);
}


int event_elements_multitouch_cb( Ihandle *ih, int count, int* pid, int* px, int* py, int* pstate){
    return event_common_iIII("MULTITOUCH_CB",ih,count,pid,px,py,pstate);
}


int event_elements_wheel_cb( Ihandle *ih, float delta, int x, int y, char *status){
    return event_common_fiis("WHEEL_CB",ih,delta,x,y,status);
}

int event_elements_wom_cb( Ihandle *ih, int state){
    return event_common_i("WOM_CB",ih,state);
}

int event_elements_dropfiles_cb( Ihandle *ih, const char* filename, int num, int x, int y){
    return event_common_siii("DROPFILES_CB",ih,filename,num,x,y);
}

int event_elements_dragbegin_cb( Ihandle *ih, int x, int y){
    return event_common_ii("DRAGBEGIN_CB",ih,x,y);
}

int event_elements_dragdatasize_cb( Ihandle *ih, int type){
    return event_common_s("DRAGDATASIZE_CB",ih,type);
}

int event_elements_dragdata_cb( Ihandle *ih, char* type, void* data, int size){
    return event_common_sCi("DRAGDATA_CB",ih,type,data,size);
}

int event_elements_dragend_cb( Ihandle *ih, int action){
    return event_common_i("DRAGEND_CB",ih,action);
}


int event_elements_dropdata_cb( Ihandle *ih, int action){
    return event_common_i("DROPDATA_CB",ih,action);
}

int event_elements_dropmotion_cb( Ihandle *ih, int action){
    return event_common_i("DROPMOTION_CB",ih,action);
}







// =============================================================================
void event_register_callback()
{
    zval event_callback;

    Icallback cb;

    // ======================================== common事件 ========================================
    ZVAL_PTR(&event_callback,(Icallback) event_common_action);
    zend_hash_str_add_new(iup_callback,"expander_ACTION",strlen("expander_ACTION"),&event_callback);
    zend_hash_str_add_new(iup_callback,"item_ACTION",strlen("item_ACTION"),&event_callback);
    zend_hash_str_add_new(iup_callback,"button_ACTION",strlen("button_ACTION"),&event_callback);
    zend_hash_str_add_new(iup_callback,"canvas_ACTION",strlen("canvas_ACTION"),&event_callback);

    ZVAL_PTR(&event_callback,(Icallback) event_flat_action);
    zend_hash_str_add_new(iup_callback,"flatbutton_FLAT_ACTION",strlen("flatbutton_FLAT_ACTION"),&event_callback);
    zend_hash_str_add_new(iup_callback,"flattoggle_FLAT_ACTION",strlen("flattoggle_FLAT_ACTION"),&event_callback);
    zend_hash_str_add_new(iup_callback,"dropbutton_FLAT_ACTION",strlen("dropbutton_FLAT_ACTION"),&event_callback);


    // DESTROY_CB
    ZVAL_PTR(&event_callback,(Icallback) event_common_destroy_cb);
    zend_hash_str_add_new(iup_callback,"frame_DESTROY_CB",strlen("frame_DESTROY_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"item_DESTROY_CB",strlen("item_DESTROY_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"submenu_DESTROY_CB",strlen("submenu_DESTROY_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"menu_DESTROY_CB",strlen("menu_DESTROY_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"button_DESTROY_CB",strlen("button_DESTROY_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"canvas_DESTROY_CB",strlen("canvas_DESTROY_CB"),&event_callback);

    ZVAL_PTR(&event_callback,(Icallback) event_flat_destroy_cb);
    zend_hash_str_add_new(iup_callback,"flatbutton_FLAT_DESTROY_CB",strlen("flatbutton_FLAT_DESTROY_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"flattoggle_FLAT_DESTROY_CB",strlen("flattoggle_FLAT_DESTROY_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"dropbutton_FLAT_DESTROY_CB",strlen("dropbutton_FLAT_DESTROY_CB"),&event_callback);
    
    // LDESTROY_CB
    ZVAL_PTR(&event_callback,(Icallback) event_common_ldestroy_cb);

    // MAP_CB
    ZVAL_PTR(&event_callback,(Icallback) event_common_map_cb);
    zend_hash_str_add_new(iup_callback,"frame_MAP_CB",strlen("frame_MAP_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"item_MAP_CB",strlen("item_MAP_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"submenu_MAP_CB",strlen("submenu_MAP_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"menu_MAP_CB",strlen("menu_MAP_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"button_MAP_CB",strlen("button_MAP_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"canvas_MAP_CB",strlen("canvas_MAP_CB"),&event_callback);

    ZVAL_PTR(&event_callback,(Icallback) event_flat_map_cb);
    zend_hash_str_add_new(iup_callback,"flatbutton_FLAT_MAP_CB",strlen("flatbutton_FLAT_MAP_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"flattoggle_FLAT_MAP_CB",strlen("flattoggle_FLAT_MAP_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"dropbutton_FLAT_MAP_CB",strlen("dropbutton_FLAT_MAP_CB"),&event_callback);
    
    // UNMAP_CB
    ZVAL_PTR(&event_callback,(Icallback) event_common_unmap_cb);
    zend_hash_str_add_new(iup_callback,"frame_UNMAP_CB",strlen("frame_UNMAP_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"item_UNMAP_CB",strlen("item_UNMAP_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"submenu_UNMAP_CB",strlen("submenu_UNMAP_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"menu_UNMAP_CB",strlen("menu_UNMAP_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"button_UNMAP_CB",strlen("button_UNMAP_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"canvas_UNMAP_CB",strlen("canvas_UNMAP_CB"),&event_callback);

    ZVAL_PTR(&event_callback,(Icallback) event_flat_unmap_cb);
    zend_hash_str_add_new(iup_callback,"flatbutton_FLAT_UNMAP_CB",strlen("flatbutton_FLAT_UNMAP_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"flattoggle_FLAT_UNMAP_CB",strlen("flattoggle_FLAT_UNMAP_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"dropbutton_FLAT_UNMAP_CB",strlen("dropbutton_FLAT_UNMAP_CB"),&event_callback);

    // GETFOCUS_CB
    ZVAL_PTR(&event_callback,(Icallback) event_common_getfocus_cb);
    zend_hash_str_add_new(iup_callback,"button_GETFOCUS_CB",strlen("button_GETFOCUS_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"canvas_GETFOCUS_CB",strlen("canvas_GETFOCUS_CB"),&event_callback);

    ZVAL_PTR(&event_callback,(Icallback) event_flat_getfocus_cb);
    zend_hash_str_add_new(iup_callback,"flatbutton_FLAT_GETFOCUS_CB",strlen("flatbutton_FLAT_GETFOCUS_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"flattoggle_FLAT_GETFOCUS_CB",strlen("flattoggle_FLAT_GETFOCUS_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"dropbutton_FLAT_GETFOCUS_CB",strlen("dropbutton_FLAT_GETFOCUS_CB"),&event_callback);

    // KILLFOCUS_CB
    ZVAL_PTR(&event_callback,(Icallback) event_common_killfocus_cb);
    zend_hash_str_add_new(iup_callback,"button_KILLFOCUS_CB",strlen("button_KILLFOCUS_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"canvas_KILLFOCUS_CB",strlen("canvas_KILLFOCUS_CB"),&event_callback);

    ZVAL_PTR(&event_callback,(Icallback) event_flat_killfocus_cb);
    zend_hash_str_add_new(iup_callback,"flatbutton_FLAT_KILLFOCUS_CB",strlen("flatbutton_FLAT_KILLFOCUS_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"flattoggle_FLAT_KILLFOCUS_CB",strlen("flattoggle_FLAT_KILLFOCUS_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"dropbutton_FLAT_KILLFOCUS_CB",strlen("dropbutton_FLAT_KILLFOCUS_CB"),&event_callback);

    // ENTERWINDOW_CB
    ZVAL_PTR(&event_callback,(Icallback) event_common_enterwindow_cb);
    zend_hash_str_add_new(iup_callback,"button_ENTERWINDOW_CB",strlen("button_ENTERWINDOW_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"canvas_ENTERWINDOW_CB",strlen("canvas_ENTERWINDOW_CB"),&event_callback);

    ZVAL_PTR(&event_callback,(Icallback) event_flat_enterwindow_cb);
    zend_hash_str_add_new(iup_callback,"flatbutton_FLAT_ENTERWINDOW_CB",strlen("flatbutton_FLAT_ENTERWINDOW_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"flattoggle_FLAT_ENTERWINDOW_CB",strlen("flattoggle_FLAT_ENTERWINDOW_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"dropbutton_FLAT_ENTERWINDOW_CB",strlen("dropbutton_FLAT_ENTERWINDOW_CB"),&event_callback);

    // LEAVEWINDOW_CB
    ZVAL_PTR(&event_callback,(Icallback) event_common_leavewindow_cb);
    zend_hash_str_add_new(iup_callback,"button_LEAVEWINDOW_CB",strlen("button_LEAVEWINDOW_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"canvas_LEAVEWINDOW_CB",strlen("canvas_LEAVEWINDOW_CB"),&event_callback);

    ZVAL_PTR(&event_callback,(Icallback) event_flat_leavewindow_cb);
    zend_hash_str_add_new(iup_callback,"flatbutton_FLAT_LEAVEWINDOW_CB",strlen("flatbutton_FLAT_LEAVEWINDOW_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"flattoggle_FLAT_LEAVEWINDOW_CB",strlen("flattoggle_FLAT_LEAVEWINDOW_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"dropbutton_FLAT_LEAVEWINDOW_CB",strlen("dropbutton_FLAT_LEAVEWINDOW_CB"),&event_callback);

    // HELP_CB
    ZVAL_PTR(&event_callback,(Icallback) event_common_help_cb);
    zend_hash_str_add_new(iup_callback,"item_HELP_CB",strlen("item_HELP_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"submenu_HELP_CB",strlen("submenu_HELP_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"menu_HELP_CB",strlen("menu_HELP_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"button_HELP_CB",strlen("button_HELP_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"canvas_HELP_CB",strlen("canvas_HELP_CB"),&event_callback);

    ZVAL_PTR(&event_callback,(Icallback) event_flat_help_cb);
    zend_hash_str_add_new(iup_callback,"flatbutton_FLAT_HELP_CB",strlen("flatbutton_FLAT_HELP_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"flattoggle_FLAT_HELP_CB",strlen("flattoggle_FLAT_HELP_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"dropbutton_FLAT_HELP_CB",strlen("dropbutton_FLAT_HELP_CB"),&event_callback);

    // K_ANY
    ZVAL_PTR(&event_callback,(Icallback) event_common_k_any);
    zend_hash_str_add_new(iup_callback,"button_K_ANY",strlen("button_K_ANY"),&event_callback);
    zend_hash_str_add_new(iup_callback,"canvas_K_ANY",strlen("canvas_K_ANY"),&event_callback);

    ZVAL_PTR(&event_callback,(Icallback) event_flat_k_any);
    zend_hash_str_add_new(iup_callback,"flatbutton_FLAT_K_ANY",strlen("flatbutton_FLAT_K_ANY"),&event_callback);
    zend_hash_str_add_new(iup_callback,"flattoggle_FLAT_K_ANY",strlen("flattoggle_FLAT_K_ANY"),&event_callback);
    zend_hash_str_add_new(iup_callback,"dropbutton_FLAT_K_ANY",strlen("dropbutton_FLAT_K_ANY"),&event_callback);


    // ======================================== 其他可共用事件 ========================================

    ZVAL_PTR(&event_callback,(Icallback) event_elements_valuechanged_cb);
    zend_hash_str_add_new(iup_callback,"split_VALUECHANGED_CB",strlen("split_VALUECHANGED_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"flatbutton_VALUECHANGED_CB",strlen("flatbutton_VALUECHANGED_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"flattoggle_VALUECHANGED_CB",strlen("flattoggle_VALUECHANGED_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"dropbutton_VALUECHANGED_CB",strlen("dropbutton_VALUECHANGED_CB"),&event_callback);

    // LAYOUTUPDATE_CB
    ZVAL_PTR(&event_callback,(Icallback) event_elements_layoutupdate_cb);

    zend_hash_str_add_new(iup_callback,"scrollbox_LAYOUTUPDATE_CB",strlen("scrollbox_LAYOUTUPDATE_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"flatscrollbox_LAYOUTUPDATE_CB",strlen("flatscrollbox_LAYOUTUPDATE_CB"),&event_callback);


    // FOCUS_CB
    ZVAL_PTR(&event_callback,(Icallback) event_elements_focus_cb);

    zend_hash_str_add_new(iup_callback,"frame_FOCUS_CB",strlen("frame_FOCUS_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"flatframe_FOCUS_CB",strlen("flatframe_FOCUS_CB"),&event_callback);

    zend_hash_str_add_new(iup_callback,"button_FOCUS_CB",strlen("button_FOCUS_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"canvas_FOCUS_CB",strlen("canvas_FOCUS_CB"),&event_callback);


    ZVAL_PTR(&event_callback,(Icallback) event_flat_focus_cb);
    zend_hash_str_add_new(iup_callback,"flatbutton_FLAT_FOCUS_CB",strlen("flatbutton_FLAT_FOCUS_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"flattoggle_FLAT_FOCUS_CB",strlen("flattoggle_FLAT_FOCUS_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"dropbutton_FLAT_FOCUS_CB",strlen("dropbutton_FLAT_FOCUS_CB"),&event_callback);

    // HIGHLIGHT_CB
    ZVAL_PTR(&event_callback,(Icallback) event_elements_highlight_cb);

    zend_hash_str_add_new(iup_callback,"item_HIGHLIGHT_CB",strlen("item_HIGHLIGHT_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"submenu_HIGHLIGHT_CB",strlen("submenu_HIGHLIGHT_CB"),&event_callback);

    // BUTTON_CB
    ZVAL_PTR(&event_callback,(Icallback) event_elements_button_cb);
    zend_hash_str_add_new(iup_callback,"button_BUTTON_CB",strlen("button_BUTTON_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"canvas_BUTTON_CB",strlen("canvas_BUTTON_CB"),&event_callback);

    ZVAL_PTR(&event_callback,(Icallback) event_flat_button_cb);
    zend_hash_str_add_new(iup_callback,"flatbutton_FLAT_BUTTON_CB",strlen("flatbutton_FLAT_BUTTON_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"flattoggle_FLAT_BUTTON_CB",strlen("flattoggle_FLAT_BUTTON_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"dropbutton_FLAT_BUTTON_CB",strlen("dropbutton_FLAT_BUTTON_CB"),&event_callback);

    
    // DROPDOWN_CB
    ZVAL_PTR(&event_callback,(Icallback) event_elements_dropdown_cb);
    zend_hash_str_add_new(iup_callback,"dropbutton_DROPDOWN_CB",strlen("dropbutton_DROPDOWN_CB"),&event_callback);

    // DROPSHOW_CB
    ZVAL_PTR(&event_callback,(Icallback) event_elements_dropshow_cb);
    zend_hash_str_add_new(iup_callback,"dropbutton_DROPSHOW_CB",strlen("dropbutton_DROPSHOW_CB"),&event_callback);

    // MOTION_CB
    ZVAL_PTR(&event_callback,(Icallback) event_elements_motion_cb);
    zend_hash_str_add_new(iup_callback,"canvas_MOTION_CB",strlen("canvas_MOTION_CB"),&event_callback);

    // KEYPRESS_CB
    ZVAL_PTR(&event_callback,(Icallback) event_elements_keypress_cb);
    zend_hash_str_add_new(iup_callback,"canvas_KEYPRESS_CB",strlen("canvas_KEYPRESS_CB"),&event_callback);

    // RESIZE_CB
    ZVAL_PTR(&event_callback,(Icallback) event_elements_resize_cb);
    zend_hash_str_add_new(iup_callback,"canvas_RESIZE_CB",strlen("canvas_RESIZE_CB"),&event_callback);

    // SCROLL_CB
    ZVAL_PTR(&event_callback,(Icallback) event_elements_scroll_cb);
    zend_hash_str_add_new(iup_callback,"canvas_SCROLL_CB",strlen("canvas_SCROLL_CB"),&event_callback);

    // TOUCH_CB
    ZVAL_PTR(&event_callback,(Icallback) event_elements_touch_cb);
    zend_hash_str_add_new(iup_callback,"canvas_TOUCH_CB",strlen("canvas_TOUCH_CB"),&event_callback);

    // MULTITOUCH_CB
    ZVAL_PTR(&event_callback,(Icallback) event_elements_multitouch_cb);
    zend_hash_str_add_new(iup_callback,"canvas_MULTITOUCH_CB",strlen("canvas_MULTITOUCH_CB"),&event_callback);

    // WHEEL_CB
    ZVAL_PTR(&event_callback,(Icallback) event_elements_wheel_cb);
    zend_hash_str_add_new(iup_callback,"canvas_WHEEL_CB",strlen("canvas_WHEEL_CB"),&event_callback);

    // WOM_CB
    ZVAL_PTR(&event_callback,(Icallback) event_elements_wom_cb);
    zend_hash_str_add_new(iup_callback,"canvas_WOM_CB",strlen("canvas_WOM_CB"),&event_callback);

    // DROPFILES_CB
    ZVAL_PTR(&event_callback,(Icallback) event_elements_dropfiles_cb);
    zend_hash_str_add_new(iup_callback,"dialog_DROPFILES_CB",strlen("dialog_DROPFILES_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"canvas_DROPFILES_CB",strlen("canvas_DROPFILES_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"glcanvas_DROPFILES_CB",strlen("glcanvas_DROPFILES_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"text_DROPFILES_CB",strlen("text_DROPFILES_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"list_DROPFILES_CB",strlen("list_DROPFILES_CB"),&event_callback);


    // DRAGBEGIN_CB
    ZVAL_PTR(&event_callback,(Icallback) event_elements_dragbegin_cb);
    zend_hash_str_add_new(iup_callback,"dialog_DRAGBEGIN_CB",strlen("dialog_DRAGBEGIN_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"canvas_DRAGBEGIN_CB",strlen("canvas_DRAGBEGIN_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"label_DRAGBEGIN_CB",strlen("label_DRAGBEGIN_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"text_DRAGBEGIN_CB",strlen("text_DRAGBEGIN_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"list_DRAGBEGIN_CB",strlen("list_DRAGBEGIN_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"tree_DRAGBEGIN_CB",strlen("tree_DRAGBEGIN_CB"),&event_callback);

    // DRAGDATASIZE_CB
    ZVAL_PTR(&event_callback,(Icallback) event_elements_dragdatasize_cb);
    zend_hash_str_add_new(iup_callback,"dialog_DRAGDATASIZE_CB",strlen("dialog_DRAGDATASIZE_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"canvas_DRAGDATASIZE_CB",strlen("canvas_DRAGDATASIZE_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"label_DRAGDATASIZE_CB",strlen("label_DRAGDATASIZE_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"text_DRAGDATASIZE_CB",strlen("text_DRAGDATASIZE_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"list_DRAGDATASIZE_CB",strlen("list_DRAGDATASIZE_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"tree_DRAGDATASIZE_CB",strlen("tree_DRAGDATASIZE_CB"),&event_callback);

    // DRAGDATA_CB
    ZVAL_PTR(&event_callback,(Icallback) event_elements_dragdata_cb);
    zend_hash_str_add_new(iup_callback,"dialog_DRAGDATA_CB",strlen("dialog_DRAGDATA_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"canvas_DRAGDATA_CB",strlen("canvas_DRAGDATA_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"label_DRAGDATA_CB",strlen("label_DRAGDATA_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"text_DRAGDATA_CB",strlen("text_DRAGDATA_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"list_DRAGDATA_CB",strlen("list_DRAGDATA_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"tree_DRAGDATA_CB",strlen("tree_DRAGDATA_CB"),&event_callback);

    // DRAGEND_CB
    ZVAL_PTR(&event_callback,(Icallback) event_elements_dragend_cb);
    zend_hash_str_add_new(iup_callback,"dialog_DRAGEND_CB",strlen("dialog_DRAGEND_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"canvas_DRAGEND_CB",strlen("canvas_DRAGEND_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"label_DRAGEND_CB",strlen("label_DRAGEND_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"text_DRAGEND_CB",strlen("text_DRAGEND_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"list_DRAGEND_CB",strlen("list_DRAGEND_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"tree_DRAGEND_CB",strlen("tree_DRAGEND_CB"),&event_callback);

    // DROPDATA_CB
    ZVAL_PTR(&event_callback,(Icallback) event_elements_dropdata_cb);
    zend_hash_str_add_new(iup_callback,"dialog_DROPDATA_CB",strlen("dialog_DROPDATA_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"canvas_DROPDATA_CB",strlen("canvas_DROPDATA_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"label_DROPDATA_CB",strlen("label_DROPDATA_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"text_DROPDATA_CB",strlen("text_DROPDATA_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"list_DROPDATA_CB",strlen("list_DROPDATA_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"tree_DROPDATA_CB",strlen("tree_DROPDATA_CB"),&event_callback);

    // DROPMOTION_CB
    ZVAL_PTR(&event_callback,(Icallback) event_elements_dropmotion_cb);
    zend_hash_str_add_new(iup_callback,"dialog_DROPMOTION_CB",strlen("dialog_DROPMOTION_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"canvas_DROPMOTION_CB",strlen("canvas_DROPMOTION_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"label_DROPMOTION_CB",strlen("label_DROPMOTION_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"text_DROPMOTION_CB",strlen("text_DROPMOTION_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"list_DROPMOTION_CB",strlen("list_DROPMOTION_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"tree_DROPMOTION_CB",strlen("tree_DROPMOTION_CB"),&event_callback);

    // ======================================== 独立事件 ========================================

    // Ihandle*  IupExpander   (Ihandle* child);
    ZVAL_PTR(&event_callback,(Icallback) event_elements_openclose_cb);
    zend_hash_str_add_new(iup_callback,"expander_OPENCLOSE_CB",strlen("expander_OPENCLOSE_CB"),&event_callback);

    ZVAL_PTR(&event_callback,(Icallback) event_elements_extrabutton_cb);
    zend_hash_str_add_new(iup_callback,"expander_EXTRABUTTON_CB",strlen("expander_EXTRABUTTON_CB"),&event_callback);

    // Ihandle*  IupDetachBox  (Ihandle* child);
    ZVAL_PTR(&event_callback,(Icallback) event_elements_detached_cb);
    zend_hash_str_add_new(iup_callback,"expander_DETACHED_CB",strlen("expander_DETACHED_CB"),&event_callback);

    ZVAL_PTR(&event_callback,(Icallback) event_elements_restored_cb);
    zend_hash_str_add_new(iup_callback,"expander_RESTORED_CB",strlen("expander_RESTORED_CB"),&event_callback);


    // Ihandle*  IupMenu       (Ihandle* child, ...);
    ZVAL_PTR(&event_callback,(Icallback) event_elements_open_cb);
    zend_hash_str_add_new(iup_callback,"menu_OPEN_CB",strlen("menu_OPEN_CB"),&event_callback);

    ZVAL_PTR(&event_callback,(Icallback) event_elements_menuclose_cb);
    zend_hash_str_add_new(iup_callback,"menu_MENUCLOSE_CB",strlen("menu_MENUCLOSE_CB"),&event_callback);




    // Ihandle*  IupCanvas(const char* action);

    // Ihandle*  IupFlatSeparator(void);
    // Ihandle*  IupDialog     (Ihandle* child);
    // Ihandle*  IupUser       (void);
    // Ihandle*  IupLabel      (const char* title);
    // Ihandle*  IupList       (const char* action);
    // Ihandle*  IupText       (const char* action);
    // Ihandle*  IupMultiLine  (const char* action);
    // Ihandle*  IupToggle     (const char* title, const char* action);
    // Ihandle*  IupTimer      (void);
    // Ihandle*  IupClipboard  (void);
    // Ihandle*  IupProgressBar(void);
    // Ihandle*  IupVal        (const char *type);
    // Ihandle*  IupTabs       (Ihandle* child, ...);
    // Ihandle*  IupTabsv      (Ihandle* *children);
    // Ihandle*  IupFlatTabs   (Ihandle* first, ...);
    // Ihandle*  IupFlatTabsv  (Ihandle* *children);
    // Ihandle*  IupTree       (void);
    // Ihandle*  IupLink       (const char* url, const char* title);
    // Ihandle*  IupAnimatedLabel(Ihandle* animation);
    // Ihandle*  IupDatePick   (void);
    // Ihandle*  IupCalendar   (void);
    // Ihandle*  IupColorbar   (void);
    // Ihandle*  IupGauge      (void);
    // Ihandle*  IupDial       (const char* type);
    // Ihandle*  IupColorBrowser(void);

    // action事件，带其他参数

    // cb = (Icallback) event_common_action;
    // ZVAL_PTR(&event_callback,cb);
    zend_hash_str_add_new(iup_callback,"item_ACTION",strlen("item_ACTION"),&event_callback);

    cb = (Icallback) event_common_k_any;
    ZVAL_PTR(&event_callback,cb);
    zend_hash_str_add_new(iup_callback,"text_K_ANY",strlen("text_K_ANY"),&event_callback);

}