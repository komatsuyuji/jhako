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
Ext.define('Jhako.view.history.Index', {
  alias: 'widget.historyIndex',
  extend: 'Ext.container.Container',
  title: I18n.t('views.title.history'),
  itemId: 'history_index',
  border: false,
  layout: 'border',
  items: [{
    xtype: 'historyList',
    region: 'west',
    width: 400
  }, {
    xtype: 'historyDetail',
    region: 'center',
  }, {
    xtype: 'historySidebar',
    region: 'east',
    width: 400
  }],
  initComponent: function() {
    this.callParent();
  }
});
