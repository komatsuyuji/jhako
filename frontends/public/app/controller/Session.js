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

Ext.define('Jhako.controller.Session', {
  extend: 'Ext.app.Controller',
  refs: [{
    ref: 'sessionForm',
    selector: 'sessionForm'
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
      'sessionForm': {
        afterrender: this.onAfterrender,
      },
      'sessionWin button[action=login]': {
        click: this.onLogin,
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
    var record = new Jhako.model.Session();
    form.loadRecord(record);
    var pw = form.child('#tf_password');

    pw.getEl().on('keydown', function(e) {
      switch (e.getKey()) {
        case e.ENTER:
          {
            me.onLogin();
            break;
          }
        default:
          {
            break;
          }
      }
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
  onLogin: function(button, e) {
    var me = this;
    var form = me.getSessionForm();
    form.updateRecord();
    var record = form.getRecord();
    record.save({
      callback: function(rec, operation, success) {
        if (success) {
          window.location.reload();
        } else {
          Ext.Msg.alert('Login Failed', 'Invalid username/password combination');
        }
      }
    });
  },
});
