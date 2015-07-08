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

Ext.define('Jhako.controller.common.Jobresult', {
  extend: 'Ext.app.Controller',
  refs: [{
    ref: 'processSidebar',
    selector: 'processSidebar'
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
  onLoadProcRecord: function(record) {
    if (!record)
      return;

    var me = this;
    var sidebar = me.getProcessSidebar();
    var panel = sidebar.child('#comm_jobresult');
    var form_jobunit = panel.child('#jobresult_jobunit');
    var form_jobresult = panel.child('#jobresult_detail');
    var store_jobresult = record['jhako.model.jobresultsStore'];

    JhakoSetForm(form_jobunit, record);
    if (store_jobresult) {
      store_jobresult.sort('id', 'DESC');
      JhakoSetForm(form_jobresult, store_jobresult.getAt(0));
    } else {
      JhakoSetForm(form_jobresult, null);
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
    if (!record) return;

    var me = this;
    var sidebar = me.getHistorySidebar();
    var panel = sidebar.child('#comm_jobresult');
    var form_jobunit = panel.child('#jobresult_jobunit');
    var form_jobresult = panel.child('#jobresult_detail');
    var store_jobresult = record['jhako.model.jobresultsStore'];

    JhakoSetForm(form_jobunit, record);
    if (store_jobresult) {
      store_jobresult.sort('id', 'DESC');
      JhakoSetForm(form_jobresult, store_jobresult.getAt(0));
    } else {
      JhakoSetForm(form_jobresult, null);
    }
  },

});
