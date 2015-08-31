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
Ext.define('Jhako.store.Jobunit', {
  extend: 'Ext.data.TreeStore',
  model: 'Jhako.model.Jobunit',
  autoSync: false,
  sorters: [{
    property: 'kind',
    direction: 'ASC'
  }, {
    property: 'name',
    direction: 'ASC'
  }],
  proxy: {
    type: 'rest',
    url: location.pathname + '/jobunits',
    appendId: false,
    format: 'json',
    pageParam: null,
    startParam: null,
    limitParam: null,
    reader: {
      root: 'jobunits',
    },
  },

  listeners: {
    append: function(store, node, index) {
      if (node.isRoot())
        return;
      node.data.qtip = node.data.description;
      if (node.data.kind < JOBUNIT_KIND_ROOTJOBNET) {} else
      if (node.data.kind < JOBUNIT_KIND_STARTJOB) {
        node.data.icon = location.pathname + '/images/icons/chart_organisation.png';
      } else {
        node.data.icon = location.pathname + '/images/icons/package.png';
        node.data.leaf = true;
        node.data.cls = 'x-hide-display';
      }
    }
  }

});