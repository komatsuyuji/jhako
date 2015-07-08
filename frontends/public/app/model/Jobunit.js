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
/*
Ext.define('Jhako.data.writer.Jobunit', {
  extend: 'Ext.data.writer.Json',
  root: 'jobunit',
  getRecordData: function(record) {
    var association, childStore;
    var data = this.callParent([record]);
    var getdata = record.getData(true);
    var kind = Number(record.data.kind);
    switch (kind) {
      case JOBUNIT_KIND_ROOTJOBNET:
        {
          if ('Jhako.model.Rootjobnet' in getdata) {
            data['rootjobnet'] = getdata['Jhako.model.Rootjobnet'];
          };
          break;
        }
      case JOBUNIT_KIND_SLEEPJOB:
        {
          if ('Jhako.model.Sleepjob' in getdata) {
            data['sleepjob'] = getdata['Jhako.model.Sleepjob'];
          };
          break;
        }
      case JOBUNIT_KIND_CLOCKJOB:
        {
          if ('Jhako.model.Clockjob' in getdata) {
            data['clockjob'] = getdata['Jhako.model.Clockjob'];
          };
          break;
        }
      case JOBUNIT_KIND_DATEJOB:
        {
          if ('Jhako.model.Datejob' in getdata) {
            data['datejob'] = getdata['Jhako.model.Datejob'];
          };
          if ('jhako.model.dateconds' in getdata) {
            data['dateconds'] = getdata['jhako.model.dateconds'];
          };
          break;
        }
      case JOBUNIT_KIND_SSHJOB:
        {
          if ('Jhako.model.Sshjob' in getdata) {
            data['sshjob'] = getdata['Jhako.model.Sshjob'];
          };
          if ('jhako.model.conditions' in getdata) {
            data['conditions'] = getdata['jhako.model.conditions'];
          };
          break;
        }
      case JOBUNIT_KIND_WINJOB:
        {
          if ('Jhako.model.Winjob' in getdata) {
            data['winjob'] = getdata['Jhako.model.Winjob'];
          };
          if ('jhako.model.conditions' in getdata) {
            data['conditions'] = getdata['jhako.model.conditions'];
          };
          break;
        }
      default:
        {}
    }
    return data;
  }
});
*/

Ext.define('Jhako.model.Jobunit', {
  extend: 'Ext.data.Model',
  proxy: {
    type: 'rest',
    url: location.pathname + '/jobunits',
    appendId: true,
    format: 'json',
    reader: {
      root: 'jobunit',
    },
    writer: {
      root: 'jobunit',
    },
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
  }],
  associations: [{
    type: 'hasMany',
    model: 'Jhako.model.Jobunit',
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
  }],
});
