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

int event_common_d(char * event_name, Ihandle *ih , double d)
{
    zval call_params[2];

    ZVAL_DOUBLE(&call_params[1],d);

    return event_base(event_name,ih,2,&call_params);
}

int event_common_s(char * event_name, Ihandle *ih , char *str)
{
    zval call_params[2];

    zend_string * zstring;

    zstring = zend_string_init(str, strlen(str), 0);

    ZVAL_STR(&call_params[1],zstring);

    return event_base(event_name,ih,2,&call_params);
}

int event_common_ii(char * event_name, Ihandle *ih , int i1, int i2)
{
    zval call_params[3];

    ZVAL_LONG(&call_params[1],i1);
    ZVAL_LONG(&call_params[2],i2);

    return event_base(event_name,ih,3,&call_params);
}


int event_common_Ii(char * event_name, Ihandle *ih , int *pi1, int i1)
{
    zval call_params[3];

    zval zlong1;

    HashTable *arr1;

    ALLOC_HASHTABLE(arr1);
    zend_hash_init(arr1,i1,NULL,NULL,0);

    for (int i = 0; i < i1; ++i)
    {

        ZVAL_LONG(&zlong1,pi1[i]);

        zend_hash_index_add(arr1,i,&zlong1);
    }

    ZVAL_ARR(&call_params[1],arr1);
    ZVAL_LONG(&call_params[2],i1);

    return event_base(event_name,ih,3,&call_params);
}


int event_common_is(char * event_name, Ihandle *ih , int i, char *str)
{
    zval call_params[3];

    zend_string * zstring;
    zstring = zend_string_init(str, strlen(str), 0);

    ZVAL_LONG(&call_params[1],i);
    ZVAL_STR(&call_params[2],zstring);

    return event_base(event_name,ih,3,&call_params);
}

int event_common_si(char * event_name, Ihandle *ih , char *str, int i)
{
    zval call_params[3];

    zend_string * zstring;
    zstring = zend_string_init(str, strlen(str), 0);

    ZVAL_STR(&call_params[1],zstring);
    ZVAL_LONG(&call_params[2],i);

    return event_base(event_name,ih,3,&call_params);
}

int event_common_nn(char * event_name, Ihandle *ih , Ihandle *ih2, Ihandle *ih3)
{
    zval call_params[3];

    ZVAL_RES(&call_params[1],zend_register_resource(ih2, le_iup_ihandle));
    ZVAL_RES(&call_params[2],zend_register_resource(ih3, le_iup_ihandle));

    return event_base(event_name,ih,3,&call_params);
}


int event_common_iii(char * event_name, Ihandle *ih , int i1, int i2, int i3)
{
    zval call_params[4];

    ZVAL_LONG(&call_params[1],i1);
    ZVAL_LONG(&call_params[2],i2);
    ZVAL_LONG(&call_params[3],i3);

    return event_base(event_name,ih,4,&call_params);
}

int event_common_ccc(char * event_name, Ihandle *ih , unsigned char r, unsigned char g, unsigned char b)
{
    zval call_params[4];

    ZVAL_LONG(&call_params[1],(int)r);
    ZVAL_LONG(&call_params[2],(int)g);
    ZVAL_LONG(&call_params[3],(int)b);

    return event_base(event_name,ih,4,&call_params);
}


int event_common_iis(char * event_name, Ihandle *ih , int i1, int i2, char * str)
{
    zval call_params[4];

    zend_string * zstring;
    zstring = zend_string_init(str, strlen(str), 0);

    ZVAL_LONG(&call_params[1],i1);
    ZVAL_LONG(&call_params[2],i2);


    ZVAL_STR(&call_params[3],zstring);

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

    char *data_str;

    data_str = (char *)malloc(i);

    data_str = (char *)data;

    zend_string *zstring1,*zstring2;
    zstring1 = zend_string_init(str, strlen(str), 0);
    zstring2 = zend_string_init(data_str, strlen(data_str), 0);

    ZVAL_STR(&call_params[1],zstring1);
    ZVAL_STR(&call_params[2],zstring2);
    ZVAL_LONG(&call_params[3],i);

    return event_base(event_name,ih,4,&call_params);
}

int event_common_siii(char * event_name, Ihandle *ih , char *str, int i1, int i2, int i3)
{
    zval call_params[5];

    zend_string * zstring;
    zstring = zend_string_init(str, strlen(str), 0);

    ZVAL_STR(&call_params[1],zstring);

    ZVAL_LONG(&call_params[2],i1);
    ZVAL_LONG(&call_params[3],i2);
    ZVAL_LONG(&call_params[4],i3);

    return event_base(event_name,ih,5,&call_params);
}

int event_common_fiis(char * event_name, Ihandle *ih , float f1, int i1, int i2, char *str)
{
    zval call_params[5];

    zend_string * zstring;
    zstring = zend_string_init(str, strlen(str), 0);

    ZVAL_DOUBLE(&call_params[1],f1);

    ZVAL_LONG(&call_params[2],i1);
    ZVAL_LONG(&call_params[3],i2);

    ZVAL_STR(&call_params[4],zstring);

    return event_base(event_name,ih,5,&call_params);
}

int event_common_iiii(char * event_name, Ihandle *ih , int i1, int i2, int i3, int i4)
{
    zval call_params[5];

    ZVAL_LONG(&call_params[1],i1);
    ZVAL_LONG(&call_params[2],i2);
    ZVAL_LONG(&call_params[3],i3);
    ZVAL_LONG(&call_params[4],i4);

    return event_base(event_name,ih,5,&call_params);
}

int event_common_iiis(char * event_name, Ihandle *ih , int i1, int i2, int i3, char * str)
{
    zval call_params[5];

    zend_string * zstring;
    zstring = zend_string_init(str, strlen(str), 0);

    ZVAL_LONG(&call_params[1],i1);
    ZVAL_LONG(&call_params[2],i2);
    ZVAL_LONG(&call_params[3],i3);

    ZVAL_STR(&call_params[4],zstring);

    return event_base(event_name,ih,5,&call_params);
}

int event_common_iIIII(char * event_name, Ihandle *ih , int i1, int *pi1, int *pi2, int *pi3, int *pi4)
{
    zval call_params[6];

    zval zlong1,zlong2,zlong3,zlong4;

    HashTable *arr1, *arr2, *arr3, *arr4;

    ALLOC_HASHTABLE(arr1);
    zend_hash_init(arr1,i1,NULL,NULL,0);

    ALLOC_HASHTABLE(arr2);
    zend_hash_init(arr2,i1,NULL,NULL,0);

    ALLOC_HASHTABLE(arr3);
    zend_hash_init(arr3,i1,NULL,NULL,0);

    ALLOC_HASHTABLE(arr4);
    zend_hash_init(arr4,i1,NULL,NULL,0);

    for (int i = 0; i < i1; ++i)
    {

        ZVAL_LONG(&zlong1,pi1[i]);
        zend_hash_index_add(arr1,i,&zlong1);

        ZVAL_LONG(&zlong2,pi2[i]);
        zend_hash_index_add(arr2,i,&zlong2);

        ZVAL_LONG(&zlong3,pi3[i]);
        zend_hash_index_add(arr3,i,&zlong3);

        ZVAL_LONG(&zlong4,pi4[i]);
        zend_hash_index_add(arr4,i,&zlong4);
    }

    ZVAL_LONG(&call_params[1],i1);

    ZVAL_ARR(&call_params[2],arr1);
    ZVAL_ARR(&call_params[3],arr2);
    ZVAL_ARR(&call_params[4],arr3);
    ZVAL_ARR(&call_params[5],arr4);

    return event_base(event_name,ih,6,&call_params);
}


int event_common_iiiis(char * event_name, Ihandle *ih , int i1, int i2, int i3, int i4, char * str)
{
    zval call_params[6];

    zend_string * zstring;
    zstring = zend_string_init(str, strlen(str), 0);

    ZVAL_LONG(&call_params[1],i1);
    ZVAL_LONG(&call_params[2],i2);
    ZVAL_LONG(&call_params[3],i3);
    ZVAL_LONG(&call_params[4],i4);

    ZVAL_STR(&call_params[5],zstring);

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

int event_dialog_close_cb( Ihandle *ih){
    return event_common("CLOSE_CB",ih);
}

int event_dialog_copydata_cb( Ihandle *ih, char* cmdLine, int size){
    return event_common_si("COPYDATA_CB",ih,cmdLine,size);
}

int event_dialog_customframe_cb( Ihandle *ih){
    return event_common("CUSTOMFRAME_CB",ih);
}

int event_dialog_customframeactivate_cb( Ihandle *ih, int active){
    return event_common_i("CUSTOMFRAMEACTIVATE_CB",ih,active);
}

int event_dialog_mdiactivate_cb( Ihandle *ih){
    return event_common("MDIACTIVATE_CB",ih);
}

int event_dialog_show_cb( Ihandle *ih, int state){
    return event_common_i("SHOW_CB",ih,state);
}

// 各个控件的事件

int event_elements_action_cb( Ihandle *ih ){
    return event_common("ACTION_CB",ih);
}

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
    return event_common_iIIII("MULTITOUCH_CB",ih,count,pid,px,py,pstate);
}


int event_elements_wheel_cb( Ihandle *ih, float delta, int x, int y, char *status){
    return event_common_fiis("WHEEL_CB",ih,delta,x,y,status);
}

int event_elements_wom_cb( Ihandle *ih, int state){
    return event_common_i("WOM_CB",ih,state);
}

int event_elements_dropfiles_cb( Ihandle *ih, char* filename, int num, int x, int y){
    return event_common_siii("DROPFILES_CB",ih,filename,num,x,y);
}

int event_elements_dragbegin_cb( Ihandle *ih, int x, int y){
    return event_common_ii("DRAGBEGIN_CB",ih,x,y);
}

int event_elements_dragdatasize_cb( Ihandle *ih, char* type){
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

int event_elements_move_cb( Ihandle *ih, int x, int y){
    return event_common_ii("MOVE_CB",ih,x,y);
}


int event_elements_trayclick_cb( Ihandle *ih, int but, int pressed, int dclick){
    return event_common_iii("TRAYCLICK_CB",ih,but,pressed,dclick);
}

int event_elements_caret_cb( Ihandle *ih, int lin, int col, int pos){
    return event_common_iii("CARET_CB",ih,lin,col,pos);
}

int event_elements_dblclick_cb( Ihandle *ih, int item, char *text){
    return event_common_is("DBLCLICK_CB",ih,item,text);
}

int event_elements_dragdrop_cb( Ihandle *ih, int drag_id, int drop_id, int isshift, int iscontrol){
    return event_common_iiii("DRAGDROP_CB",ih,drag_id,drop_id,isshift,iscontrol);
}

int event_elements_edit_cb( Ihandle *ih, int c, char *new_value){
    return event_common_is("EDIT_CB",ih,c,new_value);
}

int event_elements_multiselect_cb( Ihandle *ih, char *value){
    return event_common_s("MULTISELECT_CB",ih,value);
}

int event_elements_spin_cb( Ihandle *ih, int pos){
    return event_common_i("SPIN_CB",ih,pos);
}

int event_elements_tabchange_cb( Ihandle *ih, Ihandle* new_tab, Ihandle* old_tab){
    return event_common_nn("TABCHANGE_CB",ih,new_tab,old_tab);
}

int event_elements_tabchangepos_cb( Ihandle *ih, int new_pos, int old_pos){
    return event_common_ii("TABCHANGEPOS_CB",ih,new_pos,old_pos);
}

int event_elements_tabclose_cb( Ihandle *ih, int pos){
    return event_common_i("TABCLOSE_CB",ih,pos);
}

int event_elements_rightclick_cb( Ihandle *ih, int pos){
    return event_common_i("RIGHTCLICK_CB",ih,pos);
}

int event_elements_selection_cb( Ihandle *ih, int id, int status){
    return event_common_ii("SELECTION_CB",ih,id,status);
}

int event_elements_multiselection_cb( Ihandle *ih, int* ids, int n){
    return event_common_Ii("MULTISELECTION_CB",ih,ids,n);
}


int event_elements_multiunselection_cb( Ihandle *ih, int* ids, int n){
    return event_common_Ii("MULTIUNSELECTION_CB",ih,ids,n);
}

int event_elements_branchopen_cb( Ihandle *ih, int id){
    return event_common_i("BRANCHOPEN_CB",ih,id);
}

int event_elements_branchclose_cb( Ihandle *ih, int id){
    return event_common_i("BRANCHCLOSE_CB",ih,id);
}

int event_elements_executeleaf_cb( Ihandle *ih, int id){
    return event_common_i("EXECUTELEAF_CB",ih,id);
}

int event_elements_showrename_cb( Ihandle *ih, int id){
    return event_common_i("SHOWRENAME_CB",ih,id);
}

int event_elements_rename_cb( Ihandle *ih, int id, char *title){
    return event_common_is("RENAME_CB",ih,id,title);
}

int event_elements_noderemoved_cb( Ihandle *ih, void* userdata){
    return event_common_i("NODEREMOVED_CB",ih,(int)userdata);
}

int event_elements_togglevalue_cb( Ihandle *ih, int id, int state){
    return event_common_ii("TOGGLEVALUE_CB",ih,id,state);
}

int event_elements_cell_cb( Ihandle *ih, int cell){
    return event_common_i("CELL_CB",ih,cell);
}

int event_elements_extended_cb( Ihandle *ih, int cell){
    return event_common_i("EXTENDED_CB",ih,cell);
}

int event_elements_select_cb( Ihandle *ih, int cell, int type){
    return event_common_ii("SELECT_CB",ih,cell,type);
}

int event_elements_switch_cb( Ihandle *ih, int prim_cell, int sec_cell){
    return event_common_ii("SWITCH_CB",ih,prim_cell,sec_cell);
}

int event_elements_button_press_cb( Ihandle *ih, double angle){
    return event_common_d("BUTTON_PRESS_CB",ih,angle);
}

int event_elements_button_release_cb( Ihandle *ih, double angle){
    return event_common_d("BUTTON_RELEASE_CB",ih,angle);
}

int event_elements_mousemove_cb( Ihandle *ih, double angle){
    return event_common_d("MOUSEMOVE_CB",ih,angle);
}

int event_elements_change_cb( Ihandle *ih, unsigned char r, unsigned char g, unsigned char b){
    return event_common_ccc("CHANGE_CB",ih,r,g,b);
}

int event_elements_drag_cb( Ihandle *ih, unsigned char r, unsigned char g, unsigned char b){
    return event_common_ccc("DRAG_CB",ih,r,g,b);
}

// =============================================================================
void event_register_callback()
{
    zval event_callback;

    // ======================================== common事件 ========================================
    ZVAL_PTR(&event_callback,(Icallback) event_common_action);
    zend_hash_str_add_new(iup_callback,"expander_ACTION",strlen("expander_ACTION"),&event_callback);
    zend_hash_str_add_new(iup_callback,"item_ACTION",strlen("item_ACTION"),&event_callback);
    zend_hash_str_add_new(iup_callback,"button_ACTION",strlen("button_ACTION"),&event_callback);
    zend_hash_str_add_new(iup_callback,"canvas_ACTION",strlen("canvas_ACTION"),&event_callback);
    zend_hash_str_add_new(iup_callback,"dialog_ACTION",strlen("dialog_ACTION"),&event_callback);
    zend_hash_str_add_new(iup_callback,"list_ACTION",strlen("list_ACTION"),&event_callback);
    zend_hash_str_add_new(iup_callback,"text_ACTION",strlen("text_ACTION"),&event_callback);
    zend_hash_str_add_new(iup_callback,"toggle_ACTION",strlen("toggle_ACTION"),&event_callback);
    zend_hash_str_add_new(iup_callback,"link_ACTION",strlen("link_ACTION"),&event_callback);

    ZVAL_PTR(&event_callback,(Icallback) event_flat_action);
    zend_hash_str_add_new(iup_callback,"flatbutton_FLAT_ACTION",strlen("flatbutton_FLAT_ACTION"),&event_callback);
    zend_hash_str_add_new(iup_callback,"flattoggle_FLAT_ACTION",strlen("flattoggle_FLAT_ACTION"),&event_callback);
    zend_hash_str_add_new(iup_callback,"dropbutton_FLAT_ACTION",strlen("dropbutton_FLAT_ACTION"),&event_callback);
    zend_hash_str_add_new(iup_callback,"flattabs_FLAT_ACTION",strlen("flattabs_FLAT_ACTION"),&event_callback);
    zend_hash_str_add_new(iup_callback,"flatseparator_FLAT_ACTION",strlen("flatseparator_ACTION"),&event_callback);

    // DESTROY_CB
    ZVAL_PTR(&event_callback,(Icallback) event_common_destroy_cb);
    zend_hash_str_add_new(iup_callback,"frame_DESTROY_CB",strlen("frame_DESTROY_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"item_DESTROY_CB",strlen("item_DESTROY_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"submenu_DESTROY_CB",strlen("submenu_DESTROY_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"menu_DESTROY_CB",strlen("menu_DESTROY_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"button_DESTROY_CB",strlen("button_DESTROY_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"canvas_DESTROY_CB",strlen("canvas_DESTROY_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"flatseparator_DESTROY_CB",strlen("flatseparator_DESTROY_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"dialog_DESTROY_CB",strlen("dialog_DESTROY_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"list_DESTROY_CB",strlen("list_DESTROY_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"text_DESTROY_CB",strlen("text_DESTROY_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"val_DESTROY_CB",strlen("val_DESTROY_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"tabs_DESTROY_CB",strlen("tabs_DESTROY_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"toggle_DESTROY_CB",strlen("toggle_DESTROY_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"tree_DESTROY_CB",strlen("tree_DESTROY_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"datepick_DESTROY_CB",strlen("datepick_DESTROY_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"calendar_DESTROY_CB",strlen("calendar_DESTROY_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"colorbar_DESTROY_CB",strlen("colorbar_DESTROY_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"dial_DESTROY_CB",strlen("dial_DESTROY_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"colorbrowser_DESTROY_CB",strlen("colorbrowser_DESTROY_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"gauge_DESTROY_CB",strlen("gauge_DESTROY_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"label_DESTROY_CB",strlen("label_DESTROY_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"animatedlabel_DESTROY_CB",strlen("animatedlabel_DESTROY_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"progressbar_DESTROY_CB",strlen("progressbar_DESTROY_CB"),&event_callback);

    ZVAL_PTR(&event_callback,(Icallback) event_flat_destroy_cb);
    zend_hash_str_add_new(iup_callback,"flatbutton_FLAT_DESTROY_CB",strlen("flatbutton_FLAT_DESTROY_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"flattoggle_FLAT_DESTROY_CB",strlen("flattoggle_FLAT_DESTROY_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"dropbutton_FLAT_DESTROY_CB",strlen("dropbutton_FLAT_DESTROY_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"flattabs_FLAT_DESTROY_CB",strlen("flattabs_FLAT_DESTROY_CB"),&event_callback);
    
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
    zend_hash_str_add_new(iup_callback,"flatseparator_MAP_CB",strlen("flatseparator_MAP_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"dialog_MAP_CB",strlen("dialog_MAP_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"list_MAP_CB",strlen("list_MAP_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"text_MAP_CB",strlen("text_MAP_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"val_MAP_CB",strlen("val_MAP_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"tabs_MAP_CB",strlen("tabs_MAP_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"toggle_MAP_CB",strlen("toggle_MAP_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"tree_MAP_CB",strlen("tree_MAP_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"datepick_MAP_CB",strlen("datepick_MAP_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"calendar_MAP_CB",strlen("calendar_MAP_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"colorbar_MAP_CB",strlen("colorbar_MAP_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"dial_MAP_CB",strlen("dial_MAP_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"colorbrowser_MAP_CB",strlen("colorbrowser_MAP_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"gauge_MAP_CB",strlen("gauge_MAP_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"label_MAP_CB",strlen("label_MAP_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"animatedlabel_MAP_CB",strlen("animatedlabel_MAP_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"progressbar_MAP_CB",strlen("progressbar_MAP_CB"),&event_callback);

    ZVAL_PTR(&event_callback,(Icallback) event_flat_map_cb);
    zend_hash_str_add_new(iup_callback,"flatbutton_FLAT_MAP_CB",strlen("flatbutton_FLAT_MAP_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"flattoggle_FLAT_MAP_CB",strlen("flattoggle_FLAT_MAP_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"dropbutton_FLAT_MAP_CB",strlen("dropbutton_FLAT_MAP_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"flattabs_FLAT_MAP_CB",strlen("flattabs_FLAT_MAP_CB"),&event_callback);
    
    // UNMAP_CB
    ZVAL_PTR(&event_callback,(Icallback) event_common_unmap_cb);
    zend_hash_str_add_new(iup_callback,"frame_UNMAP_CB",strlen("frame_UNMAP_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"item_UNMAP_CB",strlen("item_UNMAP_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"submenu_UNMAP_CB",strlen("submenu_UNMAP_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"menu_UNMAP_CB",strlen("menu_UNMAP_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"button_UNMAP_CB",strlen("button_UNMAP_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"canvas_UNMAP_CB",strlen("canvas_UNMAP_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"flatseparator_UNMAP_CB",strlen("flatseparator_UNMAP_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"dialog_UNMAP_CB",strlen("dialog_UNMAP_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"list_UNMAP_CB",strlen("list_UNMAP_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"text_UNMAP_CB",strlen("text_UNMAP_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"val_UNMAP_CB",strlen("val_UNMAP_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"tabs_UNMAP_CB",strlen("tabs_UNMAP_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"toggle_UNMAP_CB",strlen("toggle_UNMAP_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"tree_UNMAP_CB",strlen("tree_UNMAP_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"datepick_UNMAP_CB",strlen("datepick_UNMAP_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"calendar_UNMAP_CB",strlen("calendar_UNMAP_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"colorbar_UNMAP_CB",strlen("colorbar_UNMAP_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"dial_UNMAP_CB",strlen("dial_UNMAP_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"colorbrowser_UNMAP_CB",strlen("colorbrowser_UNMAP_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"gauge_UNMAP_CB",strlen("gauge_UNMAP_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"label_UNMAP_CB",strlen("label_UNMAP_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"animatedlabel_UNMAP_CB",strlen("animatedlabel_UNMAP_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"progressbar_UNMAP_CB",strlen("progressbar_UNMAP_CB"),&event_callback);

    ZVAL_PTR(&event_callback,(Icallback) event_flat_unmap_cb);
    zend_hash_str_add_new(iup_callback,"flatbutton_FLAT_UNMAP_CB",strlen("flatbutton_FLAT_UNMAP_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"flattoggle_FLAT_UNMAP_CB",strlen("flattoggle_FLAT_UNMAP_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"dropbutton_FLAT_UNMAP_CB",strlen("dropbutton_FLAT_UNMAP_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"flattabs_FLAT_UNMAP_CB",strlen("flattabs_FLAT_UNMAP_CB"),&event_callback);

    // GETFOCUS_CB
    ZVAL_PTR(&event_callback,(Icallback) event_common_getfocus_cb);
    zend_hash_str_add_new(iup_callback,"button_GETFOCUS_CB",strlen("button_GETFOCUS_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"canvas_GETFOCUS_CB",strlen("canvas_GETFOCUS_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"flatseparator_GETFOCUS_CB",strlen("flatseparator_GETFOCUS_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"dialog_GETFOCUS_CB",strlen("dialog_GETFOCUS_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"list_GETFOCUS_CB",strlen("list_GETFOCUS_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"text_GETFOCUS_CB",strlen("text_GETFOCUS_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"val_GETFOCUS_CB",strlen("val_GETFOCUS_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"tabs_GETFOCUS_CB",strlen("tabs_GETFOCUS_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"toggle_GETFOCUS_CB",strlen("toggle_GETFOCUS_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"tree_GETFOCUS_CB",strlen("tree_GETFOCUS_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"datepick_GETFOCUS_CB",strlen("datepick_GETFOCUS_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"calendar_GETFOCUS_CB",strlen("calendar_GETFOCUS_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"colorbar_GETFOCUS_CB",strlen("colorbar_GETFOCUS_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"dial_GETFOCUS_CB",strlen("dial_GETFOCUS_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"colorbrowser_GETFOCUS_CB",strlen("colorbrowser_GETFOCUS_CB"),&event_callback);

    ZVAL_PTR(&event_callback,(Icallback) event_flat_getfocus_cb);
    zend_hash_str_add_new(iup_callback,"flatbutton_FLAT_GETFOCUS_CB",strlen("flatbutton_FLAT_GETFOCUS_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"flattoggle_FLAT_GETFOCUS_CB",strlen("flattoggle_FLAT_GETFOCUS_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"dropbutton_FLAT_GETFOCUS_CB",strlen("dropbutton_FLAT_GETFOCUS_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"flattabs_FLAT_GETFOCUS_CB",strlen("flattabs_FLAT_GETFOCUS_CB"),&event_callback);

    // KILLFOCUS_CB
    ZVAL_PTR(&event_callback,(Icallback) event_common_killfocus_cb);
    zend_hash_str_add_new(iup_callback,"button_KILLFOCUS_CB",strlen("button_KILLFOCUS_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"canvas_KILLFOCUS_CB",strlen("canvas_KILLFOCUS_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"flatseparator_KILLFOCUS_CB",strlen("flatseparator_KILLFOCUS_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"dialog_KILLFOCUS_CB",strlen("dialog_KILLFOCUS_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"list_KILLFOCUS_CB",strlen("list_KILLFOCUS_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"text_KILLFOCUS_CB",strlen("text_KILLFOCUS_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"val_KILLFOCUS_CB",strlen("val_KILLFOCUS_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"tabs_KILLFOCUS_CB",strlen("tabs_KILLFOCUS_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"toggle_KILLFOCUS_CB",strlen("toggle_KILLFOCUS_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"tree_KILLFOCUS_CB",strlen("tree_KILLFOCUS_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"datepick_KILLFOCUS_CB",strlen("datepick_KILLFOCUS_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"calendar_KILLFOCUS_CB",strlen("calendar_KILLFOCUS_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"colorbar_KILLFOCUS_CB",strlen("colorbar_KILLFOCUS_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"dial_KILLFOCUS_CB",strlen("dial_KILLFOCUS_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"colorbrowser_KILLFOCUS_CB",strlen("colorbrowser_KILLFOCUS_CB"),&event_callback);

    ZVAL_PTR(&event_callback,(Icallback) event_flat_killfocus_cb);
    zend_hash_str_add_new(iup_callback,"flatbutton_FLAT_KILLFOCUS_CB",strlen("flatbutton_FLAT_KILLFOCUS_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"flattoggle_FLAT_KILLFOCUS_CB",strlen("flattoggle_FLAT_KILLFOCUS_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"dropbutton_FLAT_KILLFOCUS_CB",strlen("dropbutton_FLAT_KILLFOCUS_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"flattabs_FLAT_KILLFOCUS_CB",strlen("flattabs_FLAT_KILLFOCUS_CB"),&event_callback);

    // ENTERWINDOW_CB
    ZVAL_PTR(&event_callback,(Icallback) event_common_enterwindow_cb);
    zend_hash_str_add_new(iup_callback,"button_ENTERWINDOW_CB",strlen("button_ENTERWINDOW_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"canvas_ENTERWINDOW_CB",strlen("canvas_ENTERWINDOW_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"flatseparator_ENTERWINDOW_CB",strlen("flatseparator_ENTERWINDOW_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"dialog_ENTERWINDOW_CB",strlen("dialog_ENTERWINDOW_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"list_ENTERWINDOW_CB",strlen("list_ENTERWINDOW_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"text_ENTERWINDOW_CB",strlen("text_ENTERWINDOW_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"val_ENTERWINDOW_CB",strlen("val_ENTERWINDOW_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"tabs_ENTERWINDOW_CB",strlen("tabs_ENTERWINDOW_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"toggle_ENTERWINDOW_CB",strlen("toggle_ENTERWINDOW_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"tree_ENTERWINDOW_CB",strlen("tree_ENTERWINDOW_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"datepick_ENTERWINDOW_CB",strlen("datepick_ENTERWINDOW_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"calendar_ENTERWINDOW_CB",strlen("calendar_ENTERWINDOW_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"colorbar_ENTERWINDOW_CB",strlen("colorbar_ENTERWINDOW_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"dial_ENTERWINDOW_CB",strlen("dial_ENTERWINDOW_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"colorbrowser_ENTERWINDOW_CB",strlen("colorbrowser_ENTERWINDOW_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"label_ENTERWINDOW_CB",strlen("label_ENTERWINDOW_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"animatedlabel_ENTERWINDOW_CB",strlen("animatedlabel_ENTERWINDOW_CB"),&event_callback);

    ZVAL_PTR(&event_callback,(Icallback) event_flat_enterwindow_cb);
    zend_hash_str_add_new(iup_callback,"flatbutton_FLAT_ENTERWINDOW_CB",strlen("flatbutton_FLAT_ENTERWINDOW_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"flattoggle_FLAT_ENTERWINDOW_CB",strlen("flattoggle_FLAT_ENTERWINDOW_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"dropbutton_FLAT_ENTERWINDOW_CB",strlen("dropbutton_FLAT_ENTERWINDOW_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"flattabs_FLAT_ENTERWINDOW_CB",strlen("flattabs_FLAT_ENTERWINDOW_CB"),&event_callback);

    // LEAVEWINDOW_CB
    ZVAL_PTR(&event_callback,(Icallback) event_common_leavewindow_cb);
    zend_hash_str_add_new(iup_callback,"button_LEAVEWINDOW_CB",strlen("button_LEAVEWINDOW_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"canvas_LEAVEWINDOW_CB",strlen("canvas_LEAVEWINDOW_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"flatseparator_LEAVEWINDOW_CB",strlen("flatseparator_LEAVEWINDOW_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"dialog_LEAVEWINDOW_CB",strlen("dialog_LEAVEWINDOW_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"list_LEAVEWINDOW_CB",strlen("list_LEAVEWINDOW_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"text_LEAVEWINDOW_CB",strlen("text_LEAVEWINDOW_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"val_LEAVEWINDOW_CB",strlen("val_LEAVEWINDOW_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"tabs_LEAVEWINDOW_CB",strlen("tabs_LEAVEWINDOW_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"toggle_LEAVEWINDOW_CB",strlen("toggle_LEAVEWINDOW_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"tree_LEAVEWINDOW_CB",strlen("tree_LEAVEWINDOW_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"datepick_LEAVEWINDOW_CB",strlen("datepick_LEAVEWINDOW_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"calendar_LEAVEWINDOW_CB",strlen("calendar_LEAVEWINDOW_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"colorbar_LEAVEWINDOW_CB",strlen("colorbar_LEAVEWINDOW_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"dial_LEAVEWINDOW_CB",strlen("dial_LEAVEWINDOW_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"colorbrowser_LEAVEWINDOW_CB",strlen("colorbrowser_LEAVEWINDOW_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"label_LEAVEWINDOW_CB",strlen("label_LEAVEWINDOW_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"animatedlabel_LEAVEWINDOW_CB",strlen("animatedlabel_LEAVEWINDOW_CB"),&event_callback);

    ZVAL_PTR(&event_callback,(Icallback) event_flat_leavewindow_cb);
    zend_hash_str_add_new(iup_callback,"flatbutton_FLAT_LEAVEWINDOW_CB",strlen("flatbutton_FLAT_LEAVEWINDOW_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"flattoggle_FLAT_LEAVEWINDOW_CB",strlen("flattoggle_FLAT_LEAVEWINDOW_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"dropbutton_FLAT_LEAVEWINDOW_CB",strlen("dropbutton_FLAT_LEAVEWINDOW_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"flattabs_FLAT_LEAVEWINDOW_CB",strlen("flattabs_FLAT_LEAVEWINDOW_CB"),&event_callback);

    // HELP_CB
    ZVAL_PTR(&event_callback,(Icallback) event_common_help_cb);
    zend_hash_str_add_new(iup_callback,"item_HELP_CB",strlen("item_HELP_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"submenu_HELP_CB",strlen("submenu_HELP_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"menu_HELP_CB",strlen("menu_HELP_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"button_HELP_CB",strlen("button_HELP_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"canvas_HELP_CB",strlen("canvas_HELP_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"flatseparator_HELP_CB",strlen("flatseparator_HELP_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"dialog_HELP_CB",strlen("dialog_HELP_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"list_HELP_CB",strlen("list_HELP_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"text_HELP_CB",strlen("text_HELP_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"val_HELP_CB",strlen("val_HELP_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"tabs_HELP_CB",strlen("tabs_HELP_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"toggle_HELP_CB",strlen("toggle_HELP_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"tree_HELP_CB",strlen("tree_HELP_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"datepick_HELP_CB",strlen("datepick_HELP_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"calendar_HELP_CB",strlen("calendar_HELP_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"colorbar_HELP_CB",strlen("colorbar_HELP_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"dial_HELP_CB",strlen("dial_HELP_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"colorbrowser_HELP_CB",strlen("colorbrowser_HELP_CB"),&event_callback);

    ZVAL_PTR(&event_callback,(Icallback) event_flat_help_cb);
    zend_hash_str_add_new(iup_callback,"flatbutton_FLAT_HELP_CB",strlen("flatbutton_FLAT_HELP_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"flattoggle_FLAT_HELP_CB",strlen("flattoggle_FLAT_HELP_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"dropbutton_FLAT_HELP_CB",strlen("dropbutton_FLAT_HELP_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"flattabs_FLAT_HELP_CB",strlen("flattabs_FLAT_HELP_CB"),&event_callback);

    // K_ANY
    ZVAL_PTR(&event_callback,(Icallback) event_common_k_any);
    zend_hash_str_add_new(iup_callback,"button_K_ANY",strlen("button_K_ANY"),&event_callback);
    zend_hash_str_add_new(iup_callback,"canvas_K_ANY",strlen("canvas_K_ANY"),&event_callback);
    zend_hash_str_add_new(iup_callback,"flatseparator_K_ANY",strlen("flatseparator_K_ANY"),&event_callback);
    zend_hash_str_add_new(iup_callback,"dialog_K_ANY",strlen("dialog_K_ANY"),&event_callback);
    zend_hash_str_add_new(iup_callback,"list_K_ANY",strlen("list_K_ANY"),&event_callback);
    zend_hash_str_add_new(iup_callback,"text_K_ANY",strlen("text_K_ANY"),&event_callback);
    zend_hash_str_add_new(iup_callback,"val_K_ANY",strlen("val_K_ANY"),&event_callback);
    zend_hash_str_add_new(iup_callback,"tabs_K_ANY",strlen("tabs_K_ANY"),&event_callback);
    zend_hash_str_add_new(iup_callback,"toggle_K_ANY",strlen("toggle_K_ANY"),&event_callback);
    zend_hash_str_add_new(iup_callback,"tree_K_ANY",strlen("tree_K_ANY"),&event_callback);
    zend_hash_str_add_new(iup_callback,"datepick_K_ANY",strlen("datepick_K_ANY"),&event_callback);
    zend_hash_str_add_new(iup_callback,"calendar_K_ANY",strlen("calendar_K_ANY"),&event_callback);
    zend_hash_str_add_new(iup_callback,"colorbar_K_ANY",strlen("colorbar_K_ANY"),&event_callback);
    zend_hash_str_add_new(iup_callback,"dial_K_ANY",strlen("dial_K_ANY"),&event_callback);
    zend_hash_str_add_new(iup_callback,"colorbrowser_K_ANY",strlen("colorbrowser_K_ANY"),&event_callback);

    ZVAL_PTR(&event_callback,(Icallback) event_flat_k_any);
    zend_hash_str_add_new(iup_callback,"flatbutton_FLAT_K_ANY",strlen("flatbutton_FLAT_K_ANY"),&event_callback);
    zend_hash_str_add_new(iup_callback,"flattoggle_FLAT_K_ANY",strlen("flattoggle_FLAT_K_ANY"),&event_callback);
    zend_hash_str_add_new(iup_callback,"dropbutton_FLAT_K_ANY",strlen("dropbutton_FLAT_K_ANY"),&event_callback);
    zend_hash_str_add_new(iup_callback,"flattabs_FLAT_K_ANY",strlen("flattabs_FLAT_K_ANY"),&event_callback);


    // ======================================== 其他可共用事件 ========================================

    ZVAL_PTR(&event_callback,(Icallback) event_elements_valuechanged_cb);
    zend_hash_str_add_new(iup_callback,"split_VALUECHANGED_CB",strlen("split_VALUECHANGED_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"flatbutton_VALUECHANGED_CB",strlen("flatbutton_VALUECHANGED_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"flattoggle_VALUECHANGED_CB",strlen("flattoggle_VALUECHANGED_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"dropbutton_VALUECHANGED_CB",strlen("dropbutton_VALUECHANGED_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"list_VALUECHANGED_CB",strlen("list_VALUECHANGED_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"text_VALUECHANGED_CB",strlen("text_VALUECHANGED_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"toggle_VALUECHANGED_CB",strlen("toggle_VALUECHANGED_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"val_VALUECHANGED_CB",strlen("val_VALUECHANGED_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"datepick_VALUECHANGED_CB",strlen("datepick_VALUECHANGED_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"calendar_VALUECHANGED_CB",strlen("calendar_VALUECHANGED_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"dial_VALUECHANGED_CB",strlen("dial_VALUECHANGED_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"colorbrowser_VALUECHANGED_CB",strlen("colorbrowser_VALUECHANGED_CB"),&event_callback);

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
    zend_hash_str_add_new(iup_callback,"dialog_FOCUS_CB",strlen("dialog_FOCUS_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"flatseparator_FOCUS_CB",strlen("flatseparator_FOCUS_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"tabs_FOCUS_CB",strlen("tabs_FOCUS_CB"),&event_callback);


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
    zend_hash_str_add_new(iup_callback,"flatseparator_BUTTON_CB",strlen("flatseparator_BUTTON_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"label_BUTTON_CB",strlen("label_BUTTON_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"animatedlabel_BUTTON_CB",strlen("animatedlabel_BUTTON_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"list_BUTTON_CB",strlen("list_BUTTON_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"text_BUTTON_CB",strlen("text_BUTTON_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"tree_BUTTON_CB",strlen("tree_BUTTON_CB"),&event_callback);

    ZVAL_PTR(&event_callback,(Icallback) event_flat_button_cb);
    zend_hash_str_add_new(iup_callback,"flatbutton_FLAT_BUTTON_CB",strlen("flatbutton_FLAT_BUTTON_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"flattoggle_FLAT_BUTTON_CB",strlen("flattoggle_FLAT_BUTTON_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"dropbutton_FLAT_BUTTON_CB",strlen("dropbutton_FLAT_BUTTON_CB"),&event_callback);

    
    // DROPDOWN_CB
    ZVAL_PTR(&event_callback,(Icallback) event_elements_dropdown_cb);
    zend_hash_str_add_new(iup_callback,"dropbutton_DROPDOWN_CB",strlen("dropbutton_DROPDOWN_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"list_DROPDOWN_CB",strlen("list_DROPDOWN_CB"),&event_callback);

    // DROPSHOW_CB
    ZVAL_PTR(&event_callback,(Icallback) event_elements_dropshow_cb);
    zend_hash_str_add_new(iup_callback,"dropbutton_DROPSHOW_CB",strlen("dropbutton_DROPSHOW_CB"),&event_callback);

    // DRAGDROP_CB
    ZVAL_PTR(&event_callback,(Icallback) event_elements_dragdrop_cb);
    zend_hash_str_add_new(iup_callback,"list_DRAGDROP_CB",strlen("list_DRAGDROP_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"tree_DRAGDROP_CB",strlen("tree_DRAGDROP_CB"),&event_callback);

    // MOTION_CB
    ZVAL_PTR(&event_callback,(Icallback) event_elements_motion_cb);
    zend_hash_str_add_new(iup_callback,"canvas_MOTION_CB",strlen("canvas_MOTION_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"flatseparator_MOTION_CB",strlen("flatseparator_MOTION_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"label_MOTION_CB",strlen("label_MOTION_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"animatedlabel_MOTION_CB",strlen("animatedlabel_MOTION_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"list_MOTION_CB",strlen("list_MOTION_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"text_MOTION_CB",strlen("text_MOTION_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"tree_MOTION_CB",strlen("tree_MOTION_CB"),&event_callback);

    // KEYPRESS_CB
    ZVAL_PTR(&event_callback,(Icallback) event_elements_keypress_cb);
    zend_hash_str_add_new(iup_callback,"canvas_KEYPRESS_CB",strlen("canvas_KEYPRESS_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"flatseparator_KEYPRESS_CB",strlen("flatseparator_KEYPRESS_CB"),&event_callback);

    // RESIZE_CB
    ZVAL_PTR(&event_callback,(Icallback) event_elements_resize_cb);
    zend_hash_str_add_new(iup_callback,"dialog_RESIZE_CB",strlen("dialog_RESIZE_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"canvas_RESIZE_CB",strlen("canvas_RESIZE_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"flatseparator_RESIZE_CB",strlen("flatseparator_RESIZE_CB"),&event_callback);

    // SCROLL_CB
    ZVAL_PTR(&event_callback,(Icallback) event_elements_scroll_cb);
    zend_hash_str_add_new(iup_callback,"canvas_SCROLL_CB",strlen("canvas_SCROLL_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"flatseparator_SCROLL_CB",strlen("flatseparator_SCROLL_CB"),&event_callback);

    // TOUCH_CB
    ZVAL_PTR(&event_callback,(Icallback) event_elements_touch_cb);
    zend_hash_str_add_new(iup_callback,"canvas_TOUCH_CB",strlen("canvas_TOUCH_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"flatseparator_TOUCH_CB",strlen("flatseparator_TOUCH_CB"),&event_callback);

    // MULTITOUCH_CB
    ZVAL_PTR(&event_callback,(Icallback) event_elements_multitouch_cb);
    zend_hash_str_add_new(iup_callback,"canvas_MULTITOUCH_CB",strlen("canvas_MULTITOUCH_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"flatseparator_MULTITOUCH_CB",strlen("flatseparator_MULTITOUCH_CB"),&event_callback);

    // WHEEL_CB
    ZVAL_PTR(&event_callback,(Icallback) event_elements_wheel_cb);
    zend_hash_str_add_new(iup_callback,"canvas_WHEEL_CB",strlen("canvas_WHEEL_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"flatseparator_WHEEL_CB",strlen("flatseparator_WHEEL_CB"),&event_callback);

    // WOM_CB
    ZVAL_PTR(&event_callback,(Icallback) event_elements_wom_cb);
    zend_hash_str_add_new(iup_callback,"canvas_WOM_CB",strlen("canvas_WOM_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"flatseparator_WOM_CB",strlen("flatseparator_WOM_CB"),&event_callback);

    // DROPFILES_CB
    ZVAL_PTR(&event_callback,(Icallback) event_elements_dropfiles_cb);
    zend_hash_str_add_new(iup_callback,"dialog_DROPFILES_CB",strlen("dialog_DROPFILES_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"canvas_DROPFILES_CB",strlen("canvas_DROPFILES_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"flatseparator_DROPFILES_CB",strlen("flatseparator_DROPFILES_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"glcanvas_DROPFILES_CB",strlen("glcanvas_DROPFILES_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"text_DROPFILES_CB",strlen("text_DROPFILES_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"list_DROPFILES_CB",strlen("list_DROPFILES_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"label_DROPFILES_CB",strlen("label_DROPFILES_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"animatedlabel_DROPFILES_CB",strlen("animatedlabel_DROPFILES_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"tree_DROPFILES_CB",strlen("tree_DROPFILES_CB"),&event_callback);

    // DRAGBEGIN_CB
    ZVAL_PTR(&event_callback,(Icallback) event_elements_dragbegin_cb);
    zend_hash_str_add_new(iup_callback,"dialog_DRAGBEGIN_CB",strlen("dialog_DRAGBEGIN_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"canvas_DRAGBEGIN_CB",strlen("canvas_DRAGBEGIN_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"flatseparator_DRAGBEGIN_CB",strlen("flatseparator_DRAGBEGIN_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"label_DRAGBEGIN_CB",strlen("label_DRAGBEGIN_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"animatedlabel_DRAGBEGIN_CB",strlen("animatedlabel_DRAGBEGIN_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"text_DRAGBEGIN_CB",strlen("text_DRAGBEGIN_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"list_DRAGBEGIN_CB",strlen("list_DRAGBEGIN_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"tree_DRAGBEGIN_CB",strlen("tree_DRAGBEGIN_CB"),&event_callback);

    // DRAGDATASIZE_CB
    ZVAL_PTR(&event_callback,(Icallback) event_elements_dragdatasize_cb);
    zend_hash_str_add_new(iup_callback,"dialog_DRAGDATASIZE_CB",strlen("dialog_DRAGDATASIZE_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"canvas_DRAGDATASIZE_CB",strlen("canvas_DRAGDATASIZE_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"flatseparator_DRAGDATASIZE_CB",strlen("flatseparator_DRAGDATASIZE_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"label_DRAGDATASIZE_CB",strlen("label_DRAGDATASIZE_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"animatedlabel_DRAGDATASIZE_CB",strlen("animatedlabel_DRAGDATASIZE_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"text_DRAGDATASIZE_CB",strlen("text_DRAGDATASIZE_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"list_DRAGDATASIZE_CB",strlen("list_DRAGDATASIZE_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"tree_DRAGDATASIZE_CB",strlen("tree_DRAGDATASIZE_CB"),&event_callback);

    // DRAGDATA_CB
    ZVAL_PTR(&event_callback,(Icallback) event_elements_dragdata_cb);
    zend_hash_str_add_new(iup_callback,"dialog_DRAGDATA_CB",strlen("dialog_DRAGDATA_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"canvas_DRAGDATA_CB",strlen("canvas_DRAGDATA_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"flatseparator_DRAGDATA_CB",strlen("flatseparator_DRAGDATA_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"label_DRAGDATA_CB",strlen("label_DRAGDATA_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"animatedlabel_DRAGDATA_CB",strlen("animatedlabel_DRAGDATA_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"text_DRAGDATA_CB",strlen("text_DRAGDATA_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"list_DRAGDATA_CB",strlen("list_DRAGDATA_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"tree_DRAGDATA_CB",strlen("tree_DRAGDATA_CB"),&event_callback);

    // DRAGEND_CB
    ZVAL_PTR(&event_callback,(Icallback) event_elements_dragend_cb);
    zend_hash_str_add_new(iup_callback,"dialog_DRAGEND_CB",strlen("dialog_DRAGEND_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"canvas_DRAGEND_CB",strlen("canvas_DRAGEND_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"flatseparator_DRAGEND_CB",strlen("flatseparator_DRAGEND_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"label_DRAGEND_CB",strlen("label_DRAGEND_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"animatedlabel_DRAGEND_CB",strlen("animatedlabel_DRAGEND_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"text_DRAGEND_CB",strlen("text_DRAGEND_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"list_DRAGEND_CB",strlen("list_DRAGEND_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"tree_DRAGEND_CB",strlen("tree_DRAGEND_CB"),&event_callback);

    // DROPDATA_CB
    ZVAL_PTR(&event_callback,(Icallback) event_elements_dropdata_cb);
    zend_hash_str_add_new(iup_callback,"dialog_DROPDATA_CB",strlen("dialog_DROPDATA_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"canvas_DROPDATA_CB",strlen("canvas_DROPDATA_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"flatseparator_DROPDATA_CB",strlen("flatseparator_DROPDATA_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"label_DROPDATA_CB",strlen("label_DROPDATA_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"animatedlabel_DROPDATA_CB",strlen("animatedlabel_DROPDATA_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"text_DROPDATA_CB",strlen("text_DROPDATA_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"list_DROPDATA_CB",strlen("list_DROPDATA_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"tree_DROPDATA_CB",strlen("tree_DROPDATA_CB"),&event_callback);

    // DROPMOTION_CB
    ZVAL_PTR(&event_callback,(Icallback) event_elements_dropmotion_cb);
    zend_hash_str_add_new(iup_callback,"dialog_DROPMOTION_CB",strlen("dialog_DROPMOTION_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"canvas_DROPMOTION_CB",strlen("canvas_DROPMOTION_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"flatseparator_DROPMOTION_CB",strlen("flatseparator_DROPMOTION_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"label_DROPMOTION_CB",strlen("label_DROPMOTION_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"animatedlabel_DROPMOTION_CB",strlen("animatedlabel_DROPMOTION_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"text_DROPMOTION_CB",strlen("text_DROPMOTION_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"list_DROPMOTION_CB",strlen("list_DROPMOTION_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"tree_DROPMOTION_CB",strlen("tree_DROPMOTION_CB"),&event_callback);

    // MOVE_CB
    ZVAL_PTR(&event_callback,(Icallback) event_elements_move_cb);
    zend_hash_str_add_new(iup_callback,"dialog_MOVE_CB",strlen("dialog_MOVE_CB"),&event_callback);


    // TRAYCLICK_CB
    ZVAL_PTR(&event_callback,(Icallback) event_elements_trayclick_cb);
    zend_hash_str_add_new(iup_callback,"dialog_TRAYCLICK_CB",strlen("dialog_TRAYCLICK_CB"),&event_callback);

    // CARET_CB
    ZVAL_PTR(&event_callback,(Icallback) event_elements_caret_cb);
    zend_hash_str_add_new(iup_callback,"list_CARET_CB",strlen("list_CARET_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"text_CARET_CB",strlen("text_CARET_CB"),&event_callback);

    // SPIN_CB
    ZVAL_PTR(&event_callback,(Icallback) event_elements_spin_cb);
    zend_hash_str_add_new(iup_callback,"text_SPIN_CB",strlen("text_SPIN_CB"),&event_callback);

    // TABCHANGE_CB
    ZVAL_PTR(&event_callback,(Icallback) event_elements_tabchange_cb);
    zend_hash_str_add_new(iup_callback,"tabs_TABCHANGE_CB",strlen("tabs_TABCHANGE_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"flattabs_TABCHANGE_CB",strlen("flattabs_TABCHANGE_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"colorbrowser_TABCHANGE_CB",strlen("colorbrowser_TABCHANGE_CB"),&event_callback);

    // TABCHANGEPOS_CB
    ZVAL_PTR(&event_callback,(Icallback) event_elements_tabchangepos_cb);
    zend_hash_str_add_new(iup_callback,"tabs_TABCHANGEPOS_CB",strlen("tabs_TABCHANGEPOS_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"flattabs_TABCHANGEPOS_CB",strlen("flattabs_TABCHANGEPOS_CB"),&event_callback);

    // TABCLOSE_CB
    ZVAL_PTR(&event_callback,(Icallback) event_elements_tabclose_cb);
    zend_hash_str_add_new(iup_callback,"tabs_TABCLOSE_CB",strlen("tabs_TABCLOSE_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"flattabs_TABCLOSE_CB",strlen("flattabs_TABCLOSE_CB"),&event_callback);

    // RIGHTCLICK_CB
    ZVAL_PTR(&event_callback,(Icallback) event_elements_rightclick_cb);
    zend_hash_str_add_new(iup_callback,"tabs_RIGHTCLICK_CB",strlen("tabs_RIGHTCLICK_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"flattabs_RIGHTCLICK_CB",strlen("flattabs_RIGHTCLICK_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"tree_RIGHTCLICK_CB",strlen("tree_RIGHTCLICK_CB"),&event_callback);

    ZVAL_PTR(&event_callback,(Icallback) event_elements_extrabutton_cb);
    zend_hash_str_add_new(iup_callback,"expander_EXTRABUTTON_CB",strlen("expander_EXTRABUTTON_CB"),&event_callback);
    zend_hash_str_add_new(iup_callback,"flattabs_EXTRABUTTON_CB",strlen("flattabs_EXTRABUTTON_CB"),&event_callback);

    // ======================================== 独立事件 ========================================

    // Ihandle*  IupExpander   (Ihandle* child);
    ZVAL_PTR(&event_callback,(Icallback) event_elements_openclose_cb);
    zend_hash_str_add_new(iup_callback,"expander_OPENCLOSE_CB",strlen("expander_OPENCLOSE_CB"),&event_callback);

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

    // Ihandle*  IupDialog     (Ihandle* child);
    ZVAL_PTR(&event_callback,(Icallback) event_dialog_close_cb);
    zend_hash_str_add_new(iup_callback,"dialog_CLOSE_CB",strlen("dialog_CLOSE_CB"),&event_callback);
    
    ZVAL_PTR(&event_callback,(Icallback) event_dialog_copydata_cb);
    zend_hash_str_add_new(iup_callback,"dialog_COPYDATA_CB",strlen("dialog_COPYDATA_CB"),&event_callback);
    
    ZVAL_PTR(&event_callback,(Icallback) event_dialog_customframe_cb);
    zend_hash_str_add_new(iup_callback,"dialog_CUSTOMFRAME_CB",strlen("dialog_CUSTOMFRAME_CB"),&event_callback);    

    ZVAL_PTR(&event_callback,(Icallback) event_dialog_customframeactivate_cb);
    zend_hash_str_add_new(iup_callback,"dialog_CUSTOMFRAMEACTIVATE_CB",strlen("dialog_CUSTOMFRAMEACTIVATE_CB"),&event_callback);

    ZVAL_PTR(&event_callback,(Icallback) event_dialog_mdiactivate_cb);
    zend_hash_str_add_new(iup_callback,"dialog_MDIACTIVATE_CB",strlen("dialog_MDIACTIVATE_CB"),&event_callback);

    ZVAL_PTR(&event_callback,(Icallback) event_dialog_show_cb);
    zend_hash_str_add_new(iup_callback,"dialog_SHOW_CB",strlen("dialog_SHOW_CB"),&event_callback);


    // Ihandle*  IupList       (const char* action);
    ZVAL_PTR(&event_callback,(Icallback) event_elements_dblclick_cb);
    zend_hash_str_add_new(iup_callback,"list_DBLCLICK_CB",strlen("list_DBLCLICK_CB"),&event_callback);

    ZVAL_PTR(&event_callback,(Icallback) event_elements_edit_cb);
    zend_hash_str_add_new(iup_callback,"list_EDIT_CB",strlen("list_EDIT_CB"),&event_callback);

    ZVAL_PTR(&event_callback,(Icallback) event_elements_multiselect_cb);
    zend_hash_str_add_new(iup_callback,"list_MULTISELECT_CB",strlen("list_MULTISELECT_CB"),&event_callback);

    // Ihandle*  IupTimer      (void);
    ZVAL_PTR(&event_callback,(Icallback) event_elements_action_cb);
    zend_hash_str_add_new(iup_callback,"timer_ACTION_CB",strlen("timer_ACTION_CB"),&event_callback);

    // Ihandle*  IupTree       (void);
    ZVAL_PTR(&event_callback,(Icallback) event_elements_selection_cb);
    zend_hash_str_add_new(iup_callback,"tree_SELECTION_CB",strlen("tree_SELECTION_CB"),&event_callback);

    ZVAL_PTR(&event_callback,(Icallback) event_elements_multiselection_cb);
    zend_hash_str_add_new(iup_callback,"tree_MULTISELECTION_CB",strlen("tree_MULTISELECTION_CB"),&event_callback);

    ZVAL_PTR(&event_callback,(Icallback) event_elements_multiunselection_cb);
    zend_hash_str_add_new(iup_callback,"tree_MULTIUNSELECTION_CB",strlen("tree_MULTIUNSELECTION_CB"),&event_callback);

    ZVAL_PTR(&event_callback,(Icallback) event_elements_branchopen_cb);
    zend_hash_str_add_new(iup_callback,"tree_BRANCHOPEN_CB",strlen("tree_BRANCHOPEN_CB"),&event_callback);

    ZVAL_PTR(&event_callback,(Icallback) event_elements_branchclose_cb);
    zend_hash_str_add_new(iup_callback,"tree_BRANCHCLOSE_CB",strlen("tree_BRANCHCLOSE_CB"),&event_callback);

    ZVAL_PTR(&event_callback,(Icallback) event_elements_executeleaf_cb);
    zend_hash_str_add_new(iup_callback,"tree_EXECUTELEAF_CB",strlen("tree_EXECUTELEAF_CB"),&event_callback);

    ZVAL_PTR(&event_callback,(Icallback) event_elements_showrename_cb);
    zend_hash_str_add_new(iup_callback,"tree_SHOWRENAME_CB",strlen("tree_SHOWRENAME_CB"),&event_callback);

    ZVAL_PTR(&event_callback,(Icallback) event_elements_rename_cb);
    zend_hash_str_add_new(iup_callback,"tree_RENAME_CB",strlen("tree_RENAME_CB"),&event_callback);

    ZVAL_PTR(&event_callback,(Icallback) event_elements_noderemoved_cb);
    zend_hash_str_add_new(iup_callback,"tree_NODEREMOVED_CB",strlen("tree_NODEREMOVED_CB"),&event_callback);

    ZVAL_PTR(&event_callback,(Icallback) event_elements_togglevalue_cb);
    zend_hash_str_add_new(iup_callback,"tree_TOGGLEVALUE_CB",strlen("tree_TOGGLEVALUE_CB"),&event_callback);

    // Ihandle*  IupColorbar   (void);
    ZVAL_PTR(&event_callback,(Icallback) event_elements_togglevalue_cb);
    zend_hash_str_add_new(iup_callback,"colorbar_CELL_CB",strlen("colorbar_CELL_CB"),&event_callback);

    ZVAL_PTR(&event_callback,(Icallback) event_elements_togglevalue_cb);
    zend_hash_str_add_new(iup_callback,"colorbar_EXTENDED_CB",strlen("colorbar_EXTENDED_CB"),&event_callback);

    ZVAL_PTR(&event_callback,(Icallback) event_elements_select_cb);
    zend_hash_str_add_new(iup_callback,"colorbar_SELECT_CB",strlen("colorbar_SELECT_CB"),&event_callback);

    ZVAL_PTR(&event_callback,(Icallback) event_elements_switch_cb);
    zend_hash_str_add_new(iup_callback,"colorbar_SWITCH_CB",strlen("colorbar_SWITCH_CB"),&event_callback);

    // Ihandle*  IupDial       (const char* type);
    ZVAL_PTR(&event_callback,(Icallback) event_elements_button_press_cb);
    zend_hash_str_add_new(iup_callback,"dial_BUTTON_PRESS_CB",strlen("dial_BUTTON_PRESS_CB"),&event_callback);

    ZVAL_PTR(&event_callback,(Icallback) event_elements_button_release_cb);
    zend_hash_str_add_new(iup_callback,"dial_BUTTON_RELEASE_CB",strlen("dial_BUTTON_RELEASE_CB"),&event_callback);

    ZVAL_PTR(&event_callback,(Icallback) event_elements_mousemove_cb);
    zend_hash_str_add_new(iup_callback,"dial_MOUSEMOVE_CB",strlen("dial_MOUSEMOVE_CB"),&event_callback);


    // Ihandle*  IupColorBrowser(void);
    ZVAL_PTR(&event_callback,(Icallback) event_elements_change_cb);
    zend_hash_str_add_new(iup_callback,"colorbrowser_CHANGE_CB",strlen("colorbrowser_CHANGE_CB"),&event_callback);

    ZVAL_PTR(&event_callback,(Icallback) event_elements_drag_cb);
    zend_hash_str_add_new(iup_callback,"colorbrowser_DRAG_CB",strlen("colorbrowser_DRAG_CB"),&event_callback);


}