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

Ext.define('Jhako.controller.user.Profile', {
  extend: 'Ext.app.Controller',
  refs: [{
    ref: 'userProfile',
    selector: 'userProfile'
  }, {
    ref: 'userForm',
    selector: 'userForm'
  }],

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
  init: function() {
    this.control({
      'userForm': {
        afterrender: this.onAfterrender,
      },
      'userProfile button[action=save_user]': {
        click: this.onSaveUser,
      },
      'userProfile button[action=cancel_user]': {
        click: this.onCancelUser,
      },
    });
  },

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
  onAfterrender: function(form) {
    var me = this;

    if (!jhako_login_user)
      return;

    form.loadRecord(jhako_login_user);
  },

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
  onSaveUser: function(button, e) {
    var me = this;
    var win = me.getUserProfile();
    var form = me.getUserForm();
    form.updateRecord();
    var record = form.getRecord();
    if (!record)
      return;
    record.save({
      success: function(rec, operation) {
        window.location.reload();
      },
    });
  },

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
  onCancelUser: function(button, e) {
    var me = this;
    var win = me.getUserProfile();
    var form = me.getUserForm();
    form.loadRecord(jhako_login_user);
    win.hide();
  },

});
