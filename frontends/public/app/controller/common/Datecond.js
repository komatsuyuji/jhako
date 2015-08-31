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

Ext.define('Jhako.controller.common.Datecond', {
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
      'commonDatecond button[action=add_datecond]': {
        click: this.onAddDatecond
      },
      'commonDatecond button[action=delete_datecond]': {
        click: this.onDeleteDatecond
      },
      'commonDatecond button[action=clear_datecond]': {
        click: this.onClearDatecond
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
  onAddDatecond: function(button, e) {
    var me = this;
    var tab = me.getEditorJobunitTab();
    var panel = tab.child('#comm_datejob');
    var form = panel.child('#comm_jobunit');

    var record = form.getRecord();
    if (!record)
      return;

    var grid = panel.child('#comm_datecond');
    var store = grid.getStore();

    var datecond = new Jhako.model.Datecond({
      jobunit_id: record.data.id
    });
    var cnt = store.getCount();
    store.insert(cnt, datecond);
    var cellediting = grid.getPlugin('cellediting_datecond');
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
  onDeleteDatecond: function(button, e) {
    var me = this;
    var tab = me.getEditorJobunitTab();
    var panel = tab.child('#comm_datejob');
    var grid = panel.child('#comm_datecond');
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
  onClearDatecond: function(button, e) {
    var me = this;
    var tab = me.getEditorJobunitTab();
    var panel = tab.child('#comm_datejob');
    var grid = panel.child('#comm_datecond');
    var store = grid.getStore();
    store.removeAll();
  },
});