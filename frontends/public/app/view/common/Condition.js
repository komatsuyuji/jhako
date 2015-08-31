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
Ext.define('Jhako.view.common.Condition', {
  alias: 'widget.commonCondition',
  extend: 'Ext.grid.Panel',
  title: 'Condition',
  columns: {
    items: [{
      text: I18n.t('views.column.kind'),
      dataIndex: 'kind',
      width: 100,
      editable: false,
      editor: {
        xtype: 'combobox',
        editable: false,
        store: CONDITION_KIND_DATA,
      },
      renderer: function(value) {
        return JhakoFindRecord(CONDITION_KIND_DATA, value);
      }
    }, {
      text: I18n.t('views.condition.cond'),
      dataIndex: 'cond',
      width: 200,
      editor: {
        xtype: 'textfield',
        allowBlank: false,
      },
    }, {
      xtype: 'checkcolumn',
      text: I18n.t('views.condition.negative'),
      dataIndex: 'negative',
      editable: false,
      width: 70,
    }]
  },
  plugins: [{
    ptype: 'cellediting',
    clicksToEdit: 1,
    pluginId: 'cellediting_condition'
  }],
  bbar: [{
    text: I18n.t('views.button.add'),
    icon: location.pathname + '/images/icons/add.png',
    disabled: true,
    action: 'add_condition'
  }, '-', {
    text: I18n.t('views.button.delete'),
    icon: location.pathname + '/images/icons/delete.png',
    disabled: true,
    action: 'delete_condition'
  }, '->', {
    text: I18n.t('views.button.clear'),
    icon: location.pathname + '/images/icons/bin_closed.png',
    disabled: true,
    action: 'clear_condition'
  }],
  initComponent: function() {
    this.callParent();
  }
});