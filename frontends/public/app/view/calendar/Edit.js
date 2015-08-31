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
Ext.define('Jhako.view.calendar.Edit', {
  alias: 'widget.calendarEdit',
  extend: 'Ext.form.Panel',
  frame: true,
  layout: 'hbox',
  items: [{
    xtype: 'textfield',
    labelAlign: 'right',
    fieldLabel: I18n.t('views.column.name'),
    name: 'name',
    allowBlank: false,
    minLength: 3,
    maxLength: 255,
    regex: /^[a-z]\w+$/i
  }, {
    xtype: 'textfield',
    labelAlign: 'right',
    fieldLabel: I18n.t('views.column.description'),
    name: 'description',
    width: 350
  }, {
    xtype: 'numberfield',
    itemId: 'selectedyear',
    name: 'selectedyear',
    labelAlign: 'right',
    fieldLabel: I18n.t('views.date.year'),
    labelWidth: 50,
    maxWidth: 130,
    editable: false,
    value: JHAKO_THIS_YEAR,
    maxValue: JHAKO_THIS_YEAR + 100,
    minValue: JHAKO_THIS_YEAR,
  }],

  initComponent: function() {
    this.callParent();
  }
});