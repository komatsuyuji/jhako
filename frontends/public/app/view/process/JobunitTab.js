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
Ext.define('Jhako.view.process.JobunitTab', {
  alias: 'widget.processJobunitTab',
  extend: 'Ext.tab.Panel',
  title: I18n.t('views.title.setting'),
  icon: location.pathname + '/images/icons/cog.png',
  //frame: true,
  tabBar: {
    hidden: true
  },
  items: [{
    xtype: 'commonJobnet',
    listeners: {
      afterrender: function(panel) {
        panel.getDockedItems('toolbar[dock="bottom"]')[0].setVisible(false);
      }
    }
  }, {
    xtype: 'commonSubjobnet',
  }, {
    xtype: 'commonStartjob'
  }, {
    xtype: 'commonMergejob'
  }, {
    xtype: 'commonSleepjob'
  }, {
    xtype: 'commonClockjob',
  }, {
    xtype: 'commonDatejob',
  }, {
    xtype: 'commonVarjob',
  }, {
    xtype: 'commonSshjob'
  }, {
    xtype: 'commonWinjob',
  }],
  initComponent: function() {
    this.callParent();
  }
});
