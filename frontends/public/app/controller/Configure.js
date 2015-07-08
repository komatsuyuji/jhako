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

Ext.define('Jhako.controller.Configure', {
  extend: 'Ext.app.Controller',
  refs: [{
    ref: 'configureEdit',
    selector: 'configureEdit'
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
      'configureIndex button[action=save_configure]': {
        click: this.onSaveConfigure,
      },
      'configureIndex button[action=cancel_configure]': {
        click: this.onCancelConfigure,
      },
      'configureEdit': {
        afterrender: this.onAfterrender,
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
  onSaveConfigure: function(button, e) {
    var me = this;
    var form = me.getConfigureEdit();
    form.updateRecord();
    var record = form.getRecord();
    if (!record)
      return;
    record.save({
      success: function(rec, operation) {
        form.loadRecord(rec);
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
  onCancelConfigure: function(button, e) {
    var me = this;
    var form = me.getConfigureEdit();
    var record = form.getRecord();
    if (!record)
      return;
    form.loadRecord(record);
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
  onAfterrender: function(form, e) {
    Jhako.model.Configure.load(1, {
      success: function(record, operation) {
        form.loadRecord(record);
      }
    });
  },
});
