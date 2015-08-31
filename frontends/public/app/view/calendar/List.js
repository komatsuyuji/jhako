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
Ext.define('Jhako.view.calendar.List', {
  alias: 'widget.calendarList',
  extend: 'Ext.grid.Panel',
  title: I18n.t('views.title.list'),
  icon: location.pathname + '/images/icons/calendar.png',
  store: 'Calendar',
  frame: true,
  collapsible: true,
  collapseDirection: 'left',
  split: true,

  features: [
    Ext.create('Ext.ux.grid.FiltersFeature', {
      ftype: 'filters',
      autoReload: true,
      local: false,
      filters: [{
        type: 'string',
        dataIndex: 'name',
      }, {
        type: 'string',
        dataIndex: 'description'
      }, {
        type: 'numeric',
        dataIndex: 'kind'
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
    })
  ],

  columns: {
    items: [{
      text: 'ID',
      dataIndex: 'id',
      width: 40,
      hidden: true,
    }, {
      text: I18n.t('views.column.name'),
      dataIndex: 'name',
      allowBlank: false,
      width: 100,
    }, {
      text: I18n.t('views.column.description'),
      dataIndex: 'description',
      width: 200,
    }, {
      text: I18n.t('views.column.kind'),
      dataIndex: 'kind',
      width: 40,
      hidden: true,
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