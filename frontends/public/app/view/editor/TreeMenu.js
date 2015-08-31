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
Ext.define('Jhako.view.editor.TreeMenu', {
  alias: 'widget.editorTreeMenu',
  extend: 'Ext.menu.Menu',
  items: [{
    text: 'Export',
    icon: location.pathname + '/images/icons/basket_remove.png',
    action: 'export_jobunit',
  }, {
    text: 'Import',
    icon: location.pathname + '/images/icons/basket_put.png',
    action: 'import_jobunit',
    /*
      }, {
        text: I18n.t('views.button.delete'),
        icon: location.pathname + '/images/icons/delete.png',
        action: 'delete_jobunit'
    */
  }],
  initComponent: function() {
    this.callParent();
  },
});