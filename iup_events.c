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

int event_common( Ihandle *self , char * event_name)
{
    intptr_t ih_p_int;

    char event_key_str[100];

    zend_string * event_key;

    zval * event_val;

    int call_result;

    zval fun_result;

    zend_long fun_result_value;

    zend_fcall_info * callable;

    ih_p_int = (intptr_t)self;

    sprintf(event_key_str,"%"SCNiPTR,ih_p_int);

    // 把两个字符串拼接
    strcat(event_key_str,"_");
    strcat(event_key_str,event_name);

    event_key = zend_string_init(event_key_str, strlen(event_key_str), 0);

    // 判断事件数组中是否存有相同事件id
    event_val = zend_hash_find(iup_events,event_key);

    if(event_val == NULL){
        // 没有相应的事件，直接返回
        return IUP_DEFAULT;
    }

    callable = zend_fetch_resource_ex(event_val,"iup-event",le_iup_event);

    callable->retval = &fun_result;

    call_result = zend_call_function(callable, NULL);

    fun_result_value = Z_LVAL(fun_result);

    if(
        fun_result_value == IUP_IGNORE || 
        fun_result_value == IUP_DEFAULT || 
        fun_result_value == IUP_CLOSE || 
        fun_result_value == IUP_CONTINUE
        )
    {

        return (int)fun_result_value;
    }

  return IUP_DEFAULT;

}

int event_action( Ihandle *self ){
    return event_common(self,"ACTION");
}
int event_action_cb( Ihandle *self ){
    return event_common(self,"ACTION_CB");
}
int event_autoccancelled_cb( Ihandle *self ){
    return event_common(self,"AUTOCCANCELLED_CB");
}
int event_autocchardeleted_cb( Ihandle *self ){
    return event_common(self,"AUTOCCHARDELETED_CB");
}
int event_autocselection_cb( Ihandle *self ){
    return event_common(self,"AUTOCSELECTION_CB");
}
int event_busy_cb( Ihandle *self ){
    return event_common(self,"BUSY_CB");
}
int event_button_cb( Ihandle *self ){
    return event_common(self,"BUTTON_CB");
}
int event_button_press_cb( Ihandle *self ){
    return event_common(self,"BUTTON_PRESS_CB");
}
int event_button_release_cb( Ihandle *self ){
    return event_common(self,"BUTTON_RELEASE_CB");
}
int event_cancel_cb( Ihandle *self ){
    return event_common(self,"CANCEL_CB");
}
int event_caret_cb( Ihandle *self ){
    return event_common(self,"CARET_CB");
}
int event_cell_cb( Ihandle *self ){
    return event_common(self,"CELL_CB");
}
int event_change_cb( Ihandle *self ){
    return event_common(self,"CHANGE_CB");
}
int event_clicksample_cb( Ihandle *self ){
    return event_common(self,"CLICKSAMPLE_CB");
}
int event_clicksegment_cb( Ihandle *self ){
    return event_common(self,"CLICKSEGMENT_CB");
}
int event_closetext_cb( Ihandle *self ){
    return event_common(self,"CLOSETEXT_CB");
}
int event_close_cb( Ihandle *self ){
    return event_common(self,"CLOSE_CB");
}
int event_colorupdate_cb( Ihandle *self ){
    return event_common(self,"COLORUPDATE_CB");
}
int event_completed_cb( Ihandle *self ){
    return event_common(self,"COMPLETED_CB");
}
int event_configload_cb( Ihandle *self ){
    return event_common(self,"CONFIGLOAD_CB");
}
int event_configsave_cb( Ihandle *self ){
    return event_common(self,"CONFIGSAVE_CB");
}
int event_copydata_cb( Ihandle *self ){
    return event_common(self,"COPYDATA_CB");
}
int event_customframeactivate_cb( Ihandle *self ){
    return event_common(self,"CUSTOMFRAMEACTIVATE_CB");
}
int event_customframe_cb( Ihandle *self ){
    return event_common(self,"CUSTOMFRAME_CB");
}
int event_dblclick_cb( Ihandle *self ){
    return event_common(self,"DBLCLICK_CB");
}
int event_deletebegin_cb( Ihandle *self ){
    return event_common(self,"DELETEBEGIN_CB");
}
int event_deleteend_cb( Ihandle *self ){
    return event_common(self,"DELETEEND_CB");
}
int event_delete_cb( Ihandle *self ){
    return event_common(self,"DELETE_CB");
}
int event_destroy_cb( Ihandle *self ){
    return event_common(self,"DESTROY_CB");
}
int event_detached_cb( Ihandle *self ){
    return event_common(self,"DETACHED_CB");
}
int event_dragdrop_cb( Ihandle *self ){
    return event_common(self,"DRAGDROP_CB");
}
int event_drag_cb( Ihandle *self ){
    return event_common(self,"DRAG_CB");
}
int event_drawsample_cb( Ihandle *self ){
    return event_common(self,"DRAWSAMPLE_CB");
}
int event_draw_cb( Ihandle *self ){
    return event_common(self,"DRAW_CB");
}
int event_dropdown_cb( Ihandle *self ){
    return event_common(self,"DROPDOWN_CB");
}
int event_dropfiles_cb( Ihandle *self ){
    return event_common(self,"DROPFILES_CB");
}
int event_dropshow_cb( Ihandle *self ){
    return event_common(self,"DROPSHOW_CB");
}
int event_dspropertieschanged_cb( Ihandle *self ){
    return event_common(self,"DSPROPERTIESCHANGED_CB");
}
int event_dwell_cb( Ihandle *self ){
    return event_common(self,"DWELL_CB");
}
int event_editsample_cb( Ihandle *self ){
    return event_common(self,"EDITSAMPLE_CB");
}
int event_edit_cb( Ihandle *self ){
    return event_common(self,"EDIT_CB");
}
int event_enterwindow_cb( Ihandle *self ){
    return event_common(self,"ENTERWINDOW_CB");
}
int event_error_cb( Ihandle *self ){
    return event_common(self,"ERROR_CB");
}
int event_exit_cb( Ihandle *self ){
    return event_common(self,"EXIT_CB");
}
int event_extended_cb( Ihandle *self ){
    return event_common(self,"EXTENDED_CB");
}
int event_extrabutton_cb( Ihandle *self ){
    return event_common(self,"EXTRABUTTON_CB");
}
int event_file_cb( Ihandle *self ){
    return event_common(self,"FILE_CB");
}
int event_flat_action( Ihandle *self ){
    return event_common(self,"FLAT_ACTION");
}
int event_focus_cb( Ihandle *self ){
    return event_common(self,"FOCUS_CB");
}
int event_getfocus_cb( Ihandle *self ){
    return event_common(self,"GETFOCUS_CB");
}
int event_gl_action( Ihandle *self ){
    return event_common(self,"GL_ACTION");
}
int event_gl_button_cb( Ihandle *self ){
    return event_common(self,"GL_BUTTON_CB");
}
int event_gl_enterwindow_cb( Ihandle *self ){
    return event_common(self,"GL_ENTERWINDOW_CB");
}
int event_gl_leavewindow_cb( Ihandle *self ){
    return event_common(self,"GL_LEAVEWINDOW_CB");
}
int event_gl_motion_cb( Ihandle *self ){
    return event_common(self,"GL_MOTION_CB");
}
int event_gl_wheel_cb( Ihandle *self ){
    return event_common(self,"GL_WHEEL_CB");
}
int event_height_cb( Ihandle *self ){
    return event_common(self,"HEIGHT_CB");
}
int event_help_cb( Ihandle *self ){
    return event_common(self,"HELP_CB");
}
int event_highlight_cb( Ihandle *self ){
    return event_common(self,"HIGHLIGHT_CB");
}
int event_hotspotclick_cb( Ihandle *self ){
    return event_common(self,"HOTSPOTCLICK_CB");
}
int event_hspan_cb( Ihandle *self ){
    return event_common(self,"HSPAN_CB");
}
int event_imagevaluechanged_cb( Ihandle *self ){
    return event_common(self,"IMAGEVALUECHANGED_CB");
}
int event_keypress_cb( Ihandle *self ){
    return event_common(self,"KEYPRESS_CB");
}
int event_killfocus_cb( Ihandle *self ){
    return event_common(self,"KILLFOCUS_CB");
}
int event_k_any( Ihandle *self ){
    return event_common(self,"K_ANY");
}
int event_layoutupdate_cb( Ihandle *self ){
    return event_common(self,"LAYOUTUPDATE_CB");
}
int event_leavewindow_cb( Ihandle *self ){
    return event_common(self,"LEAVEWINDOW_CB");
}
int event_lineschanged_cb( Ihandle *self ){
    return event_common(self,"LINESCHANGED_CB");
}
int event_listaction_cb( Ihandle *self ){
    return event_common(self,"LISTACTION_CB");
}
int event_listclick_cb( Ihandle *self ){
    return event_common(self,"LISTCLICK_CB");
}
int event_listdraw_cb( Ihandle *self ){
    return event_common(self,"LISTDRAW_CB");
}
int event_listedition_cb( Ihandle *self ){
    return event_common(self,"LISTEDITION_CB");
}
int event_listinsert_cb( Ihandle *self ){
    return event_common(self,"LISTINSERT_CB");
}
int event_listrelease_cb( Ihandle *self ){
    return event_common(self,"LISTRELEASE_CB");
}
int event_listremove_cb( Ihandle *self ){
    return event_common(self,"LISTREMOVE_CB");
}
int event_map_cb( Ihandle *self ){
    return event_common(self,"MAP_CB");
}
int event_marginclick_cb( Ihandle *self ){
    return event_common(self,"MARGINCLICK_CB");
}
int event_markerchanged_cb( Ihandle *self ){
    return event_common(self,"MARKERCHANGED_CB");
}
int event_maskfail_cb( Ihandle *self ){
    return event_common(self,"MASKFAIL_CB");
}
int event_mdiactivate_cb( Ihandle *self ){
    return event_common(self,"MDIACTIVATE_CB");
}
int event_menuclose_cb( Ihandle *self ){
    return event_common(self,"MENUCLOSE_CB");
}
int event_menucontextclose_cb( Ihandle *self ){
    return event_common(self,"MENUCONTEXTCLOSE_CB");
}
int event_menucontext_cb( Ihandle *self ){
    return event_common(self,"MENUCONTEXT_CB");
}
int event_motion_cb( Ihandle *self ){
    return event_common(self,"MOTION_CB");
}
int event_mouseclick_cb( Ihandle *self ){
    return event_common(self,"MOUSECLICK_CB");
}
int event_mousemotion_cb( Ihandle *self ){
    return event_common(self,"MOUSEMOTION_CB");
}
int event_mousemove_cb( Ihandle *self ){
    return event_common(self,"MOUSEMOVE_CB");
}
int event_move_cb( Ihandle *self ){
    return event_common(self,"MOVE_CB");
}
int event_multiselect_cb( Ihandle *self ){
    return event_common(self,"MULTISELECT_CB");
}
int event_multitouch_cb( Ihandle *self ){
    return event_common(self,"MULTITOUCH_CB");
}
int event_navigate_cb( Ihandle *self ){
    return event_common(self,"NAVIGATE_CB");
}
int event_ncols_cb( Ihandle *self ){
    return event_common(self,"NCOLS_CB");
}
int event_newfilename_cb( Ihandle *self ){
    return event_common(self,"NEWFILENAME_CB");
}
int event_newtext_cb( Ihandle *self ){
    return event_common(self,"NEWTEXT_CB");
}
int event_newwindow_cb( Ihandle *self ){
    return event_common(self,"NEWWINDOW_CB");
}
int event_nlines_cb( Ihandle *self ){
    return event_common(self,"NLINES_CB");
}
int event_numericgetvalue_cb( Ihandle *self ){
    return event_common(self,"NUMERICGETVALUE_CB");
}
int event_numericsetvalue_cb( Ihandle *self ){
    return event_common(self,"NUMERICSETVALUE_CB");
}
int event_openclose_cb( Ihandle *self ){
    return event_common(self,"OPENCLOSE_CB");
}
int event_open_cb( Ihandle *self ){
    return event_common(self,"OPEN_CB");
}
int event_pastesize_cb( Ihandle *self ){
    return event_common(self,"PASTESIZE_CB");
}
int event_plotbutton_cb( Ihandle *self ){
    return event_common(self,"PLOTBUTTON_CB");
}
int event_plotmotion_cb( Ihandle *self ){
    return event_common(self,"PLOTMOTION_CB");
}
int event_postdraw_cb( Ihandle *self ){
    return event_common(self,"POSTDRAW_CB");
}
int event_predraw_cb( Ihandle *self ){
    return event_common(self,"PREDRAW_CB");
}
int event_propertieschanged_cb( Ihandle *self ){
    return event_common(self,"PROPERTIESCHANGED_CB");
}
int event_resize_cb( Ihandle *self ){
    return event_common(self,"RESIZE_CB");
}
int event_restored_cb( Ihandle *self ){
    return event_common(self,"RESTORED_CB");
}
int event_restoremarkers_cb( Ihandle *self ){
    return event_common(self,"RESTOREMARKERS_CB");
}
int event_rightclick_cb( Ihandle *self ){
    return event_common(self,"RIGHTCLICK_CB");
}
int event_savemarkers_cb( Ihandle *self ){
    return event_common(self,"SAVEMARKERS_CB");
}
int event_savepoint_cb( Ihandle *self ){
    return event_common(self,"SAVEPOINT_CB");
}
int event_scrolling_cb( Ihandle *self ){
    return event_common(self,"SCROLLING_CB");
}
int event_scroll_cb( Ihandle *self ){
    return event_common(self,"SCROLL_CB");
}
int event_selectbegin_cb( Ihandle *self ){
    return event_common(self,"SELECTBEGIN_CB");
}
int event_selectend_cb( Ihandle *self ){
    return event_common(self,"SELECTEND_CB");
}
int event_selection_cb( Ihandle *self ){
    return event_common(self,"SELECTION_CB");
}
int event_select_cb( Ihandle *self ){
    return event_common(self,"SELECT_CB");
}
int event_show_cb( Ihandle *self ){
    return event_common(self,"SHOW_CB");
}
int event_sortcolumncompare_cb( Ihandle *self ){
    return event_common(self,"SORTCOLUMNCOMPARE_CB");
}
int event_spin_cb( Ihandle *self ){
    return event_common(self,"SPIN_CB");
}
int event_swapbuffers_cb( Ihandle *self ){
    return event_common(self,"SWAPBUFFERS_CB");
}
int event_switch_cb( Ihandle *self ){
    return event_common(self,"SWITCH_CB");
}
int event_tabchangepos_cb( Ihandle *self ){
    return event_common(self,"TABCHANGEPOS_CB");
}
int event_tabchange_cb( Ihandle *self ){
    return event_common(self,"TABCHANGE_CB");
}
int event_tabclose_cb( Ihandle *self ){
    return event_common(self,"TABCLOSE_CB");
}
int event_touch_cb( Ihandle *self ){
    return event_common(self,"TOUCH_CB");
}
int event_trayclick_cb( Ihandle *self ){
    return event_common(self,"TRAYCLICK_CB");
}
int event_unmap_cb( Ihandle *self ){
    return event_common(self,"UNMAP_CB");
}
int event_updatecontent_cb( Ihandle *self ){
    return event_common(self,"UPDATECONTENT_CB");
}
int event_updatehscroll_cb( Ihandle *self ){
    return event_common(self,"UPDATEHSCROLL_CB");
}
int event_updateselection_cb( Ihandle *self ){
    return event_common(self,"UPDATESELECTION_CB");
}
int event_updatevscroll_cb( Ihandle *self ){
    return event_common(self,"UPDATEVSCROLL_CB");
}
int event_valuechanged_cb( Ihandle *self ){
    return event_common(self,"VALUECHANGED_CB");
}
int event_valuechanging_cb( Ihandle *self ){
    return event_common(self,"VALUECHANGING_CB");
}
int event_vspan_cb( Ihandle *self ){
    return event_common(self,"VSPAN_CB");
}
int event_wheel_cb( Ihandle *self ){
    return event_common(self,"WHEEL_CB");
}
int event_width_cb( Ihandle *self ){
    return event_common(self,"WIDTH_CB");
}
int event_wom_cb( Ihandle *self ){
    return event_common(self,"WOM_CB");
}
int event_zoom_cb( Ihandle *self ){
    return event_common(self,"ZOOM_CB");
}
