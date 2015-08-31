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
Ext.define('Jhako.view.common.Alarm', {
  alias: 'widget.commonAlarm',
  extend: 'Ext.grid.Panel',
  title: I18n.t('views.model.alarm'),
  itemId: 'comm_alarm',
  columns: {
    items: [{
      text: I18n.t('views.jobunit.status'),
      dataIndex: 'status',
      width: 120,
      editor: {
        xtype: 'combobox',
        editable: false,
        readOnly: true,
        store: ALARM_STATUS_DATA,
        allowBlank: false,
      },
      renderer: function(value) {
        return JhakoFindRecord(ALARM_STATUS_DATA, value);
      }
    }, {
      text: I18n.t('views.alarm.path'),
      dataIndex: 'path',
      width: 200,
      editor: {
        xtype: 'triggerfield',
        itemId: 'trig_path',
        editable: false,
        onTriggerClick: function(e) {
          this.fireEvent("triggerclick", this, e);
        }
      },
    }, {
      text: 'Jobnet ID',
      dataIndex: 'jobnet_id',
      width: 50,
      hidden: true,
      editor: {
        xtype: 'textfield',
        editable: false,
        readOnly: true,
      },
    }]
  },
  plugins: [{
    ptype: 'cellediting',
    clicksToEdit: 1,
    pluginId: 'cellediting_alarm'
  }],
  bbar: [{
    text: I18n.t('views.button.add'),
    icon: location.pathname + '/images/icons/add.png',
    disabled: true,
    action: 'add_alarm'
  }, '-', {
    text: I18n.t('views.button.delete'),
    icon: location.pathname + '/images/icons/delete.png',
    disabled: true,
    action: 'delete_alarm'
  }, '->', {
    text: I18n.t('views.button.clear'),
    icon: location.pathname + '/images/icons/bin_closed.png',
    disabled: true,
    action: 'clear_alarm'
  }],
  initComponent: function() {
    this.callParent();
  }
});