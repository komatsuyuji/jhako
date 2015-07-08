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
Ext.define('Jhako.view.editor.DrawTab', {
  alias: 'widget.editorDrawTab',
  extend: 'Ext.tab.Panel',
  bodyBorder: false,
  disabled: true,
  tabPosition: 'bottom',
  height: 30, //60
  tabBar: {
    hidden: true
  },
  items: [{
    xtype: 'toolbar',
    title: 'Basic',
    border: false,
    items: [{
      xtype: 'button',
      icon: location.pathname + '/images/icons/chart_organisation.png',
      action: 'add_jobunit',
      tooltip: I18n.t('views.jobunit_kind.jobnet'),
      jobunitkind: JOBUNIT_KIND_JOBNET
    }, '', {
      xtype: 'button',
      icon: location.pathname + '/images/icons/arrow_merge.png',
      action: 'add_jobunit',
      tooltip: I18n.t('views.jobunit_kind.mergejob'),
      jobunitkind: JOBUNIT_KIND_MERGEJOB
    }, '', {
      xtype: 'button',
      icon: location.pathname + '/images/icons/time.png',
      action: 'add_jobunit',
      tooltip: I18n.t('views.jobunit_kind.sleepjob'),
      jobunitkind: JOBUNIT_KIND_SLEEPJOB
    }, '', {
      xtype: 'button',
      icon: location.pathname + '/images/icons/clock.png',
      action: 'add_jobunit',
      tooltip: I18n.t('views.jobunit_kind.clockjob'),
      jobunitkind: JOBUNIT_KIND_CLOCKJOB
    }, '', {
      xtype: 'button',
      icon: location.pathname + '/images/icons/date.png',
      action: 'add_jobunit',
      tooltip: I18n.t('views.jobunit_kind.datejob'),
      jobunitkind: JOBUNIT_KIND_DATEJOB
    }, '', {
      xtype: 'button',
      icon: location.pathname + '/images/icons/basket_put.png',
      action: 'add_jobunit',
      tooltip: I18n.t('views.jobunit_kind.varjob'),
      jobunitkind: JOBUNIT_KIND_VARJOB
    }, '', {
      xtype: 'button',
      icon: location.pathname + '/images/icons/server.png',
      action: 'add_jobunit',
      tooltip: I18n.t('views.jobunit_kind.sshjob'),
      jobunitkind: JOBUNIT_KIND_SSHJOB
    }, '', {
      xtype: 'button',
      icon: location.pathname + '/images/icons/computer.png',
      action: 'add_jobunit',
      tooltip: I18n.t('views.jobunit_kind.winjob'),
      jobunitkind: JOBUNIT_KIND_WINJOB
    }],
  }, {
    xtype: 'toolbar',
    title: 'extend',
    border: false,
    items: [],
  }],
  initComponent: function() {
    this.callParent();
  }
});
