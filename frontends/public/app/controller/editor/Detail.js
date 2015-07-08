/////////////////////////////////////////////////////////////////////////////////
//
// jHako WebGUI
// Copyright (C) 2014-2015 Komatsu Yuji(Zheng Chuyu)
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
/////////////////////////////////////////////////////////////////////////////////

Ext.define('Jhako.controller.editor.Detail', {
  extend: 'Ext.app.Controller',
  refs: [{
    ref: 'editorDetail',
    selector: 'editorDetail'
  }, {
    ref: 'editorDrawTab',
    selector: 'editorDrawTab'
  }, {
    ref: 'editorJobunitTab',
    selector: 'editorJobunitTab'
  }],

  /////////////////////////////////////////////////////////////////////////////////
  //
  // Function:
  //
  // Purpose:
  //
  // Parameters:
  //
  // Return value:
  //
  // Author: Komatsu Yuji(Zheng Chuyu)
  //
  /////////////////////////////////////////////////////////////////////////////////
  init: function() {
    this.control({
      'editorDetail checkbox[itemId=chkbox_editing]': {
        change: this.onChange
      },
    });
  },

  /////////////////////////////////////////////////////////////////////////////////
  //
  // Function:
  //
  // Purpose:
  //
  // Parameters:
  //
  // Return value:
  //
  // Author: Komatsu Yuji(Zheng Chuyu)
  //
  /////////////////////////////////////////////////////////////////////////////////
  onChange: function(chkbox, newValue, oldValue, eOpts) {
    var me = this;

    if (!jhako_selected_rootjobnet || !jhako_login_user) {
      chkbox.setRawValue(oldValue);
      return;
    }

    if (newValue && jhako_selected_rootjobnet.data.user_id > 0)
      return;

    if (!newValue && jhako_selected_rootjobnet.data.user_id == 0)
      return;

    var readonly = true;
    var user_id = jhako_selected_rootjobnet.data.user_id;
    if (newValue) {
      jhako_selected_rootjobnet.data.user_id = jhako_login_user.data.id;
      readonly = false;
    } else {
      jhako_selected_rootjobnet.data.user_id = 0;
    }

    jhako_selected_rootjobnet.save({
      failure: function(rec) {
        jhako_selected_rootjobnet.data.user_id = user_id;
        chkbox.setRawValue(oldValue);
      },
      success: function(rec, operation) {
        me.onSetReadonly(readonly);
        if (readonly) {
          var ctrl = Jhako.app.getController('editor.Tree');
          ctrl.onLoadJobunit(jhako_selected_parent.data.id);
        }
      },
    });
  },

  /////////////////////////////////////////////////////////////////////////////////
  //
  // Function:
  //
  // Purpose:
  //
  // Parameters:
  //
  // Return value:
  //
  // Author: Komatsu Yuji(Zheng Chuyu)
  //
  /////////////////////////////////////////////////////////////////////////////////
  onSetCheckbox: function(record) {
    var me = this;
    var detail = me.getEditorDetail();
    var chk_editing = detail.query('#chkbox_editing')[0];

    if (!jhako_selected_rootjobnet || !jhako_login_user)
      return;

    if (jhako_selected_rootjobnet.data.user_id == 0) {
      chk_editing.setDisabled(false);
      chk_editing.setValue(false);
      me.onSetReadonly(true);
      return;
    }

    if (jhako_selected_rootjobnet.data.user_id == jhako_login_user.data.id) {
      chk_editing.setDisabled(false);
      chk_editing.setValue(true);
      me.onSetReadonly(false);
    } else {
      chk_editing.setDisabled(true);
      chk_editing.setValue(true);
      me.onSetReadonly(true);
    }
  },

  /////////////////////////////////////////////////////////////////////////////////
  //
  // Function:
  //
  // Purpose:
  //
  // Parameters:
  //
  // Return value:
  //
  // Author: Komatsu Yuji(Zheng Chuyu)
  //
  /////////////////////////////////////////////////////////////////////////////////
  onSetReadonly: function(readonly) {
    var me = this;
    var drawtab = me.getEditorDrawTab();
    var jobunittab = me.getEditorJobunitTab();
    drawtab.setDisabled(readonly);
    var btns = jobunittab.query('button');
    btns.forEach(function(btn) {
      if (btn.getItemId() == 'btn_copy_jobunit') {
        btn.setDisabled(false);
        return;
      }
      if (btn.getItemId() == 'btn_manualrun') {
        if (readonly) {
          btn.setDisabled(false);
        } else {
          btn.setDisabled(true);
        }
      } else {
        btn.setDisabled(readonly);
      }
    });
    var fields = jobunittab.query('field');
    fields.forEach(function(field) {
      if (field.getItemId() == 'df_manualrun' || field.getItemId() == 'tf_manualrun') {
        if (readonly) {
          field.setDisabled(false);
        } else {
          field.setDisabled(true);
        }
      } else {
        field.setReadOnly(readonly);
      }
    });
  },
});
