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
Ext.define('Jhako.view.process.Sidebar', {
  alias: 'widget.processSidebar',
  extend: 'Ext.panel.Panel',
  icon: location.pathname + '/images/icons/page.png',
  title: I18n.t('views.title.result'),
  frame: true,
  layout: 'accordion',
  collapsible: true,
  //collapsed: true,
  collapseDirection: 'right',
  split: true,

  bbar: [{
    text: I18n.t('views.button.hold'),
    icon: location.pathname + '/images/icons/control_pause_blue.png',
    enableToggle: true,
    disabled: true,
    tooltip: 'Hold Job',
    action: 'hold_jobunit',
    itemId: 'btn_hold_proc_jobunit'
  }, '-', {
    text: I18n.t('views.button.skip'),
    icon: location.pathname + '/images/icons/control_fastforward_blue.png',
    enableToggle: true,
    disabled: true,
    tooltip: 'Skip Job',
    action: 'skip_jobunit',
    itemId: 'btn_skip_proc_jobunit'
  }, '-', {
    text: I18n.t('views.button.start'),
    icon: location.pathname + '/images/icons/control_play_blue.png',
    disabled: true,
    tooltip: 'Force Start',
    action: 'forcestart_jobunit',
    itemId: 'btn_forcestart_proc_jobunit'
  }, '-', {
    text: I18n.t('views.button.stop'),
    icon: location.pathname + '/images/icons/control_stop_blue.png',
    disabled: true,
    tooltip: 'Force Stop',
    action: 'forcestop_jobunit',
    itemId: 'btn_forcestop_proc_jobunit'
  }],

  items: [{
    xtype: 'commonJobresult',
  }, {
    xtype: 'commonExeclog',
  }, {
    xtype: 'commonVariable',
  }, {
    xtype: 'processJobunitTab',
  }],

  initComponent: function() {
    this.callParent();
  }
});
