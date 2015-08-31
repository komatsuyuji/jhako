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

Ext.define('Jhako.controller.common.Execlog', {
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
    var panel = sidebar.child('#comm_execlog');
    var store_execlog = record['jhako.model.execlogsStore'];

    if (!store_execlog) {
      panel.body.update('');
      return;
    }

    var text = '';
    store_execlog.sort('id', 'ASC');
    store_execlog.each(function(rec) {
      text = text + Ext.util.Format.date(rec.data.created_at, "Y-m-d H:i:s ") + rec.data.message + '<br>';
    });
    panel.body.update(text);
  },

});