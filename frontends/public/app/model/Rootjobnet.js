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
Ext.define('Jhako.model.Rootjobnet', {
  extend: 'Ext.data.Model',
  proxy: {
    type: 'rest',
    appendId: true,
    format: 'json',
    reader: {
      root: 'rootjobnet',
    },
    writer: {
      root: 'rootjobnet',
    }
  },
  fields: [{
    name: 'id',
    type: 'auto'
  }, {
    name: 'jobunit_id',
    type: 'auto'
  }, {
    name: 'user_id',
    type: 'auto',
    defaultValue: 0
  }, {
    name: 'disabled',
    type: 'boolean',
    defaultValue: false
  }, {
    name: 'active_since',
    type: 'date'
  }, {
    name: 'active_till',
    type: 'date'
  }, {
    name: 'calendar_id',
    type: 'auto',
    defaultValue: 0
  }, {
    name: 'run_type',
    type: 'int',
    defaultValue: 0
  }, {
    name: 'delay_limit',
    type: 'int',
    defaultValue: 1
  }, {
    name: 'loader_interval',
    type: 'int',
    defaultValue: 5
  }]
});
