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

Ext.define('Jhako.controller.common.Sshjob', {
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
  init: function() {
    this.control({
      'commonSshjob combobox[itemId=sshjob_authtype]': {
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
  onChange: function(combobox, newValue, oldValue) {
    var me = this;
    var tab = me.getEditorJobunitTab();
    var panel = tab.child('#comm_sshjob');
    var form_sshjob = panel.child('#sshjob_detail');
    var field_password = form_sshjob.child('#sshjob_password');
    var field_privatekey = form_sshjob.child('#sshjob_privatekey');

    if (newValue == 0) {
      field_password.setVisible(true);
      field_privatekey.setVisible(false);
    } else {
      field_password.setVisible(false);
      field_privatekey.setVisible(true);
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
  onSaveRecord: function() {
    var me = this;
    var tab = me.getEditorJobunitTab();
    var panel = tab.child('#comm_sshjob');
    var form_jobunit = panel.child('#comm_jobunit');
    var form_sshjob = panel.child('#sshjob_detail');
    var grid_error = panel.child('#sshjob_error');
    var grid_branch = panel.child('#sshjob_branch');
    var rec_jobunit = form_jobunit.getRecord();
    var rec_sshjob = form_sshjob.getRecord();

    // Update
    form_jobunit.updateRecord();
    form_sshjob.updateRecord();

    // Save
    grid_error.getStore().sync();
    grid_branch.getStore().sync();
    rec_sshjob.save();

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
    var panel = tab.child('#comm_sshjob');
    var form_jobunit = panel.child('#comm_jobunit');

    record = form_jobunit.getRecord();
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
    var panel = tab.child('#comm_sshjob');
    var form_jobunit = panel.child('#comm_jobunit');
    var form_sshjob = panel.child('#sshjob_detail');
    var grid_error = panel.child('#sshjob_error');
    var grid_branch = panel.child('#sshjob_branch');

    JhakoSetForm(form_jobunit, record);
    Jhako.model.Jobunit.load(record.data.id, {
      success: function(rec) {
        var rec_sshjob = rec['Jhako.model.SshjobHasOneInstance'];
        if (rec_sshjob) {
          rec_sshjob.getProxy().url = location.pathname + '/jobunits/' + rec.data.id + '/sshjobs';
          JhakoSetForm(form_sshjob, rec_sshjob);
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
    var panel = tab.child('#comm_sshjob');
    var form_jobunit = panel.child('#comm_jobunit');
    var form_sshjob = panel.child('#sshjob_detail');
    var grid_error = panel.child('#sshjob_error');
    var grid_branch = panel.child('#sshjob_branch');

    JhakoSetForm(form_jobunit, record);
    JhakoSetForm(form_sshjob, record['Jhako.model.SshjobHasOneInstance']);

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
    var panel = tab.child('#comm_sshjob');
    var form_jobunit = panel.child('#comm_jobunit');
    var form_sshjob = panel.child('#sshjob_detail');
    var grid_error = panel.child('#sshjob_error');
    var grid_branch = panel.child('#sshjob_branch');

    JhakoSetForm(form_jobunit, record);
    JhakoSetForm(form_sshjob, record['Jhako.model.SshjobHasOneInstance']);

    var store_error = JhakoGetCondition(record, CONDITION_METHOD_ERROR, 'hist_jobunits');
    var store_branch = JhakoGetCondition(record, CONDITION_METHOD_BRANCH, 'hist_jobunits');
    grid_error.reconfigure(store_error);
    grid_branch.reconfigure(store_branch);
  },

});