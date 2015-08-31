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
Ext.define('Jhako.view.calendar.Date', {
  alias: 'widget.calendarDate',
  extend: 'Ext.panel.Panel',
  frame: true,
  layout: {
    type: 'table',
    columns: 4
  },

  defaults: {
    xtype: 'datepicker',
    showToday: false,
    //monthYearFormat: 'Y/m',
    longDayFormat: 'Y/m/d',
    prevText: '',
    monthYearText: '',
    nextText: '',
    minText: '',
    maxText: '',
    listeners: {
      afterrender: function(datepicker) {
        var btn = datepicker.monthBtn;
        btn.setDisabled(true);
        datepicker.prevEl.setVisible(false);
        datepicker.nextEl.setVisible(false);
      }
    }
  },
  items: [{
    itemId: 'calemonth01',
  }, {
    itemId: 'calemonth02',
  }, {
    itemId: 'calemonth03',
  }, {
    itemId: 'calemonth04',
  }, {
    itemId: 'calemonth05',
  }, {
    itemId: 'calemonth06',
  }, {
    itemId: 'calemonth07',
  }, {
    itemId: 'calemonth08',
  }, {
    itemId: 'calemonth09',
  }, {
    itemId: 'calemonth10',
  }, {
    itemId: 'calemonth11',
  }, {
    itemId: 'calemonth12',
  }],

  initComponent: function() {
    this.callParent();
  }
});