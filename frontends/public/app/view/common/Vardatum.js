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
Ext.define('Jhako.view.common.Vardatum', {
  alias: 'widget.commonVardatum',
  extend: 'Ext.grid.Panel',
  title: I18n.t('views.model.vardatum'),
  itemId: 'comm_vardatum',
  columns: {
    items: [{
      text: I18n.t('views.vardatum.name'),
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
      text: I18n.t('views.vardatum.value'),
      dataIndex: 'value',
      width: 250,
      editor: {
        xtype: 'textfield',
        allowBlank: false,
      },
    }, {
      xtype: 'checkcolumn',
      text: I18n.t('views.vardatum.overwrite'),
      dataIndex: 'overwrite',
      editable: false,
      width: 70,
    }]
  },
  plugins: [{
    ptype: 'cellediting',
    clicksToEdit: 1,
    pluginId: 'cellediting_vardatum'
  }],
  bbar: [{
    text: I18n.t('views.button.add'),
    icon: location.pathname + '/images/icons/add.png',
    disabled: true,
    action: 'add_vardatum'
  }, '-', {
    text: I18n.t('views.button.delete'),
    icon: location.pathname + '/images/icons/delete.png',
    disabled: true,
    action: 'delete_vardatum'
  }, '->', {
    text: I18n.t('views.button.clear'),
    icon: location.pathname + '/images/icons/bin_closed.png',
    disabled: true,
    action: 'clear_vardatum'
  }],
  initComponent: function() {
    this.callParent();
  }
});
