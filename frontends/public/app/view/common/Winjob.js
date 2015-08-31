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
Ext.define('Jhako.view.common.Winjob', {
  alias: 'widget.commonWinjob',
  extend: 'Ext.container.Container',
  title: 'windows job',
  layout: 'border',
  autoScroll: true,
  itemId: 'comm_winjob',
  items: [{
    xtype: 'commonJobunit',
    region: 'north'
  }, {
    xtype: 'form',
    region: 'north',
    itemId: 'winjob_detail',
    bodyStyle: {
      backgroundColor: '#dfe8f6'
    },
    border: false,
    items: [{
      xtype: 'textfield',
      fieldLabel: I18n.t('views.job.host') + '(IP)',
      name: 'host',
      allowBlank: false,
      readOnly: true,
    }, {
      xtype: 'textfield',
      fieldLabel: I18n.t('views.job.username'),
      name: 'username',
      allowBlank: false,
      readOnly: true,
    }, {
      xtype: 'textfield',
      inputType: 'password',
      fieldLabel: I18n.t('views.job.password'),
      name: 'password',
      allowBlank: false,
      readOnly: true,
    }, {
      xtype: 'combobox',
      fieldLabel: I18n.t('views.job.codepage'),
      name: 'codepage',
      editable: false,
      store: [
        [437, 'IBM437'],
        [932, 'SHIFT-JIS'],
        [65001, 'UTF-8']
      ]
    }, {
      xtype: 'textareafield',
      width: 350,
      height: 50,
      fieldLabel: I18n.t('views.job.command'),
      name: 'command',
      readOnly: true,
    }, {
      xtype: 'fieldset',
      title: I18n.t('views.title.advanced'),
      collapsible: true,
      collapsed: true,
      items: [{
        xtype: 'combobox',
        fieldLabel: I18n.t('views.winjob.scheme'),
        name: 'scheme',
        allowBlank: false,
        editable: false,
        readOnly: true,
        store: ['http'],
      }, {
        xtype: 'textfield',
        fieldLabel: I18n.t('views.job.port'),
        name: 'port',
        allowBlank: false,
        maxLength: 4,
        regex: /^\d+$/,
        readOnly: true,
      }, {
        xtype: 'textfield',
        fieldLabel: I18n.t('views.winjob.path'),
        name: 'path',
        allowBlank: false,
        readOnly: true,
      }, {
        xtype: 'combobox',
        fieldLabel: I18n.t('views.winjob.auth'),
        name: 'auth',
        allowBlank: false,
        editable: false,
        readOnly: true,
        store: ['basic'],
      }]
    }]
  }, {
    xtype: 'commonCondition',
    region: 'north',
    title: I18n.t('views.title.error_condition'),
    itemId: 'winjob_error',
    height: 150,
  }, {
    xtype: 'commonCondition',
    region: 'north',
    title: I18n.t('views.title.branch_condition'),
    itemId: 'winjob_branch',
    height: 150,
  }],

  initComponent: function() {
    this.callParent();
  }
});