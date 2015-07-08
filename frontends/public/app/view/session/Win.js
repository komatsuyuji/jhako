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
Ext.define('Jhako.view.session.Win', {
  alias: 'widget.sessionWin',
  extend: 'Ext.Window',
  title: 'Loign Window',
  width: 300,
  autoHeight: true,
  closable: false,
  resizable: false,
  draggable: false,
  layout: 'fit',
  border: false,
  modal: true,
  items: [{
    xtype: 'sessionForm'
  }],
  buttons: [{
    text: "Login",
    type: "submit",
    action: "login",
    formBind: true,
  }],
  initComponent: function() {
    this.callParent();
  }
});
