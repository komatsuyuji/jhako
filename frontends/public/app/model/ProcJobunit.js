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
Ext.define('Jhako.model.ProcJobunit', {
  extend: 'Ext.data.Model',
  proxy: {
    type: 'rest',
    url: location.pathname + '/proc_jobunits',
    appendId: true,
    format: 'json',
    reader: {
      root: 'proc_jobunit',
    },
    writer: {
      root: 'proc_jobunit',
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
    name: 'parent_id',
    type: 'auto',
    defaultValue: 0
  }, {
    name: 'x',
    type: 'int',
    defaultValue: 0
  }, {
    name: 'y',
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
    name: 'boot',
    type: 'int',
    defaultValue: 0
  }, {
    name: 'finish',
    type: 'int',
    defaultValue: 0
  }, {
    name: 'pid',
    type: 'int',
    defaultValue: 0
  }, {
    name: 'path',
    type: 'string',
    defaultValue: ''
  }],

  associations: [{
    type: 'hasMany',
    model: 'Jhako.model.ProcJobunit',
    associationKey: 'children'
  }, {
    type: 'hasOne',
    model: 'Jhako.model.Rootjobnet',
    associationKey: 'rootjobnet'
  }, {
    type: 'hasMany',
    model: 'Jhako.model.Schedule',
    associationKey: 'schedules'
  }, {
    type: 'hasMany',
    model: 'Jhako.model.Connector',
    associationKey: 'connectors'
  }, {
    type: 'hasMany',
    model: 'Jhako.model.Variable',
    associationKey: 'variables'
  }, {
    type: 'hasOne',
    model: 'Jhako.model.Sleepjob',
    associationKey: 'sleepjob'
  }, {
    type: 'hasOne',
    model: 'Jhako.model.Clockjob',
    associationKey: 'clockjob'
  }, {
    type: 'hasOne',
    model: 'Jhako.model.Datejob',
    associationKey: 'datejob'
  }, {
    type: 'hasMany',
    model: 'Jhako.model.Datecond',
    associationKey: 'dateconds'
  }, {
    type: 'hasMany',
    model: 'Jhako.model.Vardatum',
    associationKey: 'vardata'
  }, {
    type: 'hasOne',
    model: 'Jhako.model.Sshjob',
    associationKey: 'sshjob'
  }, {
    type: 'hasOne',
    model: 'Jhako.model.Winjob',
    associationKey: 'winjob'
  }, {
    type: 'hasMany',
    model: 'Jhako.model.Condition',
    associationKey: 'conditions'
  }, {
    type: 'hasMany',
    model: 'Jhako.model.Jobresult',
    associationKey: 'jobresults'
  }, {
    type: 'hasMany',
    model: 'Jhako.model.Execlog',
    associationKey: 'execlogs'
  }],
});
