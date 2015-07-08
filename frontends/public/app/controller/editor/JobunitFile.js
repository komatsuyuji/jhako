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

Ext.define('Jhako.controller.editor.JobunitFile', {
  extend: 'Ext.app.Controller',

  refs: [{
    ref: 'editorTree',
    selector: 'editorTree'
  }, {
    ref: 'editorJobunitFile',
    selector: 'editorJobunitFile'
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
      'editorJobunitFile button[action=import_jobunit]': {
        click: this.onImportJobunit
      },
      'editorJobunitFile button[action=cancel_jobunit]': {
        click: this.onCancelJobunit
      }
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
  onImportJobunit: function(button, e) {
    var me = this;
    var win = me.getEditorJobunitFile();
    var tree = me.getEditorTree();
    var record = tree.getSelectionModel().getLastSelected();
    var form = win.child('#form_import').getForm();
    var ctrl = Jhako.app.getController('editor.Tree');

    var url = location.pathname + '/jobunits.json'
    form.submit({
      url: url,
      params: {
        import: 1,
        parent_id: record.data.id
      },
      waitMsg: 'Importing ...',
      failure: function(fp, action) {
        win.hide();
        ctrl.onLoadJobunit(record.data.id);
        Ext.Msg.alert(I18n.t('views.msg.error'), I18n.t('views.msg.import_failed'));
      },
      success: function(fp, action) {
        win.hide();
        ctrl.onLoadJobunit(record.data.id);
      }
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
  onCancelJobunit: function(button, e) {
    var me = this;
    var win = me.getEditorJobunitFile();
    win.hide();
  },

});
