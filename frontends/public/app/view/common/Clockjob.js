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
Ext.define('Jhako.view.common.Clockjob', {
  alias: 'widget.commonClockjob',
  extend: 'Ext.container.Container',
  title: 'clock job',
  layout: 'border',
  autoScroll: true,
  itemId: 'comm_clockjob',
  items: [{
    xtype: 'commonJobunit',
    region: 'north',
  }, {
    xtype: 'form',
    region: 'north',
    itemId: 'clockjob_detail',
    bodyStyle: {
      backgroundColor: '#dfe8f6'
    },
    border: false,
    items: [{
      xtype: 'combobox',
      fieldLabel: I18n.t('views.clockjob.basetime'),
      name: 'basetime',
      allowBlank: false,
      readOnly: true,
      editable: false,
      store: [
        [0, I18n.t('views.clockjob_basetime.job_startday')],
        [1, I18n.t('views.clockjob_basetime.job_starttime')],
        [2, I18n.t('views.clockjob_basetime.jobnet_startday')],
        [3, I18n.t('views.clockjob_basetime.jobnet_starttime')],
        [4, I18n.t('views.clockjob_basetime.rootjobnet_startday')],
        [5, I18n.t('views.clockjob_basetime.rootjobnet_starttime')]
      ],
    }, {
      xtype: 'datefield',
      fieldLabel: I18n.t('views.clockjob.predetermined_time'),
      name: 'predetermined_time',
      itemId: 'clockjob_predetermined_time',
      format: 'Y-m-d H:i',
      readOnly: true,
      editable: false,
    }, {
      xtype: 'combobox',
      fieldLabel: I18n.t('views.date.day'),
      name: 'day',
      readOnly: true,
      editable: false,
      store: [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
    }, {
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
      fieldLabel: I18n.t('views.clockjob.timeover'),
      name: 'timeover',
      allowBlank: false,
      editable: false,
      readOnly: true,
      store: [
        [0, I18n.t('views.clockjob_timeover.normal')],
        [1, I18n.t('views.clockjob_timeover.branch')],
        [2, I18n.t('views.clockjob_timeover.error')]
      ],
    }, {
      xtype: 'combobox',
      fieldLabel: I18n.t('views.clockjob.delay_limit') + '(' + I18n.t('views.date.min') + ')',
      name: 'delay_limit',
      readOnly: true,
      editable: false,
      store: ROOTJOBNET_DELAY_DATA
    }],
  }],

  initComponent: function() {
    this.callParent();
  }
});
