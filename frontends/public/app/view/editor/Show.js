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
Ext.define('Jhako.view.editor.Show', {
  alias: 'widget.editorShow',
  extend: 'Ext.form.Panel',
  bodyStyle: {
    backgroundColor: '#dfe8f6'
  },
  border: false,
  layout: 'hbox',
  defaults: {
    xtype: 'displayfield',
    labelAlign: 'right',
  },
  items: [{
    fieldLabel: I18n.t('views.column.name'),
    name: 'name',
  }, {
    fieldLabel: I18n.t('views.column.description'),
    name: 'description',
  }, {
    fieldLabel: I18n.t('views.column.created_at'),
    name: 'created_at',
    dateFormat: 'Y-m-d H:i:s',
  }, {
    fieldLabel: I18n.t('views.column.updated_at'),
    name: 'updated_at',
    dateFormat: 'Y-m-d H:i:s',
  }],

  initComponent: function() {
    this.callParent();
  }
});