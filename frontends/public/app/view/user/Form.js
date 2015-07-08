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
Ext.define('Jhako.view.user.Form', {
  alias: 'widget.userForm',
  extend: 'Ext.form.Panel',
  plain: true,
  frame: true,
  border: 0,
  bodyPadding: 5,
  items: [{
    xtype: 'textfield',
    fieldLabel: I18n.t('views.user.name'),
    name: 'name',
    readOnly: true,
  }, {
    xtype: 'textfield',
    fieldLabel: I18n.t('views.user.fullname'),
    name: 'fullname',
  }, {
    xtype: 'combobox',
    fieldLabel: I18n.t('views.column.kind'),
    name: 'kind',
    store: USER_KIND_DATA,
    readOnly: true,
  }, {
    xtype: 'textfield',
    fieldLabel: I18n.t('views.user.password'),
    name: 'password',
    inputType: 'password',
    minLength: 4,
  }, {
    xtype: 'textfield',
    fieldLabel: I18n.t('views.user.password_confirmation'),
    name: 'password_confirmation',
    inputType: 'password',
    minLength: 4,
  }, {
    xtype: 'combobox',
    fieldLabel: I18n.t('views.user.language'),
    name: 'language',
    editable: false,
    store: USER_LANGUAGE_DATA
  }, {
    xtype: 'textfield',
    fieldLabel: I18n.t('views.user.email'),
    name: 'email',
    vtype: 'email',
    width: 400
  }],
  initComponent: function() {
    this.callParent();
  }
});
