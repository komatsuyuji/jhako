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

Ext.define('Jhako.controller.editor.WinAlarm', {
  extend: 'Ext.app.Controller',
  refs: [{
    ref: 'editorWinAlarm',
    selector: 'editorWinAlarm'
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
      'editorWinAlarm #tree_alarm': {
        itemclick: this.onItemclick,
      },
      'editorWinAlarm button[action=select_alarm]': {
        click: this.onSelectAlarm,
      },
      'editorWinAlarm button[action=cancel_alarm]': {
        click: this.onCancelAlarm,
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
  onItemclick: function(view, record, item, index, e) {
    var me = this;

    if (record.data.kind < JOBUNIT_KIND_ROOTJOBNET)
      return;

    var win = me.getEditorWinAlarm();
    var tree = win.child('#tree_alarm');
    var form = win.child('#form_alarm');
    var basicform = form.getForm();

    var store = tree.getStore();
    var node = store.getNodeById(record.data.id);

    var field_jobnet_id = basicform.findField('jobnet_id');
    var field_path = basicform.findField('path');

    field_jobnet_id.setValue(record.data.id);
    field_path.setValue(node.getPath('name').slice(2));
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
  onSelectAlarm: function(button, e) {
    var me = this;
    var win = me.getEditorWinAlarm();
    var form = win.child('#form_alarm');
    form.updateRecord();
    win.hide();
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
  onCancelAlarm: function(button, e) {
    var me = this;
    var win = me.getEditorWinAlarm();
    win.hide();
  },

});