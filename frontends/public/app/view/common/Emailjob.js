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
Ext.define('Jhako.view.common.Emailjob', {
  alias: 'widget.commonEmailjob',
  extend: 'Ext.container.Container',
  title: 'email job',
  layout: 'border',
  autoScroll: true,
  itemId: 'comm_emailjob',
  items: [{
    xtype: 'commonJobunit',
    region: 'north'
  }, {
    xtype: 'form',
    region: 'north',
    itemId: 'emailjob_detail',
    bodyStyle: {
      backgroundColor: '#dfe8f6'
    },
    border: false,
    items: [{
      xtype: 'fieldset',
      title: I18n.t('views.title.advanced'),
      collapsible: true,
      collapsed: true,
      items: [{
        xtype: 'textfield',
        fieldLabel: I18n.t('views.emailjob.host'),
        name: 'host',
        allowBlank: false,
        readOnly: true,
      }, {
        xtype: 'textfield',
        fieldLabel: I18n.t('views.job.port'),
        name: 'port',
        allowBlank: false,
        maxLength: 4,
        regex: /^\d+$/,
        readOnly: true,
      }, {
        xtype: 'checkbox',
        fieldLabel: I18n.t('views.emailjob.auth'),
        name: 'auth',
        allowBlank: false,
        editable: false,
        readOnly: true,
      }, {
        xtype: 'textfield',
        fieldLabel: I18n.t('views.job.username'),
        name: 'username',
        readOnly: true,
      }, {
        xtype: 'textfield',
        inputType: 'password',
        fieldLabel: I18n.t('views.job.password'),
        name: 'password',
        readOnly: true,
      }]
    }, {
      xtype: 'textfield',
      fieldLabel: I18n.t('views.emailjob.mail_from'),
      name: 'mail_from',
      allowBlank: false,
      readOnly: true,
    }, {
      xtype: 'textfield',
      fieldLabel: I18n.t('views.emailjob.mail_to'),
      name: 'mail_to',
      allowBlank: false,
      readOnly: true,
    }, {
      xtype: 'textfield',
      fieldLabel: I18n.t('views.emailjob.subject'),
      name: 'subject',
      allowBlank: false,
      readOnly: true,
    }, {
      xtype: 'textareafield',
      width: 350,
      height: 50,
      fieldLabel: I18n.t('views.emailjob.body'),
      name: 'body',
      readOnly: true,
    }]
  }],

  initComponent: function() {
    this.callParent();
  }
});