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
Ext.define('Jhako.model.Winjob', {
  extend: 'Ext.data.Model',
  proxy: {
    type: 'rest',
    appendId: true,
    format: 'json',
    reader: {
      root: 'winjob',
    },
    writer: {
      root: 'winjob',
    }
  },
  fields: [{
    name: 'id',
    type: 'auto'
  }, {
    name: 'jobunit_id',
    type: 'auto',
  }, {
    name: 'proc_jobunit_id',
    type: 'auto'
  }, {
    name: 'hist_jobunit_id',
    type: 'auto'
  }, {
    name: 'kind',
    type: 'int'
  }, {
    name: 'host',
    type: 'string',
    defaultValue: ''
  }, {
    name: 'port',
    type: 'int'
  }, {
    name: 'username',
    type: 'string',
    defaultValue: ''
  }, {
    name: 'password',
    type: 'string',
    defaultValue: ''
  }, {
    name: 'scheme',
    type: 'string',
    defaultValue: ''
  }, {
    name: 'path',
    type: 'string',
    defaultValue: ''
  }, {
    name: 'auth',
    type: 'string',
    defaultValue: ''
  }, {
    name: 'codepage',
    type: 'int',
    defaultValue: 65001
  }, {
    name: 'command',
    type: 'string',
    defaultValue: ''
  }]
});