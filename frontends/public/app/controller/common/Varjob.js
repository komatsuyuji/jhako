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

Ext.define('Jhako.controller.common.Varjob', {
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
    var panel = tab.child('#comm_varjob');
    var form_jobunit = panel.child('#comm_jobunit');
    var grid_vardatum = panel.child('#comm_vardatum');
    var rec_jobunit = form_jobunit.getRecord();

    // Save
    form_jobunit.updateRecord();
    grid_vardatum.getStore().sync();
    rec_jobunit.save();

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
    var panel = tab.child('#comm_varjob');
    var form_jobunit = panel.child('#comm_jobunit');

    var record = form_jobunit.getRecord();
    if (!record)
      return;

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
    var panel = tab.child('#comm_varjob');
    var form_jobunit = panel.child('#comm_jobunit');
    var grid_vardatum = panel.child('#comm_vardatum');

    JhakoSetForm(form_jobunit, record);

    Jhako.model.Jobunit.load(record.data.id, {
      success: function(rec) {
        var store_vardatum = rec['jhako.model.vardataStore'];
        if (store_vardatum) {
          store_vardatum.getProxy().url = location.pathname + '/jobunits/' + rec.data.id + '/vardata';
          grid_vardatum.reconfigure(store_vardatum);
        }
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
    var panel = tab.child('#comm_varjob');
    var form_jobunit = panel.child('#comm_jobunit');
    var grid_vardatum = panel.child('#comm_vardatum');

    JhakoSetForm(form_jobunit, record);
    if (record['jhako.model.vardataStore']) {
      grid_vardatum.reconfigure(record['jhako.model.vardataStore']);
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
    var tab = me.getHistoryJobunitTab();
    var panel = tab.child('#comm_varjob');
    var form_jobunit = panel.child('#comm_jobunit');
    var grid_vardatum = panel.child('#comm_vardatum');

    JhakoSetForm(form_jobunit, record);
    if (record['jhako.model.vardataStore']) {
      grid_vardatum.reconfigure(record['jhako.model.vardataStore']);
    }
  },

});