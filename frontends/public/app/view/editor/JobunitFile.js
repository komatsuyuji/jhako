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
Ext.define('Jhako.view.editor.JobunitFile', {
  alias: 'widget.editorJobunitFile',
  extend: 'Ext.Window',
  title: 'Import',
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
    xtype: 'form',
    itemId: 'form_import',
    width: 400,
    bodyPadding: 10,
    frame: true,
    items: [{
      xtype: 'filefield',
      name: 'file',
      fieldLabel: 'File',
      itemId: 'ff_file',
      labelWidth: 50,
      msgTarget: 'side',
      allowBlank: false,
      anchor: '100%',
      buttonText: 'Select ...'
    }],
  }],
  buttons: [{
    text: 'Import',
    action: 'import_jobunit'
  }, '-', {
    text: 'Cancel',
    icon: location.pathname + '/images/icons/cancel.png',
    action: 'cancel_jobunit'
  }],
  initComponent: function() {
    this.callParent();
  }
});