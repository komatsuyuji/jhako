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
Ext.define('Jhako.view.calendar.Detail', {
  alias: 'widget.calendarDetail',
  extend: 'Ext.panel.Panel',
  icon: location.pathname + '/images/icons/calendar_view_month.png',
  title: I18n.t('views.title.setting'),
  layout: 'border',
  disabled: true,
  items: [{
    xtype: 'calendarEdit',
    region: 'north',
  }, {
    xtype: 'calendarDate',
    region: 'center'
  }],

  initComponent: function() {
    this.callParent();
  }
});