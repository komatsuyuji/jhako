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
Ext.define('Jhako.model.ProcTopjobnet', {
  extend: 'Ext.data.Model',
  proxy: {
    type: 'rest',
    url: location.pathname + '/proc_topjobnets',
    appendId: true,
    format: 'json',
    reader: {
      root: 'proc_topjobnet',
    },
    writer: {
      root: 'proc_topjobnet',
    }
  },

  fields: [{
    name: 'id',
    type: 'auto'
  }, {
    name: 'name',
    type: 'string',
    defaultValue: ''
  }, {
    name: 'description',
    type: 'string',
    defaultValue: ''
  }, {
    name: 'kind',
    type: 'int',
    defaultValue: 0
  }, {
    name: 'hold',
    type: 'boolean',
    defaultValue: false
  }, {
    name: 'skip',
    type: 'boolean',
    defaultValue: false
  }, {
    name: 'timeout',
    type: 'int',
    defaultValue: 0
  }, {
    name: 'created_at',
    type: 'date'
  }, {
    name: 'updated_at',
    type: 'date'
  }, {
    name: 'jobunit_id',
    type: 'auto',
    defaultValue: 0
  }, {
    name: 'status',
    type: 'int',
    defaultValue: 0
  }, {
    name: 'start_time',
    type: 'date'
  }, {
    name: 'end_time',
    type: 'date'
  }, {
    name: 'timeout_occur',
    type: 'boolean',
    defaultValue: false
  }, {
    name: 'force_start',
    type: 'boolean',
    defaultValue: false
  }, {
    name: 'force_stop',
    type: 'boolean',
    defaultValue: false
  }, {
    name: 'proc_jobunit_id',
    type: 'auto',
    defaultValue: 0
  }, {
    name: 'proc_alarm_id',
    type: 'auto',
    defaultValue: 0
  }, {
    name: 'mode',
    type: 'int',
    defaultValue: 0
  }, {
    name: 'schedule_time',
    type: 'date'
  }, {
    name: 'run_type',
    type: 'int',
    defaultValue: 0
  }, {
    name: 'delay_limit',
    type: 'int',
    defaultValue: 1
  }, {
    name: 'path',
    type: 'string',
    defaultValue: ''
  }],
});