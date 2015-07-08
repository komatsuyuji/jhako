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
Ext.application({
  name: 'Jhako',
  appFolder: location.pathname + '/app',
  //autoCreateViewport: true,

  models: [
    'Session',
    'Jobunit', 'ProcJobunit', 'ProcTopjobnet', 'HistJobunit', 'JobunitChild',
    'Rootjobnet', 'Connector', 'Schedule', 'Variable',
    'Sleepjob', 'Clockjob', 'Datejob', 'Datecond', 'Vardatum',
    'Sshjob', 'Winjob', 'Condition',
    'Jobresult', 'Execlog',
    'Calendar', 'CalendarDate', 'User', 'Configure',
  ],

  stores: [
    'Jobunit', 'ProcJobunit', 'ProcTopjobnet', 'HistJobunit', 'ProcTopjobnetSummary',
    'Calendar', 'CalendarCombo', 'User'
  ],

  views: [
    'Header', 'Main', 'Footer', 'Viewer',
    'session.Win', 'session.Form',

    'common.Jobunit', 'common.Schedule',
    'common.Jobnet', 'common.Subjobnet', 'common.Variable',
    'common.Startjob', 'common.Mergejob', 'common.Sleepjob', 'common.Clockjob',
    'common.Datejob', 'common.Datecond', 'common.Varjob', 'common.Vardatum',
    'common.Sshjob', 'common.Winjob', 'common.Condition',
    'common.Jobresult', 'common.Execlog',

    'process.Dashboard', 'process.Summary', 'process.Tree',
    'process.Index', 'process.List', 'process.Detail',
    'process.Show', 'process.Flowchart', 'process.Sidebar', 'process.JobunitTab',

    'history.Index', 'history.List', 'history.Detail',
    'history.Show', 'history.Flowchart', 'history.Sidebar', 'history.JobunitTab',

    'editor.Index', 'editor.Tree', 'editor.TreeMenu', 'editor.JobunitFile',
    'editor.Tab', 'editor.List', 'editor.Detail',
    'editor.Show', 'editor.Draw', 'editor.DrawTab',
    'editor.Flowchart', 'editor.JobunitTab',

    'calendar.Index', 'calendar.List', 'calendar.Detail', 'calendar.Edit', 'calendar.Date',

    'Admin',
    'user.Index', 'user.List', 'user.Edit', 'user.Profile', 'user.Form',
    'configure.Index', 'configure.Edit'
  ],

  controllers: [
    'Session', 'Main', 'Header',
    'common.Jobnet', 'common.Schedule', 'common.Variable',
    'common.Subjobnet', 'common.Startjob', 'common.Mergejob',
    'common.Sleepjob', 'common.Clockjob',
    'common.Datejob', 'common.Datecond',
    'common.Varjob', 'common.Vardatum',
    'common.Sshjob', 'common.Winjob', 'common.Condition',
    'common.Jobresult', 'common.Execlog',

    'process.Summary', 'process.List',
    'process.Tree', 'process.Flowchart', 'process.JobunitTab', 'process.Sidebar',
    'history.List', 'history.Flowchart', 'history.JobunitTab',

    'editor.Tree', 'editor.TreeMenu', 'editor.JobunitFile',
    'editor.List', 'editor.Detail', 'editor.Flowchart',
    'editor.Draw', 'editor.DrawTab', 'editor.JobunitTab',

    'user.List', 'user.Profile',
    'Calendar', 'Configure'
  ],

  launch: function() {
    jhako_login_user = null;
    Jhako.model.Session.load(1, {
      failure: function(record, operation) {
        Ext.create('Jhako.view.session.Win').show();
      },
      success: function(record, operation) {
        jhako_login_user = new Jhako.model.User(record.getData());
        if (location.search.indexOf('proc_jobunit_id') == 1) {
          Ext.create('Jhako.view.process.Viewport');
        } else {
          Ext.create('Jhako.view.user.Profile');
          Ext.create('Jhako.view.Viewport');
        }
      }
    });
  },
});
