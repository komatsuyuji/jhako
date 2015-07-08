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

Ext.define('Jhako.controller.Header', {
  extend: 'Ext.app.Controller',
  refs: [{
    ref: 'jhakoHeader',
    selector: 'jhakoHeader'
  }, {
    ref: 'userProfile',
    selector: 'userProfile'
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
      'jhakoHeader': {
        afterrender: this.onAfterrender,
      },
      'jhakoHeader displayfield[itemId=tf_username]': {
        afterrender: this.onAfterrenderUser,
      },
      'jhakoHeader button[action=logout]': {
        click: this.onLogout,
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
  onAfterrender: function(panel) {
    var me = this;
    if (!jhako_login_user)
      return;

    var field = panel.child('#tf_username');
    field.setValue(jhako_login_user.data.name);
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
  onAfterrenderUser: function(displayfield, e) {
    var me = this;
    var win = me.getUserProfile();
    displayfield.getEl().on('click', function(e, t, eOpts) {
      win.show();
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
  onLogout: function(button, e) {
    var session = new Jhako.model.Session();
    session.setId(1);
    session.destroy({
      callback: function() {
        window.location.reload();
      }
    });
  },

});
