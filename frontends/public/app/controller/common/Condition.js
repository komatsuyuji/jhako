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

Ext.define('Jhako.controller.common.Condition', {
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
      'commonCondition[itemId=sshjob_error] button[action=add_condition]': {
        click: this.onAddSshjobErrorCond
      },
      'commonCondition[itemId=sshjob_error] button[action=delete_condition]': {
        click: this.onDeleteSshjobErrorCond
      },
      'commonCondition[itemId=sshjob_error] button[action=clear_condition]': {
        click: this.onClearSshjobErrorCond
      },
      'commonCondition[itemId=sshjob_branch] button[action=add_condition]': {
        click: this.onAddSshjobBranchCond
      },
      'commonCondition[itemId=sshjob_branch] button[action=delete_condition]': {
        click: this.onDeleteSshjobBranchCond
      },
      'commonCondition[itemId=sshjob_branch] button[action=clear_condition]': {
        click: this.onClearSshjobBranchCond
      },
      'commonCondition[itemId=winjob_error] button[action=add_condition]': {
        click: this.onAddWinjobErrorCond
      },
      'commonCondition[itemId=winjob_error] button[action=delete_condition]': {
        click: this.onDeleteWinjobErrorCond
      },
      'commonCondition[itemId=winjob_error] button[action=clear_condition]': {
        click: this.onClearWinjobErrorCond
      },
      'commonCondition[itemId=winjob_branch] button[action=add_condition]': {
        click: this.onAddWinjobBranchCond
      },
      'commonCondition[itemId=winjob_branch] button[action=delete_condition]': {
        click: this.onDeleteWinjobBranchCond
      },
      'commonCondition[itemId=winjob_branch] button[action=clear_condition]': {
        click: this.onClearWinjobBranchCond
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
  onAddSshjobErrorCond: function(btn, e) {
    var me = this;
    var tab = me.getEditorJobunitTab();
    var panel = tab.child('#comm_sshjob');
    var grid = panel.child('#sshjob_error');
    var form = panel.child('#comm_jobunit');
    record = form.getRecord();
    if (!record)
      return;

    me.onAddCondition(grid, record.data.id, CONDITION_METHOD_ERROR);
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
  onDeleteSshjobErrorCond: function(btn, e) {
    var me = this;
    var tab = me.getEditorJobunitTab();
    var panel = tab.child('#comm_sshjob');
    var grid = panel.child('#sshjob_error');
    me.onDeleteCondition(grid);
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
  onClearSshjobErrorCond: function(btn, e) {
    var me = this;
    var tab = me.getEditorJobunitTab();
    var panel = tab.child('#comm_sshjob');
    var grid = panel.child('#sshjob_error');
    me.onClearCondition(grid);
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
  onAddSshjobBranchCond: function(btn, e) {
    var me = this;
    var tab = me.getEditorJobunitTab();
    var panel = tab.child('#comm_sshjob');
    var grid = panel.child('#sshjob_branch');
    var form = panel.child('#comm_jobunit');
    var record = form.getRecord();
    if (!record)
      return;

    me.onAddCondition(grid, record.data.id, CONDITION_METHOD_BRANCH);
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
  onDeleteSshjobBranchCond: function(btn, e) {
    var me = this;
    var tab = me.getEditorJobunitTab();
    var panel = tab.child('#comm_sshjob');
    var grid = panel.child('#sshjob_branch');
    me.onDeleteCondition(grid);
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
  onClearSshjobBranchCond: function(btn, e) {
    var me = this;
    var tab = me.getEditorJobunitTab();
    var panel = tab.child('#comm_sshjob');
    var grid = panel.child('#sshjob_branch');
    me.onClearCondition(grid);
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
  onAddWinjobErrorCond: function(btn, e) {
    var me = this;
    var tab = me.getEditorJobunitTab();
    var panel = tab.child('#comm_winjob');
    var grid = panel.child('#winjob_error');
    var form = panel.child('#comm_jobunit');
    var record = form.getRecord();
    if (!record)
      return;

    me.onAddCondition(grid, record.data.id, CONDITION_METHOD_ERROR);
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
  onDeleteWinjobErrorCond: function(btn, e) {
    var me = this;
    var tab = me.getEditorJobunitTab();
    var panel = tab.child('#comm_winjob');
    var grid = panel.child('#winjob_error');
    me.onClearCondition(grid);
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
  onClearWinjobErrorCond: function(btn, e) {
    var me = this;
    var tab = me.getEditorJobunitTab();
    var panel = tab.child('#comm_winjob');
    var grid = panel.child('#winjob_error');
    me.onClearCondition(grid);
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
  onAddWinjobBranchCond: function(btn, e) {
    var me = this;
    var tab = me.getEditorJobunitTab();
    var panel = tab.child('#comm_winjob');
    var grid = panel.child('#winjob_branch');
    var form = panel.child('#comm_jobunit');
    var record = form.getRecord();
    if (!record)
      return;

    me.onAddCondition(grid, record.data.id, CONDITION_METHOD_BRANCH);
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
  onDeleteWinjobBranchCond: function(btn, e) {
    var me = this;
    var tab = me.getEditorJobunitTab();
    var panel = tab.child('#comm_winjob');
    var grid = panel.child('#winjob_branch');
    me.onDeleteCondition(grid);
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
  onClearWinjobBranchCond: function(btn, e) {
    var me = this;
    var tab = me.getEditorJobunitTab();
    var panel = tab.child('#comm_winjob');
    var grid = panel.child('#winjob_branch');
    me.onClearCondition(grid);
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
  onAddCondition: function(grid, jobunit_id, method) {
    if (!grid)
      return;

    var store = grid.getStore();
    var condition = new Jhako.model.Condition({
      jobunit_id: jobunit_id,
      method: method
    });
    var cnt = store.getCount();
    store.insert(cnt, condition);

    var cellediting = grid.getPlugin('cellediting_condition');
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
  onDeleteCondition: function(grid) {
    if (!grid)
      return;

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
  onClearCondition: function(grid) {
    if (!grid)
      return;

    var store = grid.getStore();
    store.removeAll();
  },
});