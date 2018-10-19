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

#ifndef PHP_IUP_H
#define PHP_IUP_H

extern zend_module_entry iup_module_entry;
#define phpext_iup_ptr &iup_module_entry

#define PHP_IUP_VERSION "0.1.0" /* Replace with version number for your extension */

#ifdef PHP_WIN32
#	define PHP_IUP_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_IUP_API __attribute__ ((visibility("default")))
#else
#	define PHP_IUP_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

/*
  	Declare any global variables you may need between the BEGIN
	and END macros here:

ZEND_BEGIN_MODULE_GLOBALS(iup)
	zend_long  global_value;
	char *global_string;
ZEND_END_MODULE_GLOBALS(iup)
*/

/* Always refer to the globals in your function as IUP_G(variable).
   You are encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/
#define IUP_G(v) ZEND_MODULE_GLOBALS_ACCESSOR(iup, v)

#if defined(ZTS) && defined(COMPILE_DL_IUP)
ZEND_TSRMLS_CACHE_EXTERN()
#endif

#include "ext/iup/iup-3.25/include/iup.h"
#include "ext/iup/iup-3.25/include/iupcbs.h"
#include "ext/iup/iup-3.25/include/iupcontrols.h"
#include "ext/iup/iup-3.25/include/iupdraw.h"
#include "ext/iup/iup-3.25/include/iupgl.h"
#include "ext/iup/iup-3.25/include/iupglcontrols.h"
#include "ext/iup/iup-3.25/include/iupim.h"
#include "ext/iup/iup-3.25/include/iupole.h"
#include "ext/iup/iup-3.25/include/iuptuio.h"
#include "ext/iup/iup-3.25/include/iupweb.h"
#include "ext/iup/iup-3.25/include/iup_config.h"
#include "ext/iup/iup-3.25/include/iup_mglplot.h"
#include "ext/iup/iup-3.25/include/iup_plot.h"
#include "ext/iup/iup-3.25/include/iup_scintilla.h"
#include "ext/iup/iup-3.25/include/iup_varg.h"


int event_action( Ihandle *self );
int event_action_cb( Ihandle *self );
int event_autoccancelled_cb( Ihandle *self );
int event_autocchardeleted_cb( Ihandle *self );
int event_autocselection_cb( Ihandle *self );
int event_busy_cb( Ihandle *self );
int event_button_cb( Ihandle *self );
int event_button_press_cb( Ihandle *self );
int event_button_release_cb( Ihandle *self );
int event_cancel_cb( Ihandle *self );
int event_caret_cb( Ihandle *self );
int event_cell_cb( Ihandle *self );
int event_change_cb( Ihandle *self );
int event_clicksample_cb( Ihandle *self );
int event_clicksegment_cb( Ihandle *self );
int event_closetext_cb( Ihandle *self );
int event_close_cb( Ihandle *self );
int event_colorupdate_cb( Ihandle *self );
int event_completed_cb( Ihandle *self );
int event_configload_cb( Ihandle *self );
int event_configsave_cb( Ihandle *self );
int event_copydata_cb( Ihandle *self );
int event_customframeactivate_cb( Ihandle *self );
int event_customframe_cb( Ihandle *self );
int event_dblclick_cb( Ihandle *self );
int event_deletebegin_cb( Ihandle *self );
int event_deleteend_cb( Ihandle *self );
int event_delete_cb( Ihandle *self );
int event_destroy_cb( Ihandle *self );
int event_detached_cb( Ihandle *self );
int event_dragdrop_cb( Ihandle *self );
int event_drag_cb( Ihandle *self );
int event_drawsample_cb( Ihandle *self );
int event_draw_cb( Ihandle *self );
int event_dropdown_cb( Ihandle *self );
int event_dropfiles_cb( Ihandle *self );
int event_dropshow_cb( Ihandle *self );
int event_dspropertieschanged_cb( Ihandle *self );
int event_dwell_cb( Ihandle *self );
int event_editsample_cb( Ihandle *self );
int event_edit_cb( Ihandle *self );
int event_enterwindow_cb( Ihandle *self );
int event_error_cb( Ihandle *self );
int event_exit_cb( Ihandle *self );
int event_extended_cb( Ihandle *self );
int event_extrabutton_cb( Ihandle *self );
int event_file_cb( Ihandle *self );
int event_flat_action( Ihandle *self );
int event_focus_cb( Ihandle *self );
int event_getfocus_cb( Ihandle *self );
int event_gl_action( Ihandle *self );
int event_gl_button_cb( Ihandle *self );
int event_gl_enterwindow_cb( Ihandle *self );
int event_gl_leavewindow_cb( Ihandle *self );
int event_gl_motion_cb( Ihandle *self );
int event_gl_wheel_cb( Ihandle *self );
int event_height_cb( Ihandle *self );
int event_help_cb( Ihandle *self );
int event_highlight_cb( Ihandle *self );
int event_hotspotclick_cb( Ihandle *self );
int event_hspan_cb( Ihandle *self );
int event_imagevaluechanged_cb( Ihandle *self );
int event_keypress_cb( Ihandle *self );
int event_killfocus_cb( Ihandle *self );
int event_k_any( Ihandle *self );
int event_layoutupdate_cb( Ihandle *self );
int event_leavewindow_cb( Ihandle *self );
int event_lineschanged_cb( Ihandle *self );
int event_listaction_cb( Ihandle *self );
int event_listclick_cb( Ihandle *self );
int event_listdraw_cb( Ihandle *self );
int event_listedition_cb( Ihandle *self );
int event_listinsert_cb( Ihandle *self );
int event_listrelease_cb( Ihandle *self );
int event_listremove_cb( Ihandle *self );
int event_map_cb( Ihandle *self );
int event_marginclick_cb( Ihandle *self );
int event_markerchanged_cb( Ihandle *self );
int event_maskfail_cb( Ihandle *self );
int event_mdiactivate_cb( Ihandle *self );
int event_menuclose_cb( Ihandle *self );
int event_menucontextclose_cb( Ihandle *self );
int event_menucontext_cb( Ihandle *self );
int event_motion_cb( Ihandle *self );
int event_mouseclick_cb( Ihandle *self );
int event_mousemotion_cb( Ihandle *self );
int event_mousemove_cb( Ihandle *self );
int event_move_cb( Ihandle *self );
int event_multiselect_cb( Ihandle *self );
int event_multitouch_cb( Ihandle *self );
int event_navigate_cb( Ihandle *self );
int event_ncols_cb( Ihandle *self );
int event_newfilename_cb( Ihandle *self );
int event_newtext_cb( Ihandle *self );
int event_newwindow_cb( Ihandle *self );
int event_nlines_cb( Ihandle *self );
int event_numericgetvalue_cb( Ihandle *self );
int event_numericsetvalue_cb( Ihandle *self );
int event_openclose_cb( Ihandle *self );
int event_open_cb( Ihandle *self );
int event_pastesize_cb( Ihandle *self );
int event_plotbutton_cb( Ihandle *self );
int event_plotmotion_cb( Ihandle *self );
int event_postdraw_cb( Ihandle *self );
int event_predraw_cb( Ihandle *self );
int event_propertieschanged_cb( Ihandle *self );
int event_resize_cb( Ihandle *self );
int event_restored_cb( Ihandle *self );
int event_restoremarkers_cb( Ihandle *self );
int event_rightclick_cb( Ihandle *self );
int event_savemarkers_cb( Ihandle *self );
int event_savepoint_cb( Ihandle *self );
int event_scrolling_cb( Ihandle *self );
int event_scroll_cb( Ihandle *self );
int event_selectbegin_cb( Ihandle *self );
int event_selectend_cb( Ihandle *self );
int event_selection_cb( Ihandle *self );
int event_select_cb( Ihandle *self );
int event_show_cb( Ihandle *self );
int event_sortcolumncompare_cb( Ihandle *self );
int event_spin_cb( Ihandle *self );
int event_swapbuffers_cb( Ihandle *self );
int event_switch_cb( Ihandle *self );
int event_tabchangepos_cb( Ihandle *self );
int event_tabchange_cb( Ihandle *self );
int event_tabclose_cb( Ihandle *self );
int event_touch_cb( Ihandle *self );
int event_trayclick_cb( Ihandle *self );
int event_unmap_cb( Ihandle *self );
int event_updatecontent_cb( Ihandle *self );
int event_updatehscroll_cb( Ihandle *self );
int event_updateselection_cb( Ihandle *self );
int event_updatevscroll_cb( Ihandle *self );
int event_valuechanged_cb( Ihandle *self );
int event_valuechanging_cb( Ihandle *self );
int event_vspan_cb( Ihandle *self );
int event_wheel_cb( Ihandle *self );
int event_width_cb( Ihandle *self );
int event_wom_cb( Ihandle *self );
int event_zoom_cb( Ihandle *self );

PHP_FUNCTION(IupOpen);
PHP_FUNCTION(IupClose);
PHP_FUNCTION(IupMainLoop);
PHP_FUNCTION(IupLoopStep);
PHP_FUNCTION(IupLoopStepWait);
PHP_FUNCTION(IupMainLoopLevel);
PHP_FUNCTION(IupImageLibOpen);
PHP_FUNCTION(IupFlush);
PHP_FUNCTION(IupExitLoop);
PHP_FUNCTION(IupRecordInput);
PHP_FUNCTION(IupPlayInput);
PHP_FUNCTION(IupUpdate);
PHP_FUNCTION(IupUpdateChildren);
PHP_FUNCTION(IupRedraw);
PHP_FUNCTION(IupRefresh);
PHP_FUNCTION(IupRefreshChildren);
PHP_FUNCTION(IupExecute);
PHP_FUNCTION(IupExecuteWait);
PHP_FUNCTION(IupHelp);
PHP_FUNCTION(IupLog);
PHP_FUNCTION(IupLoad);
PHP_FUNCTION(IupLoadBuffer);
PHP_FUNCTION(IupVersion);
PHP_FUNCTION(IupVersionDate);
PHP_FUNCTION(IupVersionNumber);
PHP_FUNCTION(IupSetLanguage);
PHP_FUNCTION(IupGetLanguage);
PHP_FUNCTION(IupSetLanguageString);
PHP_FUNCTION(IupStoreLanguageString);
PHP_FUNCTION(IupGetLanguageString);
PHP_FUNCTION(IupSetLanguagePack);
PHP_FUNCTION(IupDestroy);
PHP_FUNCTION(IupDetach);
PHP_FUNCTION(IupAppend);
PHP_FUNCTION(IupInsert);
PHP_FUNCTION(IupGetChild);
PHP_FUNCTION(IupGetChildPos);
PHP_FUNCTION(IupGetChildCount);
PHP_FUNCTION(IupGetNextChild);
PHP_FUNCTION(IupGetBrother);
PHP_FUNCTION(IupGetParent);
PHP_FUNCTION(IupGetDialog);
PHP_FUNCTION(IupGetDialogChild);
PHP_FUNCTION(IupReparent);
PHP_FUNCTION(IupPopup);
PHP_FUNCTION(IupShow);
PHP_FUNCTION(IupShowXY);
PHP_FUNCTION(IupHide);
PHP_FUNCTION(IupMap);
PHP_FUNCTION(IupUnmap);
PHP_FUNCTION(IupResetAttribute);
PHP_FUNCTION(IupGetAllAttributes);
PHP_FUNCTION(IupSetAtt);
PHP_FUNCTION(IupSetAttributes);
PHP_FUNCTION(IupGetAttributes);
PHP_FUNCTION(IupSetAttribute);
PHP_FUNCTION(IupSetStrAttribute);
PHP_FUNCTION(IupSetStrf);
PHP_FUNCTION(IupSetInt);
PHP_FUNCTION(IupSetFloat);
PHP_FUNCTION(IupSetDouble);
PHP_FUNCTION(IupSetRGB);
PHP_FUNCTION(IupGetAttribute);
PHP_FUNCTION(IupGetInt);
PHP_FUNCTION(IupGetInt2);
PHP_FUNCTION(IupGetIntInt);
PHP_FUNCTION(IupGetFloat);
PHP_FUNCTION(IupGetDouble);
PHP_FUNCTION(IupGetRGB);
PHP_FUNCTION(IupSetAttributeId);
PHP_FUNCTION(IupSetStrAttributeId);
PHP_FUNCTION(IupSetStrfId);
PHP_FUNCTION(IupSetIntId);
PHP_FUNCTION(IupSetFloatId);
PHP_FUNCTION(IupSetDoubleId);
PHP_FUNCTION(IupSetRGBId);
PHP_FUNCTION(IupGetAttributeId);
PHP_FUNCTION(IupGetIntId);
PHP_FUNCTION(IupGetFloatId);
PHP_FUNCTION(IupGetDoubleId);
PHP_FUNCTION(IupGetRGBId);
PHP_FUNCTION(IupSetAttributeId2);
PHP_FUNCTION(IupSetStrAttributeId2);
PHP_FUNCTION(IupSetStrfId2);
PHP_FUNCTION(IupSetIntId2);
PHP_FUNCTION(IupSetFloatId2);
PHP_FUNCTION(IupSetDoubleId2);
PHP_FUNCTION(IupSetRGBId2);
PHP_FUNCTION(IupGetAttributeId2);
PHP_FUNCTION(IupGetIntId2);
PHP_FUNCTION(IupGetFloatId2);
PHP_FUNCTION(IupGetDoubleId2);
PHP_FUNCTION(IupGetRGBId2);
PHP_FUNCTION(IupSetGlobal);
PHP_FUNCTION(IupSetStrGlobal);
PHP_FUNCTION(IupGetGlobal);
PHP_FUNCTION(IupSetFocus);
PHP_FUNCTION(IupGetFocus);
PHP_FUNCTION(IupPreviousField);
PHP_FUNCTION(IupNextField);
PHP_FUNCTION(IupSetCallback);
PHP_FUNCTION(IupGetCallback);
PHP_FUNCTION(IupSetCallbacks);
PHP_FUNCTION(IupGetFunction);
PHP_FUNCTION(IupSetFunction);
PHP_FUNCTION(IupGetHandle);
PHP_FUNCTION(IupSetHandle);
PHP_FUNCTION(IupGetAllNames);
PHP_FUNCTION(IupGetAllDialogs);
PHP_FUNCTION(IupGetName);
PHP_FUNCTION(IupSetAttributeHandle);
PHP_FUNCTION(IupGetAttributeHandle);
PHP_FUNCTION(IupSetAttributeHandleId);
PHP_FUNCTION(IupGetAttributeHandleId);
PHP_FUNCTION(IupSetAttributeHandleId2);
PHP_FUNCTION(IupGetAttributeHandleId2);
PHP_FUNCTION(IupGetClassName);
PHP_FUNCTION(IupGetClassType);
PHP_FUNCTION(IupGetAllClasses);
PHP_FUNCTION(IupGetClassAttributes);
PHP_FUNCTION(IupGetClassCallbacks);
PHP_FUNCTION(IupSaveClassAttributes);
PHP_FUNCTION(IupCopyClassAttributes);
PHP_FUNCTION(IupSetClassDefaultAttribute);
PHP_FUNCTION(IupClassMatch);
PHP_FUNCTION(IupCreatek);
PHP_FUNCTION(IupCreatev);
PHP_FUNCTION(IupCreatep);
PHP_FUNCTION(IupFill);
PHP_FUNCTION(IupSpace);
PHP_FUNCTION(IupRadio);
PHP_FUNCTION(IupVbox);
PHP_FUNCTION(IupVboxv);
PHP_FUNCTION(IupZbox);
PHP_FUNCTION(IupZboxv);
PHP_FUNCTION(IupHbox);
PHP_FUNCTION(IupHboxv);
PHP_FUNCTION(IupNormalizer);
PHP_FUNCTION(IupNormalizerv);
PHP_FUNCTION(IupCbox);
PHP_FUNCTION(IupCboxv);
PHP_FUNCTION(IupSbox);
PHP_FUNCTION(IupSplit);
PHP_FUNCTION(IupScrollBox);
PHP_FUNCTION(IupFlatScrollBox);
PHP_FUNCTION(IupGridBox);
PHP_FUNCTION(IupGridBoxv);
PHP_FUNCTION(IupExpander);
PHP_FUNCTION(IupDetachBox);
PHP_FUNCTION(IupBackgroundBox);
PHP_FUNCTION(IupFrame);
PHP_FUNCTION(IupFlatFrame);
PHP_FUNCTION(IupImage);
PHP_FUNCTION(IupImageRGB);
PHP_FUNCTION(IupImageRGBA);
PHP_FUNCTION(IupItem);
PHP_FUNCTION(IupSubmenu);
PHP_FUNCTION(IupSeparator);
PHP_FUNCTION(IupMenu);
PHP_FUNCTION(IupMenuv);
PHP_FUNCTION(IupButton);
PHP_FUNCTION(IupFlatButton);
PHP_FUNCTION(IupFlatToggle);
PHP_FUNCTION(IupDropButton);
PHP_FUNCTION(IupFlatLabel);
PHP_FUNCTION(IupFlatSeparator);
PHP_FUNCTION(IupCanvas);
PHP_FUNCTION(IupDialog);
PHP_FUNCTION(IupUser);
PHP_FUNCTION(IupLabel);
PHP_FUNCTION(IupList);
PHP_FUNCTION(IupText);
PHP_FUNCTION(IupMultiLine);
PHP_FUNCTION(IupToggle);
PHP_FUNCTION(IupTimer);
PHP_FUNCTION(IupClipboard);
PHP_FUNCTION(IupProgressBar);
PHP_FUNCTION(IupVal);
PHP_FUNCTION(IupTabs);
PHP_FUNCTION(IupTabsv);
PHP_FUNCTION(IupFlatTabs);
PHP_FUNCTION(IupFlatTabsv);
PHP_FUNCTION(IupTree);
PHP_FUNCTION(IupLink);
PHP_FUNCTION(IupAnimatedLabel);
PHP_FUNCTION(IupDatePick);
PHP_FUNCTION(IupCalendar);
PHP_FUNCTION(IupColorbar);
PHP_FUNCTION(IupGauge);
PHP_FUNCTION(IupDial);
PHP_FUNCTION(IupColorBrowser);
PHP_FUNCTION(IupSpin);
PHP_FUNCTION(IupSpinbox);
PHP_FUNCTION(IupStringCompare);
PHP_FUNCTION(IupSaveImageAsText);
PHP_FUNCTION(IupTextConvertLinColToPos);
PHP_FUNCTION(IupTextConvertPosToLinCol);
PHP_FUNCTION(IupConvertXYToPos);
PHP_FUNCTION(IupStoreGlobal);
PHP_FUNCTION(IupStoreAttribute);
PHP_FUNCTION(IupSetfAttribute);
PHP_FUNCTION(IupStoreAttributeId);
PHP_FUNCTION(IupSetfAttributeId);
PHP_FUNCTION(IupStoreAttributeId2);
PHP_FUNCTION(IupSetfAttributeId2);
PHP_FUNCTION(IupTreeSetUserId);
PHP_FUNCTION(IupTreeGetUserId);
PHP_FUNCTION(IupTreeGetId);
PHP_FUNCTION(IupTreeSetAttributeHandle);
PHP_FUNCTION(IupFileDlg);
PHP_FUNCTION(IupMessageDlg);
PHP_FUNCTION(IupColorDlg);
PHP_FUNCTION(IupFontDlg);
PHP_FUNCTION(IupProgressDlg);
PHP_FUNCTION(IupGetFile);
PHP_FUNCTION(IupMessage);
PHP_FUNCTION(IupMessagef);
PHP_FUNCTION(IupMessageError);
PHP_FUNCTION(IupMessageAlarm);
PHP_FUNCTION(IupAlarm);
PHP_FUNCTION(IupScanf);
PHP_FUNCTION(IupListDialog);
PHP_FUNCTION(IupGetText);
PHP_FUNCTION(IupGetColor);
PHP_FUNCTION(IupGetParam);
PHP_FUNCTION(IupGetParamv);
PHP_FUNCTION(IupParam);
PHP_FUNCTION(IupParamBox);
PHP_FUNCTION(IupParamBoxv);
PHP_FUNCTION(IupLayoutDialog);
PHP_FUNCTION(IupElementPropertiesDialog);


#endif	/* PHP_IUP_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
