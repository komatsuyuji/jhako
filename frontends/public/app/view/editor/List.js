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
Ext.define('Jhako.view.editor.List', {
  alias: 'widget.editorList',
  extend: 'Ext.grid.Panel',
  title: 'Job List',
  frame: true,
  itemId: 'editor_list',
  plugins: [
    Ext.create('Ext.grid.plugin.CellEditing', {
      pluginId: 'cellediting_jobunit',
      clicksToEdit: 1
    })
  ],
  bbar: [{
    text: I18n.t('views.button.add'),
    icon: location.pathname + '/images/icons/folder_add.png',
    itemId: 'add_jobgroup',
    tooltip: I18n.t('views.jobunit_kind.jobgroup'),
    action: 'add_jobunit'
  }, '-', {
    text: I18n.t('views.button.add'),
    icon: location.pathname + '/images/icons/chart_organisation_add.png',
    itemId: 'add_rootjobnet',
    tooltip: I18n.t('views.jobunit_kind.rootjobnet'),
    action: 'add_jobunit'
  }, '-', {
    text: I18n.t('views.button.copy'),
    icon: location.pathname + '/images/icons/page_copy.png',
    itemId: 'copy_rootjobnet',
    tooltip: I18n.t('views.jobunit_kind.rootjobnet'),
    action: 'copy_jobunit'
  }, '-', {
    text: I18n.t('views.button.paste'),
    icon: location.pathname + '/images/icons/page_paste.png',
    itemId: 'paste_rootjobnet',
    tooltip: I18n.t('views.jobunit_kind.rootjobnet'),
    action: 'paste_jobunit'
  }, '->', {
    text: I18n.t('views.button.delete'),
    icon: location.pathname + '/images/icons/delete.png',
    action: 'delete_jobunit'
  }],
  columns: [{
    xtype: 'actioncolumn',
    width: 30,
    items: [{
      icon: location.pathname + '/ext/resources/themes/images/default/tree/folder.gif',
      getClass: function(v, metadata, record, rowIndex, colIndex, store) {
        if (record.data.kind < JOBUNIT_KIND_ROOTJOBNET) return;
        return "x-hide-display";
      }
    }, {
      icon: location.pathname + '/images/icons/chart_organisation.png',
      getClass: function(v, metadata, record, rowIndex, colIndex, store) {
        if (record.data.kind >= JOBUNIT_KIND_ROOTJOBNET && record.data.kind < JOBUNIT_KIND_STARTJOB) return;
        return "x-hide-display";
      }
    }, {
      icon: location.pathname + '/images/icons/server.png',
      getClass: function(v, metadata, record, rowIndex, colIndex, store) {
        if (record.data.kind >= JOBUNIT_KIND_STARTJOB) return;
        return "x-hide-display";
      }
    }]
  }, {
    text: 'ID',
    dataIndex: 'id',
    width: 40,
    hidden: true,
  }, {
    text: I18n.t('views.column.name'),
    dataIndex: 'name',
    width: 150,
    editor: {
      xtype: 'textfield',
      allowBlank: false,
      minLength: 3,
      maxLength: 255,
      regex: /^[a-z]\w+$/i
    }
  }, {
    text: I18n.t('views.column.description'),
    dataIndex: 'description',
    width: 400,
    editor: {
      xtype: 'textfield'
    }
  }, {
    text: I18n.t('views.column.created_at'),
    dataIndex: 'created_at',
    width: 120,
    xtype: 'datecolumn',
    format: 'Y-m-d H:i:s'
  }, {
    text: I18n.t('views.column.updated_at'),
    dataIndex: 'updated_at',
    width: 120,
    xtype: 'datecolumn',
    format: 'Y-m-d H:i:s'
  }],
  initComponent: function() {
    this.callParent();
  }
});