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

Ext.define('Jhako.controller.editor.DrawTab', {
  extend: 'Ext.app.Controller',

  refs: [{
    ref: 'editorDraw',
    selector: 'editorDraw'
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
      'editorDrawTab button[action=add_jobunit]': {
        afterrender: this.onAfterRender,
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
  onAfterRender: function(button) {
    var me = this;
    button.addManagedListener(button.getEl(), 'mousedown', me.onMouseDown, me, button);
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
  onMouseDown: function(e, t, btn) {
    var me = this;
    var editordraw = me.getEditorDraw();
    var img = editordraw.child('#jobunit_image');
    if (!img)
      return;

    var pos_x = e.getPageX() - editordraw.getX() - JHAKO_DRAW_WIDTH;
    var pos_y = e.getPageY() - editordraw.getY() - JHAKO_DRAW_WIDTH;
    img.setSrc(btn.icon);
    img.setPosition(pos_x, pos_y);
    img.setVisible(true);
    img.jobunitkind = btn.jobunitkind;
  },
});
