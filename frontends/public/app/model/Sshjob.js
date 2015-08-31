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
Ext.define('Jhako.model.Sshjob', {
  extend: 'Ext.data.Model',
  proxy: {
    type: 'rest',
    appendId: true,
    format: 'json',
    reader: {
      root: 'sshjob',
    },
    writer: {
      root: 'sshjob',
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
    name: 'host',
    type: 'string',
    defaultValue: ''
  }, {
    name: 'port',
    type: 'int'
  }, {
    name: 'authtype',
    type: 'int',
    defaultValue: 0
  }, {
    name: 'username',
    type: 'string',
    defaultValue: ''
  }, {
    name: 'password',
    type: 'string',
    defaultValue: ''
  }, {
    name: 'privatekey',
    type: 'string',
    defaultValue: ''
  }, {
    name: 'command',
    type: 'string',
    defaultValue: ''
  }]
});