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


Ext.define('Jhako.controller.process.Sidebar', {
  extend: 'Ext.app.Controller',
  refs: [{
    ref: 'processFlowchart',
    selector: 'processFlowchart'
  }, {
    ref: 'processShow',
    selector: 'processShow'
  }, {
    ref: 'processSidebar',
    selector: 'processSidebar'
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
      'processSidebar': {
        afterrender: this.onAfterrender,
      },
      'processSidebar button[action=hold_jobunit]': {
        click: this.onHoldJobunit
      },
      'processSidebar button[action=skip_jobunit]': {
        click: this.onSkipJobunit
      },
      'processSidebar button[action=forcestart_jobunit]': {
        click: this.onForcestartJobunit
      },
      'processSidebar button[action=forcestop_jobunit]': {
        click: this.onForcestopJobunit
      },
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
  onAfterrender: function(panel) {
    if (!jhako_login_user)
      return;

    if (jhako_login_user.data.kind == USER_KIND_MONITOR) {
      var items = panel.getDockedItems('toolbar[dock="bottom"]');
      items.forEach(function(item) {
        item.setVisible(false);
      });
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
  onHoldJobunit: function(button, e) {
    var me = this;
    var flowchart = me.getProcessFlowchart();
    var surface = flowchart.surface;
    if (!surface)
      return;

    var record = surface.selectedjob;
    if (!record) {
      var form = me.getProcessShow();
      record = form.getRecord();
    }
    if (!record)
      return;
    record.data.hold = (record.data.hold) ? false : true;

    button.setDisabled(true);
    record.save({
      callback: function() {
        button.setDisabled(false);
      }
    });

    var ctrl = Jhako.app.getController('process.Flowchart');
    ctrl.onRedrawJob(record);
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
  onSkipJobunit: function(button, e) {
    var me = this;
    var flowchart = me.getProcessFlowchart();
    var surface = flowchart.surface;
    if (!surface)
      return;
    var record = surface.selectedjob;
    if (!record) {
      var form = me.getProcessShow();
      record = form.getRecord();
    }
    if (!record)
      return;
    record.data.skip = (record.data.skip) ? false : true;

    button.setDisabled(true);
    record.save({
      callback: function() {
        button.setDisabled(false);
      }
    });

    var ctrl = Jhako.app.getController('process.Flowchart');
    ctrl.onRedrawJob(record);
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
  onForcestartJobunit: function(button, e) {
    var me = this;
    var flowchart = me.getProcessFlowchart();
    var surface = flowchart.surface;
    if (!surface)
      return;
    var record = surface.selectedjob;
    if (!record) {
      var form = me.getProcessShow();
      record = form.getRecord();
    }
    if (!record)
      return;
    record.data.force_start = (record.data.force_start) ? false : true;

    button.setDisabled(true);
    record.save();
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
  onForcestopJobunit: function(button, e) {
    var me = this;
    var flowchart = me.getProcessFlowchart();
    var surface = flowchart.surface;
    if (!surface)
      return;
    var record = surface.selectedjob;
    if (!record) {
      var form = me.getProcessShow();
      record = form.getRecord();
    }
    if (!record)
      return;
    record.data.force_stop = (record.data.force_stop) ? false : true;

    button.setDisabled(true);
    record.save();
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
  onResetButton: function(record) {
    var me = this;
    var panel = me.getProcessSidebar();
    var btn_hold = panel.down("button[action='hold_jobunit']");
    var btn_skip = panel.down("button[action='skip_jobunit']");
    var btn_start = panel.down("button[action='forcestart_jobunit']");
    var btn_stop = panel.down("button[action='forcestop_jobunit']");
    btn_hold.setDisabled(true);
    btn_skip.setDisabled(true);
    btn_start.setDisabled(true);
    btn_stop.setDisabled(true);
    if (!record)
      return;

    switch (record.data.status) {
      case JOBUNIT_STATUS_NONE:
      case JOBUNIT_STATUS_STANDBY:
      case JOBUNIT_STATUS_READY:
        {
          btn_hold.setDisabled(false);
          btn_hold.toggle(record.data.hold);
          btn_skip.setDisabled(false);
          btn_skip.toggle(record.data.skip);
          break;
        }
      case JOBUNIT_STATUS_RUNNING:
        {
          btn_stop.setDisabled(false);
          break;
        }
      case JOBUNIT_STATUS_END:
        {
          break;
        }
      case JOBUNIT_STATUS_ERROR:
        {
          if (record.data.kind >= JOBUNIT_KIND_STARTJOB) {
            btn_hold.setDisabled(false);
            btn_hold.toggle(record.data.hold);
            btn_skip.setDisabled(false);
            btn_skip.toggle(record.data.skip);
            btn_start.setDisabled(false);
          }
          break;
        }
      default:
        {}
    }
  },
});