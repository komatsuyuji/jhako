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

Ext.define('Jhako.controller.editor.TreeMenu', {
  extend: 'Ext.app.Controller',

  refs: [{
    ref: 'editorTree',
    selector: 'editorTree'
  }, {
    ref: 'editorTreeMenu',
    selector: 'editorTreeMenu'
  }, {
    ref: 'editorJobunitFile',
    selector: 'editorJobunitFile'
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
      'editorTreeMenu menuitem[action=export_jobunit]': {
        click: this.onExportJobunit
      },
      'editorTreeMenu menuitem[action=import_jobunit]': {
        click: this.onImportJobunit
      },
      'editorTreeMenu menuitem[action=delete_jobunit]': {
        click: this.onDeleteJobunit
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
  onExportJobunit: function(item, e) {
    var me = this;
    var tree = me.getEditorTree();
    var record = tree.getSelectionModel().getLastSelected();

    window.open(location.pathname + '/jobunits/' + record.data.id + '.json?export=1');
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
  onImportJobunit: function(item, e) {
    var me = this;
    var win = me.getEditorJobunitFile();
    win.show();
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
  onExportJobunit: function(item, e) {
    var me = this;
    var tree = me.getEditorTree();
    var record = tree.getSelectionModel().getLastSelected();

    window.open(location.pathname + '/jobunits/' + record.data.id + '.json?export=1');
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
  onDeleteJobunit: function(item, e) {
    var me = this;
    var tree = me.getEditorTree();
    var record = tree.getSelectionModel().getLastSelected();
    var parent_id = record.data.parent_id;

    if (record.data.id == 0)
      return;
    if (record.data.kind >= JOBUNIT_KIND_JOBNET)
      return;

    Ext.Msg.show({
      title: I18n.t('views.title.delete'),
      msg: I18n.t('views.msg.delete_jobunit', {
        name: record.data.name
      }),
      buttons: Ext.Msg.YESNO,
      icon: Ext.Msg.QUESTION,
      fn: function(btn) {
        if (btn != "yes")
          return;

        jhako_mask.show();
        record.destroy({
          failure: function(rec, response) {
            Ext.Msg.alert(I18n.t('views.msg.error'), I18n.t('views.msg.delete_jobunit_failed') + " '" + record.data.name + "'");
          },
          callback: function() {
            var ctrl = Jhako.app.getController('editor.Tree');
            ctrl.onLoadJobunit(parent_id);
          }
        });
      }
    });

  },

});