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
Ext.define('Jhako.view.Header', {
  alias: 'widget.jhakoHeader',
  extend: 'Ext.container.Container',
  height: 35,
  defaults: {
    xtype: 'container'
  },
  layout: 'hbox',
  items: [{
    itemId: 'logo',
    html: {
      tag: 'a',
      href: 'http://www.jhako.com',
      html: 'jHako'
    },
    flex: 1
  }, {
    xtype: 'displayfield',
    fieldLabel: I18n.t('views.label.username'),
    name: 'name',
    value: '',
    itemId: 'tf_username',
    labelStyle: 'text-align: right;',
    padding: '0 10 0 0',
    fieldStyle: {
      'text-decoration': 'underline',
      'cursor': 'pointer'
    }
  }, {
    xtype: 'button',
    text: I18n.t('views.button.logout'),
    action: "logout",
  }],

  initComponent: function() {
    this.callParent();
  }
});
