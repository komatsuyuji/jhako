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

Ext.define('Jhako.controller.user.List', {
  extend: 'Ext.app.Controller',
  refs: [{
    ref: 'userList',
    selector: 'userList'
  }, {
    ref: 'userEdit',
    selector: 'userEdit'
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
      'userList': {
        cellclick: this.onCellclick,
      },
      'userIndex button[action=clear_filter]': {
        click: this.onClearfilter,
      },
      'userIndex button[action=add_user]': {
        click: this.onAddUser,
      },
      'userIndex button[action=delete_user]': {
        click: this.onDeleteUser,
      },
      'userIndex button[action=save_user]': {
        click: this.onSaveUser,
      },
      'userIndex button[action=cancel_user]': {
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
  onCellclick: function(grid, td, cellIndex, record, tr, rowIndex, e) {
    if (!record)
      return;

    var me = this;
    var form = me.getUserEdit();
    form.setDisabled(false);
    form.loadRecord(record);
    if (record.data.id == 1) {
      // admin
      form.child('#user_name').setReadOnly(true);
      form.child('#user_fullname').setReadOnly(true);
      form.child('#user_kind').setReadOnly(true);
    } else {
      // other users
      form.child('#user_name').setReadOnly(false);
      form.child('#user_fullname').setReadOnly(false);
      form.child('#user_kind').setReadOnly(false);
    }
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
  onClearfilter: function(button, e) {
    var me = this;
    var grid = me.getUserList();
    grid.filters.clearFilters();
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
  onAddUser: function(button, e) {
    var me = this;
    var grid = me.getUserList();
    var form = me.getUserEdit();
    var store = grid.getStore();
    var cnt = store.getCount();
    var record = new Jhako.model.User();

    store.insert(cnt, record);
    grid.getSelectionModel().select(record);
    form.loadRecord(record);
    form.setDisabled(false);
    form.child('#user_name').setReadOnly(false);
    form.child('#user_fullname').setReadOnly(false);
    form.child('#user_kind').setReadOnly(false);
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
  onDeleteUser: function(button, e) {
    var me = this;
    var grid = me.getUserList();
    var form = me.getUserEdit();
    var store = grid.getStore();
    var record = form.getRecord();

    if (!record)
      return;
    if (!record.data.id)
      return;
    if (record.data.id <= 1)
      return;

    Ext.Msg.show({
      title: I18n.t('views.title.delete'),
      msg: I18n.t('views.msg.delete_user', {
        name: record.data.name
      }),
      buttons: Ext.Msg.YESNO,
      icon: Ext.Msg.QUESTION,
      fn: function(btn) {
        if (btn != "yes")
          return;

        store.remove(record);
        store.sync({
          success: function(records, operation) {
            store.reload();
            form.setDisabled(true);
            form.getForm().reset(true);
          }
        });
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
  onSaveUser: function(button, e) {
    var me = this;
    var grid = me.getUserList();
    var form = me.getUserEdit();
    var store = grid.getStore();
    form.updateRecord();
    var record = form.getRecord();
    if (!record)
      return;
    record.save({
      success: function(rec, operation) {
        if (!rec.data.id) {
          var data = Ext.JSON.decode(operation.response.responseText);
          rec.setId(data.id);
        }
        store.reload();
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
  onCancelUser: function(button, e) {
    var me = this;
    var form = me.getUserEdit();
    var record = form.getRecord();
    if (!record)
      return;
    form.loadRecord(record);
    if (!record.data.id) {
      form.setDisabled(true);
    }

    var grid = me.getUserList();
    grid.getStore().reload();
  },
});
