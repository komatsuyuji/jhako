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
Ext.define('Jhako.view.common.Variable', {
  alias: 'widget.commonVariable',
  extend: 'Ext.grid.Panel',
  title: I18n.t('views.model.variable'),
  icon: location.pathname + '/images/icons/basket.png',
  itemId: 'comm_variable',
  columns: {
    items: [{
      text: I18n.t('views.variable.name'),
      dataIndex: 'name',
      width: 100,
      editor: {
        xtype: 'textfield',
        allowBlank: false,
        minLength: 3,
        maxLength: 255,
        regex: /^[A-Z][A-Z0-9_]+$/
      },
    }, {
      text: I18n.t('views.variable.value'),
      dataIndex: 'value',
      width: 250,
      editor: {
        xtype: 'textfield',
        allowBlank: false,
      },
    }, {
      text: I18n.t('views.column.created_at'),
      dataIndex: 'created_at',
      width: 120,
      hidden: true,
      xtype: 'datecolumn',
      format: 'Y-m-d H:i:s'
    }, {
      text: I18n.t('views.column.updated_at'),
      dataIndex: 'updated_at',
      width: 120,
      hidden: true,
      xtype: 'datecolumn',
      format: 'Y-m-d H:i:s'
    }]
  },
  plugins: [{
    ptype: 'cellediting',
    clicksToEdit: 1,
    pluginId: 'cellediting_variable'
  }],
  /*
  bbar: [{
    text: I18n.t('views.button.add'),
    icon: location.pathname + '/images/icons/add.png',
    disabled: false,
    action: 'add_variable'
  }, '-', {
    text: I18n.t('views.button.delete'),
    icon: location.pathname + '/images/icons/delete.png',
    disabled: false,
    action: 'delete_variable'
  }, '->', {
    text: I18n.t('views.button.clear'),
    icon: location.pathname + '/images/icons/bin_closed.png',
    disabled: false,
    action: 'clear_variable'
  }],
  */
  initComponent: function() {
    this.callParent();
  }
});
