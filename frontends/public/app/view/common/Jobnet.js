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
Ext.define('Jhako.view.common.Jobnet', {
  alias: 'widget.commonJobnet',
  extend: 'Ext.panel.Panel',
  title: 'jobnet',
  layout: 'border',
  autoScroll: true,
  frame: true,
  itemId: 'comm_jobnet',
  items: [{
    xtype: 'form',
    region: 'north',
    itemId: 'comm_rootjobnet',
    bodyStyle: {
      backgroundColor: '#dfe8f6'
    },
    border: false,
    items: [{
      xtype: 'checkboxfield',
      fieldLabel: I18n.t('views.rootjobnet.disabled'),
      name: 'disabled',
      checked: false,
      readOnly: true
    }, {
      xtype: 'datefield',
      fieldLabel: I18n.t('views.rootjobnet.active_since'),
      name: 'active_since',
      format: 'Y-m-d',
      readOnly: true
    }, {
      xtype: 'datefield',
      fieldLabel: I18n.t('views.rootjobnet.active_till'),
      name: 'active_till',
      format: 'Y-m-d',
      readOnly: true
    }, {
      xtype: 'combobox',
      fieldLabel: I18n.t('views.model.calendar'),
      name: 'calendar_id',
      store: 'CalendarCombo',
      displayField: 'name',
      valueField: 'id',
      readOnly: true,
      editable: false
    }, {
      xtype: 'fieldset',
      title: I18n.t('views.title.advanced'),
      collapsible: true,
      collapsed: true,
      items: [{
        xtype: 'combobox',
        fieldLabel: I18n.t('views.rootjobnet.run_type'),
        name: 'run_type',
        readOnly: true,
        editable: false,
        store: ROOTJOBNET_RUNTYPE_DATA
      }, {
        xtype: 'combobox',
        fieldLabel: I18n.t('views.rootjobnet.delay_limit'),
        name: 'delay_limit',
        readOnly: true,
        editable: false,
        store: ROOTJOBNET_DELAY_DATA
      }, {
        xtype: 'combobox',
        fieldLabel: I18n.t('views.rootjobnet.loader_interval'),
        name: 'loader_interval',
        readOnly: true,
        editable: false,
        store: ROOTJOBNET_LOADER_DATA
      }]
    }]
  }, {
    xtype: 'commonSchedule',
    region: 'north',
    height: 150
  }, {
    xtype: 'commonAlarm',
    region: 'north',
    height: 150
  }, {
    xtype: 'commonJobunit',
    region: 'north',
  }],

  bbar: [{
    xtype: 'datefield',
    name: 'specified_date',
    format: 'Y-m-d',
    width: 90,
    minValue: new Date(),
    itemId: 'df_manualrun'
  }, {
    xtype: 'timefield',
    name: 'specified_time',
    minValue: '0:00',
    maxValue: '24:00',
    increment: 5,
    width: 60,
    format: 'H:i',
    itemId: 'tf_manualrun'
  }, {
    text: I18n.t('views.button.run'),
    icon: location.pathname + '/images/icons/control_play_blue.png',
    tooltip: I18n.t('views.title.manual_run'),
    itemId: 'btn_manualrun',
    action: 'manualrun_jobnet'
  }],

  initComponent: function() {
    this.callParent();
  },
});