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
Ext.define('Jhako.view.common.Jobresult', {
  alias: 'widget.commonJobresult',
  extend: 'Ext.panel.Panel',
  title: I18n.t('views.title.jobresult'),
  icon: location.pathname + '/images/icons/report.png',
  frame: true,
  autoScroll: true,
  itemId: 'comm_jobresult',
  defaults: {
    xtype: 'form',
    region: 'north',
    bodyStyle: {
      backgroundColor: '#dfe8f6'
    },
    border: false,
  },
  items: [{
    itemId: 'jobresult_jobunit',
    defaults: {
      xtype: 'displayfield',
    },
    items: [{
      fieldLabel: I18n.t('views.column.name'),
      name: 'name',
    }, {
      fieldLabel: I18n.t('views.jobunit.status'),
      name: 'status',
    }, {
      fieldLabel: I18n.t('views.jobunit.timeout_occur'),
      name: 'timeout_occur',
    }, {
      fieldLabel: I18n.t('views.jobunit.force_start'),
      name: 'force_start',
    }, {
      fieldLabel: I18n.t('views.jobunit.force_stop'),
      name: 'force_stop',
    }, {
      fieldLabel: I18n.t('views.jobunit.start_time'),
      name: 'start_time',
      dateFormat: 'Y-m-d H:i:s',
    }, {
      fieldLabel: I18n.t('views.jobunit.end_time'),
      name: 'end_time',
      dateFormat: 'Y-m-d H:i:s',
    }],
  }, {
    itemId: 'jobresult_detail',
    defaults: {
      xtype: 'displayfield',
    },
    items: [{
      fieldLabel: I18n.t('views.jobresult.exitcode'),
      name: 'exitcode'
    }, {
      fieldLabel: I18n.t('views.jobresult.exitsignal'),
      name: 'exitsignal'
    }, {
      xtype: 'textareafield',
      width: 350,
      height: 100,
      fieldLabel: I18n.t('views.jobresult.stdout'),
      name: 'stdout',
      readOnly: true,
    }, {
      xtype: 'textareafield',
      width: 350,
      height: 100,
      fieldLabel: I18n.t('views.jobresult.stderr'),
      name: 'stderr',
      readOnly: true,
    }],
  }],

  initComponent: function() {
    this.callParent();
  }
});