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
Ext.define('Jhako.view.common.Datejob', {
  alias: 'widget.commonDatejob',
  extend: 'Ext.container.Container',
  title: 'date job',
  layout: 'border',
  autoScroll: true,
  itemId: 'comm_datejob',
  items: [{
    xtype: 'commonJobunit',
    region: 'north',
  }, {
    xtype: 'form',
    region: 'north',
    itemId: 'datejob_detail',
    bodyStyle: {
      backgroundColor: '#dfe8f6'
    },
    border: false,
    items: [{
      xtype: 'combobox',
      fieldLabel: I18n.t('views.datejob.baseday'),
      name: 'baseday',
      allowBlank: false,
      readOnly: true,
      editable: false,
      store: [
        [0, I18n.t('views.datejob_baseday.job_startday')],
        [1, I18n.t('views.datejob_baseday.jobnet_startday')],
        [2, I18n.t('views.datejob_baseday.rootjobnet_startday')]
      ],
    }, {
      xtype: 'combobox',
      fieldLabel: I18n.t('views.datejob.beginning_of_week'),
      name: 'beginning_of_week',
      allowBlank: false,
      readOnly: true,
      editable: false,
      store: JHAKO_DAYOFWEEK_DATA,
    }],
  }, {
    xtype: 'commonDatecond',
    region: 'north',
    height: 300,
  }],

  initComponent: function() {
    this.callParent();
  }
});
