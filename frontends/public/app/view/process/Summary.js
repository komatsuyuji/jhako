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
Ext.define('Jhako.view.process.Summary', {
  alias: 'widget.processSummary',
  extend: 'Ext.grid.Panel',
  title: I18n.t('views.title.summary'),
  border: false,
  frame: true,
  store: 'ProcTopjobnetSummary',
  viewConfig: {
    loadMask: false,
    preserveScrollOnRefresh: true,
  },
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
    }],
  }],
  columns: {
    items: [{
      text: I18n.t('views.jobunit.status'),
      dataIndex: 'status',
      width: 200,
      renderer: function(value, meta) {
        meta.style = JhakoMetaStyle(value);
        return JhakoFindRecord(JOBUNIT_STATUS_DATA, value);
      }
    }, {
      text: I18n.t('views.jobunit.hold'),
      dataIndex: 'hold',
      width: 100,
      renderer: function(value, meta, record) {
        meta.style = JhakoMetaStyle(record.data.status);
        return value;
      }
    }, {
      text: I18n.t('views.jobunit.skip'),
      dataIndex: 'skip',
      width: 100,
      renderer: function(value, meta, record) {
        meta.style = JhakoMetaStyle(record.data.status);
        return value;
      }
    }, {
      text: I18n.t('views.jobunit.timeout_occur'),
      dataIndex: 'timeout_occur',
      width: 100,
      renderer: function(value, meta, record) {
        meta.style = JhakoMetaStyle(record.data.status);
        return value;
      }
    }, {
      text: I18n.t('views.jobunit.force_start'),
      dataIndex: 'force_start',
      width: 100,
      renderer: function(value, meta, record) {
        meta.style = JhakoMetaStyle(record.data.status);
        return value;
      }
    }, {
      text: I18n.t('views.jobunit.force_stop'),
      dataIndex: 'force_stop',
      width: 100,
      renderer: function(value, meta, record) {
        meta.style = JhakoMetaStyle(record.data.status);
        return value;
      }
    }, {
      text: 'Total',
      dataIndex: 'total',
      width: 100,
      renderer: function(value, meta, record) {
        meta.style = JhakoMetaStyle(record.data.status);
        meta.style += '; text-decoration: underline';
        meta.style += '; cursor: pointer';
        return value;
      }
    }]
  },

  initComponent: function() {
    this.callParent();
  }
});
