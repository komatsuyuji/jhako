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

Ext.define('Jhako.controller.common.Alarm', {
  extend: 'Ext.app.Controller',
  refs: [{
    ref: 'editorJobunitTab',
    selector: 'editorJobunitTab'
  }, {
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
      'commonAlarm button[action=add_alarm]': {
        click: this.onAddAlarm
      },
      'commonAlarm button[action=delete_alarm]': {
        click: this.onDeleteAlarm
      },
      'commonAlarm button[action=clear_alarm]': {
        click: this.onClearAlarm
      },
      'commonAlarm #trig_path': {
        triggerclick: this.onTriggerClick
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
  onAddAlarm: function(button, e) {
    var me = this;
    var tab = me.getEditorJobunitTab();
    var panel = tab.child('#comm_jobnet');
    var form = panel.child('#comm_jobunit');
    var record = form.getRecord();
    if (!record)
      return;

    var grid = panel.child('#comm_alarm');
    var store = grid.getStore();

    var alarm = new Jhako.model.Alarm({
      jobunit_id: record.data.id
    });
    var cnt = store.getCount();
    store.insert(cnt, alarm);
    var cellediting = grid.getPlugin('cellediting_alarm');
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
  onDeleteAlarm: function(button, e) {
    var me = this;
    var tab = me.getEditorJobunitTab();
    var panel = tab.child('#comm_jobnet');
    var grid = panel.child('#comm_alarm');
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
  onClearAlarm: function(button, e) {
    var me = this;
    var tab = me.getEditorJobunitTab();
    var panel = tab.child('#comm_jobnet');
    var grid = panel.child('#comm_alarm');
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
  onTriggerClick: function() {
    var me = this;
    var tab = me.getEditorJobunitTab();
    var panel = tab.child('#comm_jobnet');
    var grid = panel.child('#comm_alarm');
    var win = me.getEditorWinAlarm();
    var form = win.child('#form_alarm');

    var record = grid.getSelectionModel().getLastSelected();
    form.loadRecord(record);
    win.show();
  },

});