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

Ext.define('Jhako.controller.editor.List', {
  extend: 'Ext.app.Controller',

  refs: [{
    ref: 'editorTree',
    selector: 'editorTree'
  }, {
    ref: 'editorList',
    selector: 'editorList'
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
      'editorList': {
        edit: this.onEditJobunit
      },
      'editorList button[action=add_jobunit]': {
        click: this.onAddJobunit
      },
      'editorList button[action=copy_jobunit]': {
        click: this.onCopyJobunit
      },
      'editorList button[action=paste_jobunit]': {
        click: this.onPasteJobunit
      },
      'editorList button[action=delete_jobunit]': {
        click: this.onDeleteJobunit
      },
      'editorList actioncolumn': {
        click: this.onActionColumn
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
  onEditJobunit: function(editor, e) {
    var me = this;
    var grid_panel = me.getEditorList();
    var store = grid_panel.getStore();

    if (!jhako_selected_parent)
      return;

    store.sync({
      callback: function() {
        var ctrl = Jhako.app.getController('editor.Tree');
        ctrl.onLoadJobunit(jhako_selected_parent.data.id);
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
  onAddJobunit: function(button, e) {
    var me = this;

    var parent_id = 0;
    if (jhako_selected_parent) {
      parent_id = jhako_selected_parent.data.id;
    };

    var jobunit_kind = JOBUNIT_KIND_JOBGROUP;
    if (button.getItemId() == 'add_rootjobnet')
      jobunit_kind = JOBUNIT_KIND_ROOTJOBNET;

    // can not create jobnet int root jobgroup
    if (jobunit_kind >= JOBUNIT_KIND_ROOTJOBNET && parent_id == 0) {
      Ext.Msg.alert(I18n.t('views.msg.error'), I18n.t('views.msg.create_rootjobnet'));
      return;
    }

    var jobunit = new Jhako.model.Jobunit({
      parent_id: parent_id,
      kind: jobunit_kind
    });

    var grid = me.getEditorList();
    var store = grid.getStore();
    var cnt = store.getCount();
    store.insert(cnt, jobunit);

    var cellediting = grid.getPlugin('cellediting_jobunit');
    cellediting.startEdit(cnt, 1);
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
  onCopyJobunit: function(button, e) {
    jhako_selected_jobunits = [];

    var me = this;
    var grid = me.getEditorList();
    var record = grid.getSelectionModel().getLastSelected();
    if (!record)
      return;

    jhako_selected_jobunits.push(record);
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
  onPasteJobunit: function(button, e) {
    var me = this;
    var tree = me.getEditorTree();
    var store = tree.getStore();

    if (jhako_selected_jobunits.length == 0)
      return;
    if (!jhako_selected_parent)
      return;

    var parent_node = store.getNodeById(jhako_selected_parent.data.id);
    if (!parent_node)
      return;
    var parent_path = parent_node.getPath();

    var cnt = 0;
    jhako_selected_jobunits.forEach(function(record) {
      // can not paste jobnet into root jobgroup
      if (jhako_selected_parent.data.id == 0 && record.data.kind >= JOBUNIT_KIND_ROOTJOBNET) {
        cnt++;
        Ext.Msg.alert(I18n.t('views.msg.error'), I18n.t('views.msg.create_rootjobnet'));
        return;
      }

      // can not paste job
      if (record.data.kind >= JOBUNIT_KIND_STARTJOB) {
        cnt++;
        return;
      }

      // check node path
      var node = store.getNodeById(record.data.id);
      if (node) {
        if (parent_path.indexOf(node.getPath()) == 0) {
          cnt++;
          return;
        }
      }

      // create new child
      var child = new Jhako.model.JobunitChild(record.data);
      child.set('parent_id', jhako_selected_parent.data.id);
      child.set('x', 0);
      child.set('y', 0);
      if (record.data.kind >= JOBUNIT_KIND_ROOTJOBNET) {
        child.set('kind', JOBUNIT_KIND_ROOTJOBNET);
      }
      var name = JhakoMakeName(jhako_selected_parent['jhako.model.jobunitsStore'], record.data.name, 'copy');
      child.set('name', name);

      jhako_mask.show();
      child.save({
        callback: function() {
          cnt++;
          if (cnt < jhako_selected_jobunits.length)
            return;
          var ctrl = Jhako.app.getController('editor.Tree');
          ctrl.onLoadJobunit(jhako_selected_parent.data.id);
        }
      });
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
  onDeleteJobunit: function(button, e) {
    var me = this;
    var grid = me.getEditorList();
    var record = grid.getSelectionModel().getLastSelected();
    if (!record)
      return;

    var store = grid.getStore();
    var ctrl = Jhako.app.getController('editor.Tree');

    if (!jhako_selected_parent)
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
        store.remove(record);
        jhako_mask.show();
        store.sync({
          callback: function() {
            ctrl.onLoadJobunit(jhako_selected_parent.data.id);
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
  onActionColumn: function(view, rowIndex, colIndex, item, e, record) {
    if (!record.data.id)
      return;
    if (record.data.kind >= JOBUNIT_KIND_STARTJOB)
      return;
    var ctrl = Jhako.app.getController('editor.Tree');
    ctrl.onLoadJobunit(record.data.id);
  },
});
