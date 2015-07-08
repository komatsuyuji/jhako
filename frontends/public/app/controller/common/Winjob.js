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

Ext.define('Jhako.controller.common.Winjob', {
  extend: 'Ext.app.Controller',
  refs: [{
    ref: 'editorJobunitTab',
    selector: 'editorJobunitTab'
  }, {
    ref: 'processJobunitTab',
    selector: 'processJobunitTab'
  }, {
    ref: 'historyJobunitTab',
    selector: 'historyJobunitTab'
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
  init: function() {},

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
  onSaveRecord: function() {
    var me = this;
    var tab = me.getEditorJobunitTab();
    var panel = tab.child('#comm_winjob');
    var form_jobunit = panel.child('#comm_jobunit');
    var form_winjob = panel.child('#winjob_detail');
    var grid_error = panel.child('#winjob_error');
    var grid_branch = panel.child('#winjob_branch');
    var rec_jobunit = form_jobunit.getRecord();
    var rec_winjob = form_winjob.getRecord();

    // Update
    form_jobunit.updateRecord();
    form_winjob.updateRecord();

    // Save
    grid_error.getStore().sync();
    grid_branch.getStore().sync();
    rec_winjob.save();

    return rec_jobunit;
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
  onCancelRecord: function() {
    var me = this;
    var tab = me.getEditorJobunitTab();
    var panel = tab.child('#comm_winjob');
    var form_jobunit = panel.child('#comm_jobunit');

    var record = form_jobunit.getRecord();
    me.onLoadRecord(record);
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
  onLoadRecord: function(record) {
    if (!record)
      return;

    var me = this;
    var tab = me.getEditorJobunitTab();
    var panel = tab.child('#comm_winjob');
    var form_jobunit = panel.child('#comm_jobunit');
    var form_winjob = panel.child('#winjob_detail');
    var grid_error = panel.child('#winjob_error');
    var grid_branch = panel.child('#winjob_branch');

    JhakoSetForm(form_jobunit, record);
    Jhako.model.Jobunit.load(record.data.id, {
      success: function(rec) {
        var rec_winjob = rec['Jhako.model.WinjobHasOneInstance'];
        if (rec_winjob) {
          rec_winjob.getProxy().url = location.pathname + '/jobunits/' + rec.data.id + '/winjobs';
          JhakoSetForm(form_winjob, rec_winjob);
        }
        var store_error = JhakoGetCondition(rec, CONDITION_METHOD_ERROR, 'jobunits');
        var store_branch = JhakoGetCondition(rec, CONDITION_METHOD_BRANCH, 'jobunits');
        grid_error.reconfigure(store_error);
        grid_branch.reconfigure(store_branch);
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
  onLoadProcRecord: function(record) {
    if (!record)
      return;

    var me = this;
    var tab = me.getProcessJobunitTab();
    var panel = tab.child('#comm_winjob');
    var form_jobunit = panel.child('#comm_jobunit');
    var form_winjob = panel.child('#winjob_detail');
    var grid_error = panel.child('#winjob_error');
    var grid_branch = panel.child('#winjob_branch');

    JhakoSetForm(form_jobunit, record);
    JhakoSetForm(form_winjob, record['Jhako.model.WinjobHasOneInstance']);

    var store_error = JhakoGetCondition(record, CONDITION_METHOD_ERROR, 'proc_jobunits');
    var store_branch = JhakoGetCondition(record, CONDITION_METHOD_BRANCH, 'proc_jobunits');
    grid_error.reconfigure(store_error);
    grid_branch.reconfigure(store_branch);
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
    var tab = me.getHistoryJobunitTab();
    var panel = tab.child('#comm_winjob');
    var form_jobunit = panel.child('#comm_jobunit');
    var form_winjob = panel.child('#winjob_detail');
    var grid_error = panel.child('#winjob_error');
    var grid_branch = panel.child('#winjob_branch');

    JhakoSetForm(form_jobunit, record);
    JhakoSetForm(form_winjob, record['Jhako.model.WinjobHasOneInstance']);

    var store_error = JhakoGetCondition(record, CONDITION_METHOD_ERROR, 'hist_jobunits');
    var store_branch = JhakoGetCondition(record, CONDITION_METHOD_BRANCH, 'hist_jobunits');
    grid_error.reconfigure(store_error);
    grid_branch.reconfigure(store_branch);
  },

});
