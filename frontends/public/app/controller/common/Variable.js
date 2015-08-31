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

Ext.define('Jhako.controller.common.Variable', {
  extend: 'Ext.app.Controller',
  refs: [{
    ref: 'processSidebar',
    selector: 'processSidebar'
  }, {
    ref: 'processJobunitTab',
    selector: 'processJobunitTab'
  }, {
    ref: 'historySidebar',
    selector: 'historySidebar'
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
      'commonVariable button[action=add_variable]': {
        click: this.onAddVariable
      },
      'commonVariable button[action=delete_variable]': {
        click: this.onDeleteVariable
      },
      'commonVariable button[action=clear_variable]': {
        click: this.onClearVariable
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
  onAddVariable: function(button, e) {
    return;
    var me = this;

    var tab = me.getProcessJobunitTab();
    var panel = tab.child('#comm_jobnet');
    var form = panel.child('#comm_jobunit');
    var record = form.getRecord();
    if (!record)
      return;

    var sidebar = me.getProcessSidebar();
    var grid = sidebar.child('#comm_variable');
    var store = grid.getStore();

    var variable = new Jhako.model.Variable({
      proc_jobunit_id: record.data.id
    });

    var cnt = store.getCount();
    store.insert(cnt, variable);
    var cellediting = grid.getPlugin('cellediting_variable');
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
  onDeleteVariable: function(button, e) {
    return;
    var me = this;
    var sidebar = me.getProcessSidebar();
    var grid = sidebar.child('#comm_variable');
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
  onClearVariable: function(button, e) {
    return;
    var me = this;
    var sidebar = me.getProcessSidebar();
    var grid = sidebar.child('#comm_variable');
    var store = grid.getStore();
    store.removeAll();
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
  onLoadProcRecord: function(record) {
    if (!record)
      return;

    var me = this;
    var sidebar = me.getProcessSidebar();
    var grid_variable = sidebar.child('#comm_variable');

    if (record['jhako.model.variablesStore']) {
      grid_variable.reconfigure(record['jhako.model.variablesStore']);
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
  onLoadHistRecord: function(record) {
    if (!record)
      return;

    var me = this;
    var sidebar = me.getHistorySidebar();
    var grid_variable = sidebar.child('#comm_variable');

    if (record['jhako.model.variablesStore']) {
      grid_variable.reconfigure(record['jhako.model.variablesStore']);
    }
  },
});