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
Ext.define('Jhako.model.User', {
  extend: 'Ext.data.Model',
  proxy: {
    type: 'rest',
    url: location.pathname + '/users',
    appendId: true,
    format: 'json',
    reader: {
      root: 'user',
    },
    writer: {
      root: 'user',
    }
  },
  fields: [{
    name: 'id',
    type: 'auto'
  }, {
    name: 'name',
    type: 'string'
  }, {
    name: 'fullname',
    type: 'string',
    defaultValue: ''
  }, {
    name: 'kind',
    type: 'int',
    defaultValue: 0
  }, {
    name: 'password',
    type: 'string'
  }, {
    name: 'password_confirmation',
    type: 'string'
  }, {
    name: 'language',
    type: 'string',
    defaultValue: 'en'
  }, {
    name: 'email',
    type: 'string',
    defaultValue: ''
  }, {
    name: 'created_at',
    type: 'date'
  }, {
    name: 'updated_at',
    type: 'date'
  }]
});
