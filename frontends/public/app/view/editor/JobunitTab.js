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
Ext.define('Jhako.view.editor.JobunitTab', {
  alias: 'widget.editorJobunitTab',
  extend: 'Ext.tab.Panel',
  title: I18n.t('views.title.setting'),
  icon: location.pathname + '/images/icons/cog.png',
  frame: true,
  collapsible: true,
  collapseDirection: 'right',
  split: true,
  tabBar: {
    hidden: true
  },
  bbar: ['->', {
    text: I18n.t('views.button.copy'),
    disabled: false,
    icon: location.pathname + '/images/icons/page_copy.png',
    action: 'copy_jobunit',
    itemId: 'btn_copy_jobunit'
  }, '-', {
    text: I18n.t('views.button.paste'),
    disabled: true,
    icon: location.pathname + '/images/icons/page_paste.png',
    action: 'paste_jobunit'
  }, '-', {
    text: I18n.t('views.button.delete'),
    disabled: true,
    icon: location.pathname + '/images/icons/delete.png',
    action: 'delete_jobunit'
  }, '-', {
    text: I18n.t('views.button.save'),
    disabled: true,
    icon: location.pathname + '/images/icons/disk.png',
    action: 'save_jobunit'
  }, '-', {
    text: I18n.t('views.button.cancel'),
    disabled: true,
    icon: location.pathname + '/images/icons/cancel.png',
    action: 'cancel_jobunit'
  }],

  items: [{
    xtype: 'commonJobnet',
    listeners: {
      afterrender: function(panel) {
        var form = panel.child('#comm_jobunit');
        form.child('#jobunit_name').setVisible(false);
        form.child('#jobunit_description').setVisible(false);
      }
    }
  }, {
    xtype: 'commonSubjobnet'
  }, {
    xtype: 'commonStartjob'
  }, {
    xtype: 'commonMergejob'
  }, {
    xtype: 'commonSleepjob'
  }, {
    xtype: 'commonClockjob',
    listeners: {
      afterrender: function(panel) {
        var form = panel.child('#clockjob_detail');
        form.child('#clockjob_predetermined_time').setVisible(false);
      }
    }
  }, {
    xtype: 'commonDatejob'
  }, {
    xtype: 'commonVarjob'
  }, {
    xtype: 'commonSshjob'
  }, {
    xtype: 'commonWinjob',
  }, {
    xtype: 'commonEmailjob',
  }],

  initComponent: function() {
    this.callParent();
  }
});