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
Ext.define('Jhako.view.history.List', {
  alias: 'widget.historyList',
  extend: 'Ext.grid.Panel',
  title: I18n.t('views.title.list'),
  icon: location.pathname + '/images/icons/application_view_list.png',
  store: 'HistJobunit',
  frame: true,
  collapsible: true,
  collapseDirection: 'left',
  split: true,
  viewConfig: {
    loadMask: false,
    preserveScrollOnRefresh: true,
  },
  tbar: [{
    text: I18n.t('views.button.clear_filter'),
    icon: location.pathname + '/images/icons/hourglass.png',
    action: 'clear_filter',
  }, '->', {
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
  bbar: [{
    xtype: 'pagingtoolbar',
    store: 'HistJobunit',
    itemId: 'hist_pagingtoolbar',
    displayInfo: true,
  }],

  features: [Ext.create('Ext.ux.grid.FiltersFeature', {
    ftype: 'filters',
    autoReload: true,
    local: false,
    filters: [{
      type: 'numeric',
      dataIndex: 'id',
    }, {
      type: 'numeric',
      dataIndex: 'kind'
    }, {
      type: 'numeric',
      dataIndex: 'jobunit_id'
    }, {
      type: 'string',
      dataIndex: 'name'
    }, {
      type: 'list',
      dataIndex: 'mode',
      options: TOPJOBNET_MODE_DATA,
      phpMode: true,
    }, {
      type: 'list',
      dataIndex: 'status',
      options: JOBUNIT_STATUS_DATA,
      phpMode: true,
    }, {
      type: 'datetime',
      dataIndex: 'schedule_time',
      date: {
        format: 'Y-m-d',
      },
      time: {
        format: 'H:i',
      },
    }, {
      type: 'datetime',
      dataIndex: 'start_time',
      date: {
        format: 'Y-m-d',
      },
      time: {
        format: 'H:i',
      },
    }, {
      type: 'datetime',
      dataIndex: 'end_time',
      date: {
        format: 'Y-m-d',
      },
      time: {
        format: 'H:i',
      },
    }, {
      type: 'boolean',
      dataIndex: 'timeout_occur'
    }, {
      type: 'boolean',
      dataIndex: 'hold'
    }, {
      type: 'boolean',
      dataIndex: 'skip'
    }, {
      type: 'string',
      dataIndex: 'description'
    }, {
      type: 'string',
      dataIndex: 'path'
    }, {
      type: 'numeric',
      dataIndex: 'timeout'
    }, {
      type: 'datetime',
      dataIndex: 'created_at',
      date: {
        format: 'Y-m-d',
      },
      time: {
        format: 'H:i',
      },
    }, {
      type: 'datetime',
      dataIndex: 'updated_at',
      date: {
        format: 'Y-m-d',
      },
      time: {
        format: 'H:i',
      },
    }]
  })],
  columns: {
    items: [{
      text: 'ID',
      dataIndex: 'id',
      width: 40,
      hidden: true,
    }, {
      text: I18n.t('views.column.kind'),
      dataIndex: 'kind',
      hidden: true,
      width: 40,
    }, {
      text: 'Jobunit ID',
      dataIndex: 'jobunit_id',
      hidden: true,
      width: 40,
    }, {
      text: I18n.t('views.column.name'),
      dataIndex: 'name',
      width: 80,
    }, {
      text: I18n.t('views.topjobnet.mode'),
      dataIndex: 'mode',
      width: 100,
      renderer: function(value) {
        return JhakoFindRecord(TOPJOBNET_MODE_DATA, value);
      }
    }, {
      text: I18n.t('views.jobunit.status'),
      dataIndex: 'status',
      width: 60,
      renderer: function(value, meta) {
        meta.style = JhakoMetaStyle(value);
        return JhakoFindRecord(JOBUNIT_STATUS_DATA, value);
      }
    }, {
      text: I18n.t('views.topjobnet.schedule_time'),
      dataIndex: 'schedule_time',
      width: 100,
      xtype: 'datecolumn',
      format: 'Y-m-d H:i'
    }, {
      text: I18n.t('views.jobunit.start_time'),
      dataIndex: 'start_time',
      width: 120,
      xtype: 'datecolumn',
      format: 'Y-m-d H:i:s'
    }, {
      text: I18n.t('views.jobunit.end_time'),
      dataIndex: 'end_time',
      width: 120,
      xtype: 'datecolumn',
      format: 'Y-m-d H:i:s'
    }, {
      text: I18n.t('views.jobunit.timeout_occur'),
      dataIndex: 'timeout_occur',
      width: 60,
      renderer: function(value, meta) {
        if (value) {
          meta.style = "color:orange";
        }
        return value;
      }
    }, {
      text: I18n.t('views.jobunit.hold'),
      dataIndex: 'hold',
      width: 60,
      renderer: function(value, meta) {
        if (value) {
          meta.style = "color:blue";
        }
        return value;
      }
    }, {
      text: I18n.t('views.jobunit.skip'),
      dataIndex: 'skip',
      width: 60,
      renderer: function(value, meta) {
        if (value) {
          meta.style = "color:blue";
        }
        return value;
      }
    }, {
      text: I18n.t('views.column.description'),
      dataIndex: 'description',
      width: 120,
    }, {
      text: 'Path',
      dataIndex: 'path',
      width: 120,
    }, {
      text: I18n.t('views.jobunit.timeout'),
      dataIndex: 'timeout',
      width: 40,
    }, {
      text: I18n.t('views.column.created_at'),
      dataIndex: 'created_at',
      width: 120,
      xtype: 'datecolumn',
      format: 'Y-m-d H:i:s'
    }, {
      text: I18n.t('views.column.updated_at'),
      dataIndex: 'updated_at',
      width: 120,
      xtype: 'datecolumn',
      format: 'Y-m-d H:i:s'
    }]
  },
  initComponent: function() {
    this.callParent();
  }
});