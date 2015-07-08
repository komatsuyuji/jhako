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


Ext.define('Jhako.controller.Main', {
  extend: 'Ext.app.Controller',
  refs: [{
    ref: 'jhakoMain',
    selector: 'jhakoMain'
  }, {
    ref: 'jhakoViewer',
    selector: 'jhakoViewer'
  }, {
    ref: 'editorIndex',
    selector: 'editorIndex'
  }, {
    ref: 'calendarIndex',
    selector: 'calendarIndex'
  }, {
    ref: 'jhakoAdmin',
    selector: 'jhakoAdmin'
  }, {
    ref: 'processSummary',
    selector: 'processSummary'
  }, {
    ref: 'processList',
    selector: 'processList'
  }, {
    ref: 'historyList',
    selector: 'historyList'
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
      'jhakoMain': {
        afterrender: this.onAfterrender,
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
  onAfterrender: function(tab, e) {
    var me = this;

    if (!jhako_login_user)
      return;

    var tab_main = me.getJhakoMain();
    var tab_viewer = me.getJhakoViewer();
    var tab_editor = me.getEditorIndex();
    var tab_calender = me.getCalendarIndex();
    var tab_admin = me.getJhakoAdmin();
    var pnl_process = me.getProcessList();
    var pnl_history = me.getHistoryList();

    // set disabled tab
    tab_main.setActiveTab(0);
    tab_viewer.setDisabled(true);
    tab_editor.setDisabled(true);
    tab_calender.setDisabled(true);
    tab_admin.setDisabled(true);

    tab_viewer.setActiveTab('process_index');
    tab_viewer.setActiveTab(0);

    switch (jhako_login_user.data.kind) {
      case USER_KIND_MONITOR:
        {
          tab_viewer.setDisabled(false);
          break;
        };
      case USER_KIND_OPERATOR:
        {
          tab_viewer.setDisabled(false);
          break;
        };
      case USER_KIND_POWER:
        {
          tab_viewer.setDisabled(false);
          tab_editor.setDisabled(false);
          break;
        };
      case USER_KIND_RESOURCE:
        {
          tab_viewer.setDisabled(false);
          tab_editor.setDisabled(false);
          tab_calender.setDisabled(false);
          break;
        };
      case USER_KIND_ADMIN:
        {
          tab_viewer.setDisabled(false);
          tab_editor.setDisabled(false);
          tab_calender.setDisabled(false);
          tab_admin.setDisabled(false);
          break;
        };
      default:
        {
          break;
        }
    }

    // create body mask
    jhako_mask = new Ext.LoadMask(Ext.getBody(), {
      msg: "Please wait..."
    });

    // set timer
    var proc_paging = pnl_process.query('#proc_pagingtoolbar')[0];
    var hist_paging = pnl_history.query('#hist_pagingtoolbar')[0];

    var store_summary = Jhako.app.getStore('ProcTopjobnetSummary');
    var store_topjobnet = Jhako.app.getStore('ProcTopjobnet');
    var store_history = Jhako.app.getStore('HistJobunit');
    var ctrl_process = Jhako.app.getController('process.List');
    var errs = 0;
    var interval = setInterval(function() {
      // check the session
      Jhako.model.Session.load(1, {
        failure: function(record, operation) {
          errs++;
          if (errs <= 10)
            return;
          Ext.Msg.alert(I18n.t('views.msg.alert'), I18n.t('views.msg.logged_different'), function(btn, text) {
            if (btn == 'ok') {
              window.location.reload();
            }
          });
        },
        success: function(record, operation) {
          errs = 0;
        }
      });

      // load data
      if (tab_main.getActiveTab().getItemId() != 'jhako_viewer')
        return;

      var itemid = tab_viewer.getActiveTab().getItemId();
      switch (itemid) {
        case 'process_dashboard':
          {
            if (!store_summary)
              return;
            if (store_summary.isLoading())
              return;

            if (jhako_interval_count >= jhako_summary_interval)
              jhako_interval_count = 0;
            if (jhako_interval_count != 0)
              break;
            store_summary.reload();
            break;
          }
        case 'process_index':
          {
            if (!store_topjobnet)
              return;
            if (store_topjobnet.isLoading())
              return;

            if (jhako_interval_count >= jhako_process_interval)
              jhako_interval_count = 0;
            if (jhako_interval_count != 0)
              break;

            if (store_topjobnet.count() == 0) {
              proc_paging.moveFirst();
            } else {
              store_topjobnet.reload();
            }
            ctrl_process.onLoadJobnet(jhako_topjobnet_id);
            break;
          }
        case 'history_index':
          {
            if (!store_history)
              return;
            if (store_history.isLoading())
              return;

            if (jhako_interval_count >= jhako_history_interval)
              jhako_interval_count = 0;
            if (jhako_interval_count != 0)
              break;

            if (store_history.count() == 0) {
              hist_paging.moveFirst();
            } else {
              store_history.reload();
            }
            break;
          }
        default:
          {}
      }
      jhako_interval_count++;
    }, 1000);
  },

});
