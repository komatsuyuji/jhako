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
Ext.define('Jhako.view.process.Tree', {
  alias: 'widget.processTree',
  extend: 'Ext.tree.Panel',
  title: I18n.t('views.title.tree'),
  store: 'ProcJobunit',
  frame: true,
  collapsible: true,
  collapseDirection: 'left',
  split: true,
  //rootVisible: true,
  displayField: 'name',
  useArrows: false,
  viewConfig: {
    toggleOnDblClick: false,
  },
  icon: location.pathname + '/images/icons/application_side_tree.png',
  tbar: ['->', {
    icon: location.pathname + '/images/icons/arrow_refresh.png',
    plain: true,
    floating: false,
    menu: [{
      text: '1' + I18n.t('views.date.sec'),
      icon: location.pathname + '/images/icons/bullet_black.png',
      action: 'set_interval',
      interval: 1
    }, {
      text: '5' + I18n.t('views.date.sec'),
      icon: location.pathname + '/images/icons/bullet_black.png',
      action: 'set_interval',
      interval: 5
    }, {
      text: '15' + I18n.t('views.date.sec'),
      icon: location.pathname + '/images/icons/bullet_black.png',
      action: 'set_interval',
      interval: 15
    }, {
      text: '30' + I18n.t('views.date.sec'),
      icon: location.pathname + '/images/icons/bullet_black.png',
      action: 'set_interval',
      interval: 30
    }, {
      text: '1' + I18n.t('views.date.min'),
      icon: location.pathname + '/images/icons/bullet_black.png',
      action: 'set_interval',
      interval: 60
    }, {
      text: '5' + I18n.t('views.date.min'),
      icon: location.pathname + '/images/icons/bullet_black.png',
      action: 'set_interval',
      interval: 300
    }]
  }],
  initComponent: function() {
    this.callParent();
  },
});