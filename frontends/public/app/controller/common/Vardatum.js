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

Ext.define('Jhako.controller.common.Vardatum', {
  extend: 'Ext.app.Controller',
  refs: [{
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
      'commonVardatum button[action=add_vardatum]': {
        click: this.onAddVardatum
      },
      'commonVardatum button[action=delete_vardatum]': {
        click: this.onDeleteVardatum
      },
      'commonVardatum button[action=clear_vardatum]': {
        click: this.onClearVardatum
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
  onAddVardatum: function(button, e) {
    var me = this;
    var tab = me.getEditorJobunitTab();
    var panel = tab.child('#comm_varjob');
    var form = panel.child('#comm_jobunit');
    var record = form.getRecord();
    if (!record)
      return;

    var grid = panel.child('#comm_vardatum');
    var store = grid.getStore();

    var vardatum = new Jhako.model.Vardatum({
      jobunit_id: record.data.id
    });
    var cnt = store.getCount();
    store.insert(cnt, vardatum);
    var cellediting = grid.getPlugin('cellediting_vardatum');
    cellediting.startEdit(cnt, 0);
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
  onDeleteVardatum: function(button, e) {
    var me = this;
    var tab = me.getEditorJobunitTab();
    var panel = tab.child('#comm_varjob');
    var grid = panel.child('#comm_vardatum');
    var store = grid.getStore();
    var record = grid.getSelectionModel().getLastSelected();
    if (!record)
      return;
    store.remove(record);
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
  onClearVardatum: function(button, e) {
    var me = this;
    var tab = me.getEditorJobunitTab();
    var panel = tab.child('#comm_varjob');
    var grid = panel.child('#comm_vardatum');
    var store = grid.getStore();
    store.removeAll();
  },
});
