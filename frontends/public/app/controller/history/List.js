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


Ext.define('Jhako.controller.history.List', {
  extend: 'Ext.app.Controller',
  refs: [{
    ref: 'historyList',
    selector: 'historyList'
  }, {
    ref: 'historyShow',
    selector: 'historyShow'
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
      'historyList': {
        afterrender: this.onAfterrender,
        cellclick: this.onCellclick,
      },
      'historyList button[action=clear_filter]': {
        click: this.onClearfilter
      },
      'historyList menuitem[action=set_interval]': {
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
  onAfterrender: function(grid) {
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
  onCellclick: function(grid, td, cellIndex, record, tr, rowIndex, e) {
    if (!record)
      return;
    var me = this;

    var ctrl = Jhako.app.getController('history.Flowchart');
    ctrl.onReset();
    me.onLoadJobnet(record.data.id);
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
  onLoadJobnet: function(id) {
    if (!id)
      return;

    var me = this;
    var form = me.getHistoryShow();
    var ctrl = Jhako.app.getController('history.Flowchart');
    Jhako.model.HistJobunit.load(id, {
      failure: function(record) {},
      success: function(record) {
        JhakoSetForm(form, record);
        ctrl.onDrawFlowchart(record);
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
  onClearfilter: function(button, e) {
    var me = this;
    var grid = me.getHistoryList();
    grid.filters.clearFilters();
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
    jhako_history_interval = item.interval;
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
    var grid = me.getHistoryList();
    var menuitems = grid.query('menuitem');

    menuitems.forEach(function(item) {
      if (item.interval) {
        if (jhako_history_interval == item.interval) {
          item.setIconCls('');
        } else {
          item.setIconCls('x-hide-display');
        }
      }
    });
  },

});