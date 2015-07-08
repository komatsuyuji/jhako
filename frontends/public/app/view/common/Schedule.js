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
Ext.define('Jhako.view.common.Schedule', {
  alias: 'widget.commonSchedule',
  extend: 'Ext.grid.Panel',
  title: I18n.t('views.model.schedule'),
  itemId: 'comm_schedule',
  columns: {
    defaults: {
      width: 75,
      editor: {
        xtype: 'textfield'
      }
    },
    items: [{
      text: I18n.t('views.date.week'),
      dataIndex: 'week',
      editor: {
        xtype: 'combobox',
        multiSelect: true,
        delimiter: ',',
        store: [
          0, 1, 2, 3, 4, 5, 6
        ],
        allowBlank: false,
        regex: /^((\d+(-\d+){0,1},)*\d+(-\d+){0,1}|\*)$/
      },
    }, {
      text: I18n.t('views.date.month'),
      dataIndex: 'month',
      editor: {
        xtype: 'combobox',
        multiSelect: true,
        delimiter: ',',
        store: [
          1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12
        ],
        allowBlank: false,
        regex: /^((\d+(-\d+){0,1},)*\d+(-\d+){0,1}|\*)$/
      }
    }, {
      text: I18n.t('views.date.day'),
      dataIndex: 'day',
      editor: {
        xtype: 'combobox',
        multiSelect: true,
        delimiter: ',',
        store: JHAKO_DAY_DATA,
        allowBlank: false,
        regex: /^((\d+(-\d+){0,1},)*\d+(-\d+){0,1}|\*)$/
      }
    }, {
      text: I18n.t('views.date.hour'),
      dataIndex: 'hour',
      editor: {
        xtype: 'combobox',
        multiSelect: true,
        delimiter: ',',
        store: JHAKO_HOUR_DATA,
        allowBlank: false,
        regex: /^((\d+(-\d+){0,1},)*\d+(-\d+){0,1}|\*)$/
      }
    }, {
      text: I18n.t('views.date.min'),
      dataIndex: 'minute',
      editor: {
        xtype: 'combobox',
        multiSelect: true,
        delimiter: ',',
        store: JHAKO_MINUTE_DATA,
        allowBlank: false,
        regex: /^((\d+(-\d+){0,1},)*\d+(-\d+){0,1}|\*)$/
      }
    }]
  },
  plugins: [{
    ptype: 'cellediting',
    clicksToEdit: 1,
    pluginId: 'cellediting_schedule'
  }],
  bbar: [{
    text: I18n.t('views.button.add'),
    icon: location.pathname + '/images/icons/add.png',
    disabled: true,
    action: 'add_schedule'
  }, '-', {
    text: I18n.t('views.button.delete'),
    icon: location.pathname + '/images/icons/delete.png',
    disabled: true,
    action: 'delete_schedule'
  }, '->', {
    text: I18n.t('views.button.clear'),
    icon: location.pathname + '/images/icons/bin_closed.png',
    disabled: true,
    action: 'clear_schedule'
  }],
  initComponent: function() {
    this.callParent();
  }
});
