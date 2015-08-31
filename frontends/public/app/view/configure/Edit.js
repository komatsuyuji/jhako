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
Ext.define('Jhako.view.configure.Edit', {
  alias: 'widget.configureEdit',
  extend: 'Ext.form.Panel',
  title: I18n.t('views.model.configure'),
  icon: location.pathname + '/images/icons/cog.png',
  frame: true,
  defaults: {
    labelWidth: 300
  },
  items: [{
    xtype: 'combobox',
    fieldLabel: I18n.t('views.configure.end_period'),
    name: 'end_period',
    editable: false,
    store: CONFIGURE_VIEWER_DATA
  }, {
    xtype: 'combobox',
    fieldLabel: I18n.t('views.configure.error_period'),
    name: 'error_period',
    editable: false,
    store: CONFIGURE_VIEWER_DATA
  }, {
    xtype: 'combobox',
    fieldLabel: I18n.t('views.configure.outschedule_period'),
    name: 'outschedule_period',
    editable: false,
    store: CONFIGURE_VIEWER_DATA
  }, {
    xtype: 'combobox',
    fieldLabel: I18n.t('views.configure.carryover_period'),
    name: 'carryover_period',
    editable: false,
    store: CONFIGURE_VIEWER_DATA
  }, {
    xtype: 'combobox',
    fieldLabel: I18n.t('views.configure.forcestop_period'),
    name: 'forcestop_period',
    editable: false,
    store: CONFIGURE_VIEWER_DATA
  }, {
    xtype: 'combobox',
    fieldLabel: I18n.t('views.configure.history_period') + '(' + I18n.t('views.date.day') + ')',
    name: 'history_period',
    editable: false,
    store: [1, 7, 30, 90, 180]
  }],

  initComponent: function() {
    this.callParent();
  }
});