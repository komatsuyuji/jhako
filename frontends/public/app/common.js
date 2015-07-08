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

var jhako_mask = null;

var jhako_summary_interval = 5;
var jhako_process_interval = 5;
var jhako_history_interval = 60;
var jhako_interval_count = 0;
var jhako_topjobnet_id = 0;

var jhako_login_user = null;
var jhako_selected_parent = null;
var jhako_selected_rootjobnet = null;
var jhako_selected_jobunits = [];
var jhako_selected_conns = [];

var jhako_surface_state = 0;
var jhako_surface_mouseDown = false;
var jhako_surface_x1 = 0;
var jhako_surface_y1 = 0;
var jhako_surface_x2 = 0;
var jhako_surface_y2 = 0;
var jhako_surface_choiceX = 0;
var jhako_surface_choiceY = 0;
var jhako_surface_startpointX = 0;
var jhako_surface_startpointY = 0;
var jhako_surface_endpointX = 0;
var jhako_surface_endpointY = 0;
var jhako_surface_jobunits = [];
var jhako_surface_conns = [];
var jhako_surface_inconns = [];
var jhako_surface_outconns = [];

var JHAKO_THIS_YEAR = (new Date()).getFullYear();
var JHAKO_DRAW_WIDTH = 15;

var JHAKO_STATE_NONE = 0;
var JHAKO_STATE_STARTPOINT = 1;
var JHAKO_STATE_ENDPOINT = 2;
var JHAKO_STATE_CONNECTOR = 3;
var JHAKO_STATE_JOB = 4;
var JHAKO_STATE_NEWCONN = 5;

var JHAKO_CONN_IN = 0;
var JHAKO_CONN_OUT = 1;
var JHAKO_CONN_WHOLE = 2;

var JHAKO_DAYOFWEEK_DATA = [
  [0, I18n.t('views.week.sun')],
  [1, I18n.t('views.week.mon')],
  [2, I18n.t('views.week.tue')],
  [3, I18n.t('views.week.wed')],
  [4, I18n.t('views.week.thu')],
  [5, I18n.t('views.week.fri')],
  [6, I18n.t('views.week.sat')]
];

var JHAKO_DAY_DATA = [
  1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31
];
var JHAKO_HOUR_DATA = [
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
  20, 21, 22, 23
];
var JHAKO_MINUTE_DATA = [
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
  20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
  40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59
];
var JHAKO_SECOND_DATA = JHAKO_MINUTE_DATA;

var JOBUNIT_KIND_JOBGROUP = 0;
var JOBUNIT_KIND_ROOTJOBNET = 10;
var JOBUNIT_KIND_JOBNET = 20;
var JOBUNIT_KIND_STARTJOB = 100;
var JOBUNIT_KIND_ENDJOB = 101;
var JOBUNIT_KIND_MERGEJOB = 102;
var JOBUNIT_KIND_SLEEPJOB = 103;
var JOBUNIT_KIND_CLOCKJOB = 104;
var JOBUNIT_KIND_DATEJOB = 105;
var JOBUNIT_KIND_VARJOB = 106;
var JOBUNIT_KIND_SSHJOB = 200;
var JOBUNIT_KIND_WINJOB = 201;

var JOBUNIT_STATUS_NONE = 0;
var JOBUNIT_STATUS_STANDBY = 1;
var JOBUNIT_STATUS_READY = 2;
var JOBUNIT_STATUS_RUNNING = 3;
var JOBUNIT_STATUS_END = 4;
var JOBUNIT_STATUS_ERROR = 5;
var JOBUNIT_STATUS_OUTSCHEDULE = 6;
var JOBUNIT_STATUS_CARRYOVER = 7;
var JOBUNIT_STATUS_FORCESTOP = 8;
var JOBUNIT_STATUS_DATA = [
  [JOBUNIT_STATUS_NONE, I18n.t('views.jobunit_status.none')],
  [JOBUNIT_STATUS_STANDBY, I18n.t('views.jobunit_status.standby')],
  [JOBUNIT_STATUS_READY, I18n.t('views.jobunit_status.ready')],
  [JOBUNIT_STATUS_RUNNING, I18n.t('views.jobunit_status.running')],
  [JOBUNIT_STATUS_END, I18n.t('views.jobunit_status.end')],
  [JOBUNIT_STATUS_ERROR, I18n.t('views.jobunit_status.error')],
  [JOBUNIT_STATUS_OUTSCHEDULE, I18n.t('views.jobunit_status.out_schedule')],
  [JOBUNIT_STATUS_CARRYOVER, I18n.t('views.jobunit_status.carry_over')],
  [JOBUNIT_STATUS_FORCESTOP, I18n.t('views.jobunit_status.force_stop')]
];

var TOPJOBNET_MODE_SCHEDULE = 0;
var TOPJOBNET_MODE_IMMEDIATE = 1;
var TOPJOBNET_MODE_SPECIFIED = 2;
var TOPJOBNET_MODE_DATA = [
  [TOPJOBNET_MODE_SCHEDULE, I18n.t('views.topjobnet_mode.schedule')],
  [TOPJOBNET_MODE_IMMEDIATE, I18n.t('views.topjobnet_mode.immediate')],
  [TOPJOBNET_MODE_SPECIFIED, I18n.t('views.topjobnet_mode.specified')]
];

var ROOTJOBNET_RUNTYPE_DATA = [
  [0, I18n.t('views.rootjobnet_runtype.exclusive')],
  [1, I18n.t('views.rootjobnet_runtype.fullexclusive')],
  [2, I18n.t('views.rootjobnet_runtype.parallel')]
];

var ROOTJOBNET_DELAY_DATA = [
  [0, 0],
  [1, '1' + I18n.t('views.date.min')],
  [5, '5' + I18n.t('views.date.min')],
  [10, '10' + I18n.t('views.date.min')],
  [20, '20' + I18n.t('views.date.min')],
  [30, '30' + I18n.t('views.date.min')],
  [60, '1' + I18n.t('views.date.hour')],
  [120, '2' + I18n.t('views.date.hour')],
  [180, '3' + I18n.t('views.date.hour')],
];

var ROOTJOBNET_LOADER_DATA = [
  [5, '5' + I18n.t('views.date.min')],
  [15, '15' + I18n.t('views.date.min')],
  [30, '30' + I18n.t('views.date.min')],
  [60, '1' + I18n.t('views.date.hour')],
  [180, '3' + I18n.t('views.date.hour')],
  [360, '6' + I18n.t('views.date.hour')],
  [720, '12' + I18n.t('views.date.hour')],
  [1440, '1' + I18n.t('views.date.day')],
  [2880, '2' + I18n.t('views.date.day')],
  [4320, '3' + I18n.t('views.date.day')]
];

var CONNECTOR_KIND_NORMAL = 0;
var CONNECTOR_KIND_BRANCH = 1;
var CONNECTOR_KIND_ERROR = 2;

var DATECOND_KIND_EVERY_WEEK = 0;
var DATECOND_KIND_FIRST_WEEK = 1;
var DATECOND_KIND_SECOND_WEEK = 2;
var DATECOND_KIND_THIRD_WEEK = 3;
var DATECOND_KIND_FOURTH_WEEK = 4;
var DATECOND_KIND_FIFTH_WEEK = 5;
var DATECOND_KIND_EVERY_MONTH = 6;
var DATECOND_KIND_REVERSE_MONTH = 7;
var DATECOND_KIND_CALENDAR = 100;
var DATECOND_KIND_DATA = [
  [DATECOND_KIND_EVERY_WEEK, I18n.t('views.datecond_kind.every_week')],
  [DATECOND_KIND_FIRST_WEEK, I18n.t('views.datecond_kind.first_week')],
  [DATECOND_KIND_SECOND_WEEK, I18n.t('views.datecond_kind.second_week')],
  [DATECOND_KIND_THIRD_WEEK, I18n.t('views.datecond_kind.third_week')],
  [DATECOND_KIND_FOURTH_WEEK, I18n.t('views.datecond_kind.fourth_week')],
  [DATECOND_KIND_FIFTH_WEEK, I18n.t('views.datecond_kind.fifth_week')],
  [DATECOND_KIND_EVERY_MONTH, I18n.t('views.datecond_kind.every_month')],
  [DATECOND_KIND_REVERSE_MONTH, I18n.t('views.datecond_kind.reverse_month')],
  [DATECOND_KIND_CALENDAR, I18n.t('views.model.calendar')]
];

var CONDITION_METHOD_ERROR = 0;
var CONDITION_METHOD_BRANCH = 1;
var CONDITION_KIND_DATA = [
  [0, I18n.t('views.jobresult.exitcode')],
  //[1, I18n.t('views.jobresult.signal')],
  [2, I18n.t('views.jobresult.stdout')],
  [3, I18n.t('views.jobresult.stderr')]
];

var USER_KIND_MONITOR = 0;
var USER_KIND_OPERATOR = 3;
var USER_KIND_POWER = 5;
var USER_KIND_RESOURCE = 7;
var USER_KIND_ADMIN = 9;
var USER_KIND_DATA = [
  [USER_KIND_MONITOR, I18n.t('views.user_kind.monitor')],
  [USER_KIND_OPERATOR, I18n.t('views.user_kind.operator')],
  [USER_KIND_POWER, I18n.t('views.user_kind.power')],
  [USER_KIND_RESOURCE, I18n.t('views.user_kind.resource')],
  [USER_KIND_ADMIN, I18n.t('views.user_kind.admin')]
];

var USER_LANGUAGE_DATA = [
  ['en', 'English'],
  ['ja', '日本語'],
  ['zh_CN', '中文简体']
];

var CONFIGURE_VIEWER_DATA = [
  [1, '1' + I18n.t('views.date.hour')],
  [3, '3' + I18n.t('views.date.hour')],
  [6, '6' + I18n.t('views.date.hour')],
  [12, '12' + I18n.t('views.date.hour')],
  [24, '1' + I18n.t('views.date.day')],
  [48, '2' + I18n.t('views.date.day')],
  [72, '3' + I18n.t('views.date.day')],
  [96, '4' + I18n.t('views.date.day')],
  [120, '5' + I18n.t('views.date.day')],
];

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
Ext.override(Ext.form.DisplayField, {
  getValue: function() {
    return this.value;
  },
  setValue: function(v) {
    var me = this;
    me.value = v;
    if (me.dateFormat && Ext.isDate(v)) {
      me.setRawValue(Ext.Date.dateFormat(v, me.dateFormat));
    } else
    if (me.numberFormat && typeof v == 'number') {
      me.setRawValue(Ext.util.Format.number(v, me.numberFormat));
    } else {
      me.setRawValue(v);
    }
    return me;
  },
});

/*
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
Ext.override(Ext.dd.DragDropManager, {
  startDrag: function(x, y) {
    var me = this,
      current = me.dragCurrent,
      dragEl;
    clearTimeout(me.clickTimeout);
    if (current) {
      current.b4StartDrag(x, y);
      current.startDrag(x, y);
      dragEl = current.getDragEl();
      if (dragEl && dragEl.dom.className.replace) {
        Ext.fly(dragEl).addCls(me.dragCls);
      }
    }
    me.dragThreshMet = true;
  }
});
*/

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
function JhakoMetaStyle(status) {
  switch (status) {
    case JOBUNIT_STATUS_RUNNING:
      {
        return "color:blue";
      }
    case JOBUNIT_STATUS_END:
      {
        return "color:darkgreen";
      }
    case JOBUNIT_STATUS_ERROR:
      {
        return "color:crimson";
      }
    case JOBUNIT_STATUS_OUTSCHEDULE:
    case JOBUNIT_STATUS_CARRYOVER:
      {
        return "color:orange";
      }
    case JOBUNIT_STATUS_FORCESTOP:
      {
        return "color:brown";
      }
    default:
      {}
  }
  return "color:black";
};

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
function JhakoFindRecord(array, value) {
  if (!array)
    return '';

  for (var i = 0; i < array.length; i++) {
    if (array[i][0] == value)
      return array[i][1];
  }

  return '';
};

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
function JhakoSetForm(form, record) {
  if (!form)
    return;
  if (!record) {
    form.getForm().reset(true);
    return;
  }

  form.loadRecord(record);
  var field = form.getForm().findField('status');
  if (field) {
    field.setValue(JhakoFindRecord(JOBUNIT_STATUS_DATA, record.data.status));
  }
};

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
function JhakoGetCondition(record, method, model) {
  if (!record) return;
  var store = new Ext.data.Store({
    model: 'Jhako.model.Condition'
  });
  store.getProxy().url = location.pathname + '/' + model + '/' + record.data.id + '/conditions';

  if (record['jhako.model.conditionsStore']) {
    record['jhako.model.conditionsStore'].each(function(cond) {
      if (cond.data.method == method) {
        store.add(cond);
      }
    });
  };
  return store;
};

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
function JhakoMakeName(store, name, att) {
  var newname = name;
  if (!store)
    return newname;

  var i, num;
  for (i = 100; i < 900; i++) {
    if (store.find('name', newname) == -1)
      break;
    num = 100 + Math.floor(Math.random() * 900);
    newname = name + '_' + att + num;
  }

  return newname;
};

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
function JhakoDrawArrow(x1, y1, x2, y2, kind) {
  var len = 10;
  var rotate = 30 * (Math.PI / 180);
  var arrow = Ext.String.format('M{0} {1}', x1, y1);
  var rad = Math.atan2(0, -1);

  //rad = Math.atan2(y1 - y2, x1 - x2);
  if (kind == 1) {
    arrow += Ext.String.format('M{0} {1}', x1, y1 + 15);
    arrow += Ext.String.format('L{0} {1}', x1, y1 + 20);
    y1 = y1 + 20;
  }

  if (x2 > x1 + 20) {
    var cx = (x1 + x2) / 2;
    arrow += Ext.String.format('L{0} {1}', cx, y1);
    arrow += Ext.String.format('L{0} {1}', cx, y2);
  } else {
    var cy = (y1 + y2) / 2;
    if (kind == 0) {
      arrow += Ext.String.format('L{0} {1}', x1 + 10, y1);
      arrow += Ext.String.format('L{0} {1}', x1 + 10, cy);
    } else
    if (kind == 1) {
      arrow += Ext.String.format('L{0} {1}', x1, cy);
    }
    arrow += Ext.String.format('L{0} {1}', x2 - 10, cy);
    arrow += Ext.String.format('L{0} {1}', x2 - 10, y2);
    arrow += Ext.String.format('L{0} {1}', x2 - 10, y2);
  }

  var sx = Math.cos(rad + rotate) * len;
  var sy = Math.sin(rad + rotate) * len;
  var ex = Math.cos(rad - rotate) * len;
  var ey = Math.sin(rad - rotate) * len;
  arrow += Ext.String.format('L{0} {1}', x2, y2);
  arrow += Ext.String.format('M{0} {1} L{2} {3} L{4} {5}', x2 + sx, y2 + sy, x2, y2, x2 + ex, y2 + ey);
  return arrow;
};

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
function JhakoDrawConnector(record, prev_job, next_job, view) {
  if (!record) return;
  if (!prev_job) return;
  var cursor = (view) ? 'auto' : 'pointer';

  var x1, y1, x2, y2;
  if (record.data.kind == CONNECTOR_KIND_NORMAL) {
    x1 = prev_job.data.x + JHAKO_DRAW_WIDTH * 2;
    y1 = prev_job.data.y + JHAKO_DRAW_WIDTH;
  } else
  if (record.data.kind == CONNECTOR_KIND_BRANCH) {
    x1 = prev_job.data.x + JHAKO_DRAW_WIDTH;
    y1 = prev_job.data.y + JHAKO_DRAW_WIDTH * 2;
  }
  if (next_job) {
    x2 = next_job.data.x;
    y2 = next_job.data.y + JHAKO_DRAW_WIDTH;
  } else {
    x2 = x1 + 1;
    y2 = y1 + 1;
  };

  var stroke = 'black';
  if (record.data.kind == CONNECTOR_KIND_BRANCH) {
    stroke = 'dimgray';
  };
  if (prev_job.data.status == JOBUNIT_STATUS_END) {
    switch (next_job.data.status) {
      case JOBUNIT_STATUS_RUNNING:
        {
          stroke = 'blue';
          if (next_job.data.timeout_occur) {
            stroke = 'orange';
          }
          break;
        }
      case JOBUNIT_STATUS_END:
        {
          stroke = 'green';
          if (next_job.data.timeout_occur) {
            stroke = 'orange';
          }
          break;
        }
      case JOBUNIT_STATUS_ERROR:
        {
          stroke = 'red';
          break;
        }
    }
  }

  var arrow = {
    record: record,
    prev_job: prev_job,
    next_job: next_job,
    type: 'path',
    path: JhakoDrawArrow(x1, y1, x2, y2, record.data.kind),
    stroke: stroke,
    'stroke-width': 3,
    style: {
      cursor: cursor
    },
    zIndex: 10,
    group: 'spriteconn_' + record.data.id
  };

  var name = '';
  if (record.data.kind == CONNECTOR_KIND_BRANCH) {
    name = 'F';
  };

  var text = {
    record: record,
    prev_job: prev_job,
    next_job: next_job,
    type: 'text',
    x: (x1 + x2) / 2 - 10,
    y: (y1 + y2) / 2 + 10,
    text: name,
    style: {
      cursor: cursor
    },
    zIndex: 11,
    group: 'spriteconn_' + record.data.id
  };
  var sprites = [arrow, text];
  return sprites;
};

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
function JhakoDrawJob(record, view) {
  if (!record) return;
  if (!record.data) return;
  var group = 'spritegroup_' + record.data.id;
  var cursor = (view) ? 'auto' : 'move';

  var img = {
    record: record,
    type: 'image',
    width: 2 * JHAKO_DRAW_WIDTH,
    height: 2 * JHAKO_DRAW_WIDTH,
    x: record.data.x,
    y: record.data.y,
    style: {
      cursor: cursor
    },
    zIndex: 50,
    group: [group, 'spritejob_' + record.data.id]
  };

  var name = record.data.name;
  var text = {
    record: record,
    type: "text",
    text: name,
    x: record.data.x,
    y: record.data.y,
    style: {
      cursor: cursor
    },
    translate: {
      x: JHAKO_DRAW_WIDTH - name.length * 3,
      y: 2 * JHAKO_DRAW_WIDTH + 5
    },
    zIndex: 51,
    group: [group, 'spritetext_' + record.data.id]
  };

  var hold = {
    record: record,
    type: 'image',
    width: JHAKO_DRAW_WIDTH,
    height: JHAKO_DRAW_WIDTH,
    x: record.data.x,
    y: record.data.y,
    style: {
      cursor: cursor
    },
    src: location.pathname + '/images/icons/control_pause_blue.png',
    opacity: (record.data.hold) ? 1 : 0,
    translate: {
      x: 0,
      y: 5 - JHAKO_DRAW_WIDTH
    },
    zIndex: 60,
    group: [group, 'spritehold_' + record.data.id]
  };

  var skip = {
    record: record,
    type: 'image',
    width: JHAKO_DRAW_WIDTH,
    height: JHAKO_DRAW_WIDTH,
    x: record.data.x,
    y: record.data.y,
    style: {
      cursor: cursor
    },
    src: location.pathname + '/images/icons/control_fastforward_blue.png',
    opacity: (record.data.skip) ? 1 : 0,
    translate: {
      x: JHAKO_DRAW_WIDTH,
      y: 5 - JHAKO_DRAW_WIDTH
    },
    zIndex: 61,
    group: [group, 'spriteskip_' + record.data.id]
  };

  var status_img = '';
  var status_opacity = 1;
  switch (record.data.status) {
    case JOBUNIT_STATUS_NONE:
    case JOBUNIT_STATUS_STANDBY:
      {
        status_img = location.pathname + '/images/icons/control_play.png';
        status_opacity = 0;
        break;
      }
    case JOBUNIT_STATUS_READY:
      {
        status_img = location.pathname + '/images/icons/control_play.png';
        break;
      }
    case JOBUNIT_STATUS_RUNNING:
      {
        status_img = location.pathname + '/images/icons/control_play_blue.png';
        break;
      }
    case JOBUNIT_STATUS_END:
      {
        status_img = location.pathname + '/images/icons/tick.png';
        break;
      }
    case JOBUNIT_STATUS_ERROR:
      {
        status_img = location.pathname + '/images/icons/cross.png';
        break;
      }
    default:
      {
        status_img = location.pathname + '/images/icons/exclamation.png';
        break;
      }
  }

  var subscript_img = '';
  var subscript_opacity = 0;
  if (record.data.force_stop) {
    subscript_img = location.pathname + '/images/icons/stop.png';
    subscript_opacity = 1;
  } else
  if (record.data.force_start) {
    subscript_img = location.pathname + '/images/icons/control_repeat_blue.png';
    subscript_opacity = 1;
  } else
  if (record.data.timeout_occur) {
    subscript_img = location.pathname + '/images/icons/error.png';
    subscript_opacity = 1;
  }

  var subscript = {
    record: record,
    type: 'image',
    width: JHAKO_DRAW_WIDTH,
    height: JHAKO_DRAW_WIDTH,
    x: record.data.x,
    y: record.data.y,
    src: subscript_img,
    opacity: subscript_opacity,
    translate: {
      x: 2 * JHAKO_DRAW_WIDTH - 2,
      y: -4
    },
    zIndex: 70,
    group: [group, 'spritessubscript_' + record.data.id]
  };

  var status = {
    record: record,
    type: 'image',
    width: JHAKO_DRAW_WIDTH,
    height: JHAKO_DRAW_WIDTH,
    x: record.data.x,
    y: record.data.y,
    src: status_img,
    translate: {
      x: 2 * JHAKO_DRAW_WIDTH - 2,
      y: JHAKO_DRAW_WIDTH + 2
    },
    opacity: status_opacity,
    zIndex: 70,
    group: [group, 'spritesstatus_' + record.data.id]
  };

  var in_point = {
    record: record,
    type: "circle",
    radius: 4,
    x: record.data.x,
    y: record.data.y,
    translate: {
      x: 0,
      y: JHAKO_DRAW_WIDTH
    },
    fill: 'yellow',
    stroke: 'black',
    'stroke-width': 1,
    zIndex: 80,
    group: [group, 'spriteinpoint_' + record.data.id, 'flowchart_inpoint']
  };

  var out_point = {
    record: record,
    type: "circle",
    radius: 4,
    x: record.data.x,
    y: record.data.y,
    translate: {
      x: 2 * JHAKO_DRAW_WIDTH,
      y: JHAKO_DRAW_WIDTH
    },
    fill: 'yellow',
    stroke: 'black',
    'stroke-width': 1,
    style: {
      cursor: 'crosshair'
    },
    zIndex: 81,
    kind: CONNECTOR_KIND_NORMAL,
    group: [group, 'spriteoutpoint_' + record.data.id, 'flowchart_outpoint']
  };

  var branch_point = {
    record: record,
    type: "circle",
    radius: 4,
    x: record.data.x,
    y: record.data.y,
    translate: {
      x: JHAKO_DRAW_WIDTH,
      y: 2 * JHAKO_DRAW_WIDTH
    },
    fill: 'yellow',
    stroke: 'black',
    'stroke-width': 1,
    style: {
      cursor: 'crosshair'
    },
    zIndex: 82,
    kind: CONNECTOR_KIND_BRANCH,
    group: [group, 'spritebranchpoint_' + record.data.id, 'flowchart_branchpoint']
  };

  var sprites = [img, text, hold, skip];
  if (view) {
    sprites.push(subscript, status);
  };
  switch (record.data.kind) {
    case JOBUNIT_KIND_JOBNET:
      {
        img.src = location.pathname + '/images/icons/chart_organisation.png';
        if (!view) sprites.push(in_point, out_point);
        break;
      }
    case JOBUNIT_KIND_STARTJOB:
      {
        img.src = location.pathname + '/images/icons/door_in.png';
        if (!view) sprites.push(out_point);
        break;
      }
    case JOBUNIT_KIND_ENDJOB:
      {
        img.src = location.pathname + '/images/icons/door_out.png';
        if (!view) sprites.push(in_point);
        break;
      }
    case JOBUNIT_KIND_MERGEJOB:
      {
        img.src = location.pathname + '/images/icons/arrow_merge.png';
        if (!view) sprites.push(in_point, out_point);
        break;
      }
    case JOBUNIT_KIND_SLEEPJOB:
      {
        img.src = location.pathname + '/images/icons/time.png';
        if (!view) sprites.push(in_point, out_point);
        break;
      }
    case JOBUNIT_KIND_CLOCKJOB:
      {
        img.src = location.pathname + '/images/icons/clock.png';
        if (!view) sprites.push(in_point, out_point, branch_point);
        break;
      }
    case JOBUNIT_KIND_DATEJOB:
      {
        img.src = location.pathname + '/images/icons/date.png';
        if (!view) sprites.push(in_point, out_point, branch_point);
        break;
      }
    case JOBUNIT_KIND_VARJOB:
      {
        img.src = location.pathname + '/images/icons/basket_put.png';
        if (!view) sprites.push(in_point, out_point);
        break;
      }
    case JOBUNIT_KIND_SSHJOB:
      {
        img.src = location.pathname + '/images/icons/server.png';
        if (!view) sprites.push(in_point, out_point, branch_point);
        break;
      }
    case JOBUNIT_KIND_WINJOB:
      {
        img.src = location.pathname + '/images/icons/computer.png';
        if (!view) sprites.push(in_point, out_point, branch_point);
        break;
      }
    default:
      {
        break;
      }
  };
  return sprites;
};

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
Ext.define('Jhako.draw.Grid', {
  constructor: function(width, height) {
    var sprite = {
      type: "path",
      stroke: '#C0C0C0',
      zIndex: 0,
      group: 'flowchart_grid'
    };

    var gridpath = '';
    var e = Math.floor(width / JHAKO_DRAW_WIDTH) + 1;
    var g = Math.floor(height / JHAKO_DRAW_WIDTH) + 1;
    for (var d = 0; d < g; d++) {
      for (var c = 0; c < e; c++) {
        var x = c * JHAKO_DRAW_WIDTH * 2;
        var y = d * JHAKO_DRAW_WIDTH * 2;
        var xh = x + JHAKO_DRAW_WIDTH;
        var yh = y + JHAKO_DRAW_WIDTH;
        gridpath += Ext.String.format('M{0} {1} L{2} {3}', x - 2, y, x + 2, y);
        gridpath += Ext.String.format('M{0} {1} L{2} {3}', x, y - 2, x, y + 2);
        gridpath += Ext.String.format('M{0} {1} L{2} {3}', xh - 1, yh, xh + 1, yh);
      };
    };
    sprite.path = gridpath;
    return sprite;
  }
});

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
Ext.define('Jhako.draw.Choice', {
  constructor: function() {
    var sprite = {
      type: "rect",
      width: JHAKO_DRAW_WIDTH,
      height: JHAKO_DRAW_WIDTH,
      x: 0,
      y: 0,
      style: {
        cursor: 'move'
      },
      stroke: 'gray',
      fill: 'white',
      'stroke-width': 1,
      zIndex: 1,
      group: 'flowchart_choice'
    };
    return sprite;
  }
});

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
Ext.define('Jhako.draw.StartPoint', {
  constructor: function() {
    var sprite = {
      type: "circle",
      radius: 4,
      fill: 'yellow',
      stroke: 'black',
      'stroke-width': 1,
      x: 0,
      y: 0,
      style: {
        cursor: 'crosshair'
      },
      record: null,
      zIndex: 100,
      group: "flowchart_startpoint"
    };
    return sprite;
  }
});

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
Ext.define('Jhako.draw.EndPoint', {
  constructor: function() {
    var sprite = {
      type: "circle",
      radius: 4,
      fill: 'yellow',
      stroke: 'black',
      'stroke-width': 1,
      x: 0,
      y: 0,
      style: {
        cursor: 'crosshair'
      },
      record: null,
      zIndex: 101,
      group: "flowchart_endpoint"
    };
    return sprite;
  }
});

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
Ext.define('Jhako.draw.Connector', {
  constructor: function(record, prev_job, next_job) {
    return JhakoDrawConnector(record, prev_job, next_job, false);
  }
});

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
Ext.define('Jhako.draw.ConnectorView', {
  constructor: function(record, prev_job, next_job) {
    return JhakoDrawConnector(record, prev_job, next_job, true);
  }
});

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
Ext.define('Jhako.draw.Job', {
  constructor: function(record) {
    return JhakoDrawJob(record, false);
  }
});

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
Ext.define('Jhako.draw.JobView', {
  constructor: function(record) {
    return JhakoDrawJob(record, true);
  }
});
