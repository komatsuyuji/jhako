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
Ext.define('Jhako.view.editor.Draw', {
  alias: 'widget.editorDraw',
  extend: 'Ext.container.Container',
  layout: 'absolute',
  items: [{
    xtype: 'container',
    layout: 'border',
    anchor: '100% 100%',
    items: [{
      xtype: 'editorDrawTab',
      region: 'north'
    }, {
      xtype: 'container',
      region: 'center',
      autoScroll: true,
      itemId: 'cont_flowchart',
      items: {
        xtype: 'editorFlowchart',
      },
    }]
  }, {
    xtype: 'image',
    itemId: 'jobunit_image',
    width: 2 * JHAKO_DRAW_WIDTH,
    height: 2 * JHAKO_DRAW_WIDTH,
    hidden: true,
  }],
  initComponent: function() {
    this.callParent();
  },
});