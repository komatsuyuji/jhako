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
Ext.define('Jhako.view.user.Profile', {
  alias: 'widget.userProfile',
  extend: 'Ext.Window',
  title: 'User Profile',
  width: 500,
  autoHeight: true,
  closable: false,
  resizable: false,
  draggable: false,
  layout: 'fit',
  border: false,
  modal: true,
  hidden: true,
  items: [{
    xtype: 'userForm',
  }],
  buttons: [{
    text: 'Save',
    icon: location.pathname + '/images/icons/disk.png',
    action: 'save_user'
  }, '-', {
    text: 'Cancel',
    icon: location.pathname + '/images/icons/cancel.png',
    action: 'cancel_user'
  }],
  initComponent: function() {
    this.callParent();
  }
});
