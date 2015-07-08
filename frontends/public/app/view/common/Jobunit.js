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
Ext.define('Jhako.view.common.Jobunit', {
  alias: 'widget.commonJobunit',
  extend: 'Ext.form.Panel',
  itemId: 'comm_jobunit',
  bodyStyle: {
    backgroundColor: '#dfe8f6'
  },
  border: false,
  items: [{
    xtype: 'textfield',
    fieldLabel: I18n.t('views.column.name'),
    name: 'name',
    itemId: 'jobunit_name',
    readOnly: true,
    allowBlank: false,
    minLength: 3,
    maxLength: 255,
    regex: /^[a-z]\w+$/i
  }, {
    xtype: 'textfield',
    fieldLabel: I18n.t('views.column.description'),
    name: 'description',
    itemId: 'jobunit_description',
    readOnly: true,
  }, {
    xtype: 'checkboxfield',
    boxLabel: I18n.t('views.jobunit.hold'),
    name: 'hold',
    readOnly: true,
    checked: false
  }, {
    xtype: 'checkboxfield',
    boxLabel: I18n.t('views.jobunit.skip'),
    name: 'skip',
    readOnly: true,
    checked: false
  }, {
    xtype: 'textfield',
    fieldLabel: I18n.t('views.jobunit.timeout') + '(' + I18n.t('views.date.min') + ')',
    name: 'timeout',
    itemId: 'jobunit_timeout',
    value: 0,
    readOnly: true,
    allowBlank: false,
    maxLength: 8,
    regex: /^\d+$/,
  }],

  initComponent: function() {
    this.callParent();
  }
});
