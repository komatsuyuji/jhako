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
Ext.define('Jhako.view.common.Sleepjob', {
  alias: 'widget.commonSleepjob',
  extend: 'Ext.container.Container',
  title: 'sleep job',
  layout: 'border',
  autoScroll: true,
  itemId: 'comm_sleepjob',
  items: [{
    xtype: 'commonJobunit',
    region: 'north',
  }, {
    xtype: 'form',
    region: 'north',
    itemId: 'sleepjob_detail',
    bodyStyle: {
      backgroundColor: '#dfe8f6'
    },
    border: false,
    items: [{
      xtype: 'combobox',
      fieldLabel: I18n.t('views.date.hour'),
      name: 'hour',
      readOnly: true,
      editable: false,
      store: JHAKO_HOUR_DATA
    }, {
      xtype: 'combobox',
      fieldLabel: I18n.t('views.date.min'),
      name: 'minute',
      readOnly: true,
      editable: false,
      store: JHAKO_MINUTE_DATA
    }, {
      xtype: 'combobox',
      fieldLabel: I18n.t('views.date.sec'),
      name: 'second',
      readOnly: true,
      editable: false,
      store: JHAKO_SECOND_DATA
    }],
  }],

  initComponent: function() {
    this.callParent();
  }
});