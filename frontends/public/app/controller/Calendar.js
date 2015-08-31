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

Ext.define('Jhako.controller.Calendar', {
  extend: 'Ext.app.Controller',
  refs: [{
    ref: 'calendarList',
    selector: 'calendarList'
  }, {
    ref: 'calendarDetail',
    selector: 'calendarDetail'
  }, {
    ref: 'calendarEdit',
    selector: 'calendarEdit'
  }, {
    ref: 'calendarDate',
    selector: 'calendarDate'
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
      'calendarList': {
        cellclick: this.onCellclick,
      },
      'calendarIndex button[action=clear_filter]': {
        click: this.onClearfilter,
      },
      'calendarIndex button[action=add_calendar]': {
        click: this.onAddCalendar,
      },
      'calendarIndex button[action=delete_calendar]': {
        click: this.onDeleteCalendar,
      },
      'calendarIndex button[action=save_calendar]': {
        click: this.onSaveCalendar,
      },
      'calendarIndex button[action=cancel_calendar]': {
        click: this.onCancelCalendar,
      },
      'calendarIndex button[action=clear_calendar]': {
        click: this.onClearCalendar,
      },
      'calendarEdit numberfield[name=selectedyear]': {
        change: this.onChangeSelectedyear,
      },
      'calendarDate': {
        afterrender: this.onAfterrenderDate,
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
  onCellclick: function(grid, td, cellIndex, record, tr, rowIndex, e) {
    if (!record)
      return;

    var me = this;
    var panel = me.getCalendarDetail();
    var form = me.getCalendarEdit();
    var field = form.child('#selectedyear');
    var store = grid.getStore();

    panel.setDisabled(true);
    form.getForm().reset(true);
    field.setValue(JHAKO_THIS_YEAR);

    if (!record.data.id) {
      store.reload();
      return;
    }

    Jhako.model.Calendar.load(record.data.id, {
      failure: function(record, operation) {},
      success: function(rec, operation) {
        panel.setDisabled(false);
        form.loadRecord(rec);
        if (rec['jhako.model.calendardatesStore']) {
          rec['jhako.model.calendardatesStore'].getProxy().url = location.pathname + '/calendars/' + record.data.id + '/calendar_dates';
          me.onLoadCalendarDate();
        }
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
  onClearfilter: function(button, e) {
    var me = this;
    var grid = me.getCalendarList();
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
  onAddCalendar: function(button, e) {
    var me = this;
    var grid = me.getCalendarList();
    var panel = me.getCalendarDetail();
    var form = me.getCalendarEdit();
    var field = form.child('#selectedyear');
    var store = grid.getStore();
    var cnt = store.getCount();

    var record = new Jhako.model.Calendar();
    record['jhako.model.calendardatesStore'] = new Ext.data.Store({
      model: 'Jhako.model.CalendarDate',
    });

    store.insert(cnt, record);
    grid.getSelectionModel().select(record);
    field.setValue(JHAKO_THIS_YEAR);
    panel.setDisabled(false);
    form.loadRecord(record);
    me.onLoadCalendarDate();
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
  onDeleteCalendar: function(button, e) {
    var me = this;
    var grid = me.getCalendarList();
    var panel = me.getCalendarDetail();
    var form = me.getCalendarEdit();
    var store = grid.getStore();
    var record = form.getRecord();

    if (!record)
      return;

    Ext.Msg.show({
      title: I18n.t('views.title.delete'),
      msg: I18n.t('views.msg.delete_calender', {
        name: record.data.name
      }),
      buttons: Ext.Msg.YESNO,
      icon: Ext.Msg.QUESTION,
      fn: function(btn) {
        if (btn == "yes") {
          store.remove(record);
          store.sync({
            success: function(records, operation) {
              store.reload();
              panel.setDisabled(true);
              form.getForm().reset(true);
              var combo_store = Jhako.app.getStore('CalendarCombo');
              combo_store.reload();
              me.onLoadCalendarDate();
            }
          });
        }
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
  onSaveCalendar: function(button, e) {
    var me = this;
    var grid = me.getCalendarList();
    var form = me.getCalendarEdit();
    var store = grid.getStore();
    form.updateRecord();
    var record = form.getRecord();
    if (!record)
      return;
    record.save({
      success: function(rec, operation) {
        if (!rec.data.id) {
          var data = Ext.JSON.decode(operation.response.responseText);
          rec.setId(data.id);
          rec['jhako.model.calendardatesStore'].getProxy().url = location.pathname + '/calendars/' + rec.data.id + '/calendar_dates';
        }
        rec['jhako.model.calendardatesStore'].sync();

        store.reload();
        form.loadRecord(rec);
        var combo_store = Jhako.app.getStore('CalendarCombo');
        combo_store.reload();
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
  onCancelCalendar: function(button, e) {
    var me = this;
    var grid = me.getCalendarList();
    var panel = me.getCalendarDetail();
    var form = me.getCalendarEdit();
    var field = form.child('#selectedyear');
    var record = form.getRecord();

    panel.setDisabled(true);
    form.getForm().reset(true);
    field.setValue(JHAKO_THIS_YEAR);

    if (!record.data.id) {
      grid.getStore().reload();
      return;
    }

    Jhako.model.Calendar.load(record.data.id, {
      success: function(rec, operation) {
        panel.setDisabled(false);
        form.loadRecord(rec);
        if (rec['jhako.model.calendardatesStore']) {
          rec['jhako.model.calendardatesStore'].getProxy().url = location.pathname + '/calendars/' + record.data.id + '/calendar_dates';
          me.onLoadCalendarDate();
        }
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
  onClearCalendar: function(button, e) {
    var me = this;
    var form = me.getCalendarEdit();
    var record = form.getRecord();
    if (record['jhako.model.calendardatesStore']) {
      record['jhako.model.calendardatesStore'].removeAll();
    }
    me.onLoadCalendarDate();
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
  onChangeSelectedyear: function(field, year) {
    if (year < 2000)
      return;
    var me = this;
    var panel = me.getCalendarDate();
    panel.items.each(function(datepicker, index, len) {
      var mindate = new Date(year, index, 1);
      if (new Date().getTime() >= mindate.getTime()) mindate = new Date();
      datepicker.setValue(new Date(year, index, 1));
      datepicker.setMinDate(mindate);
      datepicker.setMaxDate(new Date(year, index + 1, 0));
    });
    me.onLoadCalendarDate();
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
  onAfterrenderDate: function(panel) {
    if (!panel) return;
    var me = this;
    var onselectdate = me.onSelectDate;

    panel.items.each(function(datepicker, index, len) {
      var year = JHAKO_THIS_YEAR;
      var mindate = new Date(year, index, 1);
      if (new Date().getTime() >= mindate.getTime())
        mindate = new Date();
      datepicker.setValue(new Date(year, index, 1));
      datepicker.setMinDate(mindate);
      datepicker.setMaxDate(new Date(year, index + 1, 0));
      datepicker.removeManagedListener(datepicker.eventEl, 'mousewheel');
      datepicker.on('select', onselectdate);
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
  onSelectDate: function(datepicker, date, eOpts) {
    var ctrl = Jhako.app.getController('Calendar');
    var form = ctrl.getCalendarEdit();
    var record = form.getRecord();
    var store = record['jhako.model.calendardatesStore'];

    var gettime = date.getTime();
    datepicker.cells.each(function(cell, index, len) {
      if (cell.dom.firstChild.dateValue == gettime) {
        var num = store.find('caledate', date);
        if (num >= 0) {
          store.removeAt(num);
        } else {
          store.add({
            caledate: date
          });
        }
      }
    });

    ctrl.onLoadCalendarDate();
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
  onLoadCalendarDate: function() {
    var me = this;
    var form = me.getCalendarEdit();
    var record = form.getRecord();
    var store = null;
    if (record)
      store = record['jhako.model.calendardatesStore'];

    var pnl = me.getCalendarDate();
    pnl.items.each(function(datepicker, i) {
      datepicker.cells.each(function(cell, j) {
        if (!store) {
          cell.removeCls('x-datepicker-selected');
          return;
        }

        var celldate = new Date(cell.dom.firstChild.dateValue);
        if (store.find('caledate', celldate) >= 0) {
          cell.addCls('x-datepicker-selected');
        } else {
          cell.removeCls('x-datepicker-selected');
        }
      });
    });
  },

});