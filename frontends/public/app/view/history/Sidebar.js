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
Ext.define('Jhako.view.history.Sidebar', {
  alias: 'widget.historySidebar',
  extend: 'Ext.panel.Panel',
  icon: location.pathname + '/images/icons/page.png',
  title: I18n.t('views.title.result'),
  frame: true,
  layout: 'accordion',
  collapsible: true,
  collapseDirection: 'right',
  split: true,

  items: [{
    xtype: 'commonJobresult',
  }, {
    xtype: 'commonVariable',
  }, {
    xtype: 'historyJobunitTab',
  }],

  initComponent: function() {
    this.callParent();
  }
});
