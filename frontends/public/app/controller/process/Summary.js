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

Ext.define('Jhako.controller.process.Summary', {
  extend: 'Ext.app.Controller',
  refs: [{
    ref: 'processSummary',
    selector: 'processSummary'
  }, {
    ref: 'jhakoViewer',
    selector: 'jhakoViewer'
  }, {
    ref: 'processList',
    selector: 'processList'
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
      'processSummary': {
        afterrender: this.onAfterrender,
        cellclick: this.onCellclick,
      },
      'processSummary menuitem[action=set_interval]': {
        click: this.onSetInterval
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
  onAfterrender: function(grid, e) {
    var me = this;
    me.onSetIconCls();
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
  onCellclick: function(gridview, td, cellIndex, record, tr, rowIndex, e, eOpts) {
    var me = this;
    var panel = me.getProcessSummary();
    if (panel.columns.length != cellIndex + 1)
      return;

    var tab = me.getJhakoViewer();
    var grid = me.getProcessList();
    var filters = grid.filters;
    var filter_status = filters.getFilter('status');
    if (!filter_status) {
      filters.addFilter({
        type: 'list',
        dataIndex: 'status',
        options: JOBUNIT_STATUS_DATA,
        phpMode: true,
      });
      filter_status = filters.getFilter('status');
    }

    filter_status.setValue(record.data.status);
    filter_status.setActive(true);

    tab.setActiveTab('process_index');
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
  onSetInterval: function(item, e) {
    var me = this;
    jhako_summary_interval = item.interval;
    me.onSetIconCls();
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
  onSetIconCls: function() {
    var me = this;
    var grid = me.getProcessSummary();
    var menuitems = grid.query('menuitem');

    menuitems.forEach(function(item) {
      if (item.interval) {
        if (jhako_summary_interval == item.interval) {
          item.setIconCls('');
        } else {
          item.setIconCls('x-hide-display');
        }
      }
    });
  },

});
