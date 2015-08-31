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

Ext.define('Jhako.controller.process.List', {
  extend: 'Ext.app.Controller',
  refs: [{
    ref: 'processList',
    selector: 'processList'
  }, {
    ref: 'processShow',
    selector: 'processShow'
  }, {
    ref: 'userForm',
    selector: 'userForm'
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
      'processList': {
        afterrender: this.onAfterrender,
        cellclick: this.onCellclick,
        celldblclick: this.onCelldblclick,
      },
      'processList menuitem[action=selected_forcestart]': {
        click: this.onSelectedforcestart
      },
      'processList menuitem[action=selected_forcestop]': {
        click: this.onSelectedforcestop
      },
      'processList menuitem[action=selected_hold]': {
        click: this.onSelectedhold
      },
      'processList menuitem[action=selected_unhold]': {
        click: this.onSelectedunhold
      },
      'processList menuitem[action=selected_skip]': {
        click: this.onSelectedskip
      },
      'processList menuitem[action=selected_unskip]': {
        click: this.onSelectedunskip
      },
      'processList button[action=clear_filter]': {
        click: this.onClearfilter
      },
      'processList menuitem[action=set_interval]': {
        click: this.onSetInterval
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
  onAfterrender: function(grid) {
    var me = this;
    if (!jhako_login_user)
      return;

    if (jhako_login_user.data.kind == USER_KIND_MONITOR) {
      var btn = grid.down('#btn_selected_proceed');
      btn.setVisible(false);
    }

    me.onSetIconCls();
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
  onCellclick: function(gridview, td, cellIndex, record, tr, rowIndex, e) {
    if (!record)
      return;

    jhako_topjobnet_id = record.data.proc_jobunit_id;
    jhako_interval_count = 0;
    var ctrl = Jhako.app.getController('process.Flowchart');
    ctrl.onReset();
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
  onCelldblclick: function(gridview, td, cellIndex, record, tr, rowIndex, e) {
    if (!record)
      return;
    if (record.data.proc_jobunit_id <= 0)
      return;
    window.open("?proc_jobunit_id=" + record.data.proc_jobunit_id);
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
  onLoadJobnet: function(id) {
    if (!id)
      return;

    var me = this;
    var form = me.getProcessShow();
    var ctrl = Jhako.app.getController('process.Flowchart');

    Jhako.model.ProcJobunit.load(id, {
      failure: function(record) {
        jhako_topjobnet_id = 0;
        form.getForm().reset(true);
        ctrl.onReset();
      },
      success: function(record) {
        JhakoSetForm(form, record);
        ctrl.onDrawFlowchart(record);
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
  onSelectedforcestart: function(item, e) {
    var me = this;
    var grid = me.getProcessList();
    var records = grid.getSelectionModel().getSelection();
    records.forEach(function(record) {
      switch (record.data.status) {
        case JOBUNIT_STATUS_STANDBY:
        case JOBUNIT_STATUS_ERROR:
        case JOBUNIT_STATUS_OUTSCHEDULE:
        case JOBUNIT_STATUS_CARRYOVER:
          {
            record.set("force_start", true);
            record.save();
            break;
          }
      }
    });
    grid.getSelectionModel().deselectAll();
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
  onSelectedforcestop: function(item, e) {
    var me = this;
    var grid = me.getProcessList();
    var records = grid.getSelectionModel().getSelection();
    records.forEach(function(record) {
      switch (record.data.status) {
        case JOBUNIT_STATUS_RUNNING:
        case JOBUNIT_STATUS_ERROR:
          {
            record.set("force_stop", true);
            record.save();
            break;
          }
      }
    });
    grid.getSelectionModel().deselectAll();
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
  onSelectedhold: function(item, e) {
    var me = this;
    var grid = me.getProcessList();
    var records = grid.getSelectionModel().getSelection();
    records.forEach(function(record) {
      switch (record.data.status) {
        case JOBUNIT_STATUS_NONE:
        case JOBUNIT_STATUS_STANDBY:
        case JOBUNIT_STATUS_READY:
          {
            record.set("hold", true);
            record.save();
            break;
          }
      }
    });
    grid.getSelectionModel().deselectAll();
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
  onSelectedunhold: function(item, e) {
    var me = this;
    var grid = me.getProcessList();
    var records = grid.getSelectionModel().getSelection();
    records.forEach(function(record) {
      switch (record.data.status) {
        case JOBUNIT_STATUS_NONE:
        case JOBUNIT_STATUS_STANDBY:
        case JOBUNIT_STATUS_READY:
          {
            record.set("hold", false);
            record.save();
            break;
          }
      }
    });
    grid.getSelectionModel().deselectAll();
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
  onSelectedskip: function(item, e) {
    var me = this;
    var grid = me.getProcessList();
    var records = grid.getSelectionModel().getSelection();
    records.forEach(function(record) {
      switch (record.data.status) {
        case JOBUNIT_STATUS_NONE:
        case JOBUNIT_STATUS_STANDBY:
        case JOBUNIT_STATUS_READY:
          {
            record.set("skip", true);
            record.save();
            break;
          }
      }
    });
    grid.getSelectionModel().deselectAll();
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
  onSelectedunskip: function(item, e) {
    var me = this;
    var grid = me.getProcessList();
    var records = grid.getSelectionModel().getSelection();
    records.forEach(function(record) {
      switch (record.data.status) {
        case JOBUNIT_STATUS_NONE:
        case JOBUNIT_STATUS_STANDBY:
        case JOBUNIT_STATUS_READY:
          {
            record.set("skip", false);
            record.save();
            break;
          }
      }
    });
    grid.getSelectionModel().deselectAll();
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
  onClearfilter: function(button, e) {
    var me = this;
    var grid = me.getProcessList();
    grid.filters.clearFilters();
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
  onSetInterval: function(item, e) {
    var me = this;
    jhako_process_interval = item.interval;
    me.onSetIconCls();
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
  onSetIconCls: function() {
    var me = this;
    var grid = me.getProcessList();
    var menuitems = grid.query('menuitem');

    menuitems.forEach(function(item) {
      if (item.interval) {
        if (jhako_process_interval == item.interval) {
          item.setIconCls('');
        } else {
          item.setIconCls('x-hide-display');
        }
      }
    });
  },

});