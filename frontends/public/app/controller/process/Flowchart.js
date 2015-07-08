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

Ext.define('Jhako.controller.process.Flowchart', {
  extend: 'Ext.app.Controller',

  refs: [{
    ref: 'processFlowchart',
    selector: 'processFlowchart'
  }, {
    ref: 'processJobunitTab',
    selector: 'processJobunitTab'
  }, {
    ref: 'processTree',
    selector: 'processTree'
  }],

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
  init: function() {
    this.control({
      'processFlowchart': {
        afterrender: this.onAfterRender,
        mousedown: this.onMousedown,
        dblclick: this.onDblclick,
      }
    });
  },

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
  onReset: function() {
    var me = this;
    var flowchart = me.getProcessFlowchart();
    var surface = flowchart.surface;

    if (!surface)
      return;
    surface.removeAll();
    surface.selectedjob = null;
  },

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
  onAfterRender: function(flowchart) {
    var surface = flowchart.surface;
    surface.selectedjob = null;
  },

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
  onMousedown: function(e) {
    var me = this;
    var flowchart = me.getProcessFlowchart();
    var surface = flowchart.surface;
    surface.getGroup('flowchart_choice').hide(true);
    surface.selectedjob = null;
    var tab = me.getProcessJobunitTab();
    tab.setActiveTab('comm_jobnet');
  },

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
  onDblclick: function(e) {
    var me = this;
    var flowchart = me.getProcessFlowchart();
    var surface = flowchart.surface;
    if (!surface.selectedjob)
      return;
    var record = surface.selectedjob;
    if (!(record.data.kind > JOBUNIT_KIND_ROOTJOBNET && record.data.kind < JOBUNIT_KIND_STARTJOB))
      return;

    jhako_topjobnet_id = record.data.id;
    jhako_interval_count = 0;
    me.onReset();

    var tree = me.getProcessTree();
    if (tree) {
      var store = tree.getStore();
      var node = store.getNodeById(record.data.id);
      tree.selectPath(node.getPath());
      tree.expandPath(node.getPath());
    }
  },

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
  onDrawFlowchart: function(record) {
    if (!record)
      return;

    var me = this;
    var flowchart = me.getProcessFlowchart();
    var surface = flowchart.surface;
    if (surface) surface.removeAll();
    else return;

    surface.add(new Jhako.draw.Choice()).hide(true);
    if ('jhako.model.procjobunitsStore' in record) {
      var store = record['jhako.model.procjobunitsStore'];
      store.getRange().forEach(function(job) {
        me.onDrawJob(job);
      });

      record['jhako.model.connectorsStore'].each(function(conn) {
        prev_job = store.getById(conn.data.prev_jobid)
        next_job = store.getById(conn.data.next_jobid);
        if (!prev_job || !next_job) return;
        me.onDrawConnector(conn, prev_job, next_job);
      });
    };

    if (surface.selectedjob) {
      var group = surface.getGroup('spritejob_' + surface.selectedjob.data.id);
      if (!group) return;
      var job = group.getAt(0);
      if (!job) return;
      me.onMousedownJob(job);
    } else {
      var ctrl = Jhako.app.getController('process.JobunitTab');
      ctrl.onLoadJobunit(record);
    }
  },

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
  onDrawConnector: function(record, prev_job, next_job) {
    var me = this;
    var flowchart = me.getProcessFlowchart();
    var surface = flowchart.surface;
    var connector = new Jhako.draw.ConnectorView(record, prev_job, next_job);
    surface.add(connector);
    var group = surface.getGroup('spriteconn_' + record.data.id);
    group.show(true);
    return group;
  },

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
  onDrawJob: function(record) {
    var me = this;
    var flowchart = me.getProcessFlowchart();
    var surface = flowchart.surface;
    surface.add(new Jhako.draw.JobView(record));

    var group = surface.getGroup('spritegroup_' + record.data.id);
    group.on({
      'mousedown': me.onMousedownJob,
    });
    group.show(true);
    return group;
  },

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
  onRedrawJob: function(record) {
    if (!record)
      return;

    var me = this;
    var flowchart = me.getProcessFlowchart();
    var surface = flowchart.surface;

    var hold = surface.getGroup('spritehold_' + record.data.id);
    if (hold) {
      hold.setAttributes({
        opacity: (record.data.hold) ? 1 : 0,
      }, true);
    };

    var skip = surface.getGroup('spriteskip_' + record.data.id);
    if (skip) {
      skip.setAttributes({
        opacity: (record.data.skip) ? 1 : 0,
      }, true);
    };
  },

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
  onMousedownJob: function(job, t) {
    var surface = job.surface;
    var record = job.record;
    var group = surface.getGroup('spritegroup_' + record.data.id);
    if (!group)
      return;

    var bbox = group.getBBox();
    var choice = surface.getGroup('flowchart_choice');
    choice.setAttributes({
      width: bbox.width + 2,
      height: bbox.height + 2,
      x: bbox.x - 1,
      y: bbox.y - 1
    });
    choice.show(true);

    surface.selectedjob = record;
    // load job record
    var ctrl = Jhako.app.getController('process.JobunitTab');
    ctrl.onLoadJobunit(record);
  },

});
