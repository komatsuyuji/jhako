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
Ext.define('Jhako.view.common.Datecond', {
  alias: 'widget.commonDatecond',
  extend: 'Ext.grid.Panel',
  title: I18n.t('views.model.datecond'),
  itemId: 'comm_datecond',
  columns: {
    items: [{
      text: I18n.t('views.column.kind'),
      dataIndex: 'kind',
      width: 100,
      editor: {
        xtype: 'combobox',
        store: DATECOND_KIND_DATA,
        editable: false,
        allowBlank: false,
      },
      renderer: function(value) {
        return JhakoFindRecord(DATECOND_KIND_DATA, value);
      }
    }, {
      text: I18n.t('views.condition.cond'),
      dataIndex: 'cond',
      width: 100,
      getEditor: function(record) {
        switch (record.data.kind) {
          case DATECOND_KIND_EVERY_WEEK:
          case DATECOND_KIND_FIRST_WEEK:
          case DATECOND_KIND_SECOND_WEEK:
          case DATECOND_KIND_THIRD_WEEK:
          case DATECOND_KIND_FOURTH_WEEK:
            {
              return Ext.create('Ext.grid.CellEditor', {
                field: Ext.create('Ext.form.field.ComboBox', {
                  editable: false,
                  allowBlank: false,
                  store: JHAKO_DAYOFWEEK_DATA
                })
              });
              break;
            }
          case DATECOND_KIND_EVERY_MONTH:
          case DATECOND_KIND_REVERSE_MONTH:
            {
              return Ext.create('Ext.grid.CellEditor', {
                field: Ext.create('Ext.form.field.ComboBox', {
                  editable: false,
                  allowBlank: false,
                  store: JHAKO_DAY_DATA
                })
              });
              break;
            }
          default:
            {
              break;
            }
        }
        return null;
      },
      renderer: function(value, metaData, record) {
        switch (record.data.kind) {
          case DATECOND_KIND_EVERY_WEEK:
          case DATECOND_KIND_FIRST_WEEK:
          case DATECOND_KIND_SECOND_WEEK:
          case DATECOND_KIND_THIRD_WEEK:
          case DATECOND_KIND_FOURTH_WEEK:
            {
              return JhakoFindRecord(JHAKO_DAYOFWEEK_DATA, value);
              break;
            }
          case DATECOND_KIND_EVERY_MONTH:
          case DATECOND_KIND_REVERSE_MONTH:
            {
              return value;
              break;
            }
          default:
            {
              return '';
              break;
            }
        }
      }
    }, {
      text: I18n.t('views.model.calendar'),
      dataIndex: 'calendar_id',
      width: 100,
      getEditor: function(record) {
        if (record.data.kind == DATECOND_KIND_CALENDAR) {
          return Ext.create('Ext.grid.CellEditor', {
            field: Ext.create('Ext.form.field.ComboBox', {
              editable: false,
              allowBlank: false,
              displayField: 'name',
              valueField: 'id',
              store: 'Calendar'
            })
          });
        }
        return null;
      },
      renderer: function(value, metaData, record) {
        if (record.data.kind == DATECOND_KIND_CALENDAR) {
          var store = Jhako.app.getStore('Calendar');
          var rec = store.findRecord('id', value);
          if (value) {
            return rec.data.name;
          }
        }
        return '';
      }
    }, {
      xtype: 'checkcolumn',
      text: I18n.t('views.condition.negative'),
      dataIndex: 'negative',
      editable: false,
      width: 70,
    }]
  },
  plugins: [{
    ptype: 'cellediting',
    clicksToEdit: 1,
    pluginId: 'cellediting_datecond'
  }],
  bbar: [{
    text: I18n.t('views.button.add'),
    icon: location.pathname + '/images/icons/add.png',
    disabled: true,
    action: 'add_datecond'
  }, '-', {
    text: I18n.t('views.button.delete'),
    icon: location.pathname + '/images/icons/delete.png',
    disabled: true,
    action: 'delete_datecond'
  }, '->', {
    text: I18n.t('views.button.clear'),
    icon: location.pathname + '/images/icons/bin_closed.png',
    disabled: true,
    action: 'clear_datecond'
  }],
  initComponent: function() {
    this.callParent();
  }
});