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
Ext.define('Jhako.view.calendar.Index', {
  alias: 'widget.calendarIndex',
  extend: 'Ext.panel.Panel',
  title: I18n.t('views.model.calendar'),
  layout: 'border',
  disabled: true,
  bbar: [{
    xtype: 'pagingtoolbar',
    store: 'Calendar',
    displayInfo: true
  }, '-', {
    text: I18n.t('views.button.clear_filter'),
    icon: location.pathname + '/images/icons/hourglass.png',
    action: 'clear_filter',
  }, '->', {
    text: I18n.t('views.button.add'),
    icon: location.pathname + '/images/icons/calendar_add.png',
    action: 'add_calendar'
  }, '-', {
    text: I18n.t('views.button.delete'),
    icon: location.pathname + '/images/icons/calendar_delete.png',
    action: 'delete_calendar'
  }, '-', {
    text: I18n.t('views.button.save'),
    icon: location.pathname + '/images/icons/disk.png',
    action: 'save_calendar'
  }, '-', {
    text: I18n.t('views.button.cancel'),
    icon: location.pathname + '/images/icons/cancel.png',
    action: 'cancel_calendar'
  }, '-', {
    text: I18n.t('views.button.clear'),
    icon: location.pathname + '/images/icons/bin_closed.png',
    action: 'clear_calendar'
  }],

  items: [{
    xtype: 'calendarList',
    region: 'west',
    width: 400
  }, {
    xtype: 'calendarDetail',
    region: 'center'
  }],
  initComponent: function() {
    this.callParent();
  }
});