#################################################################################
#
# jHako WebAPI
# Copyright (C) 2014-2015 Komatsu Yuji(Zheng Chuyu)
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
#################################################################################

class CreateSchedules < ActiveRecord::Migration
  def change
    create_table :schedules do |t|
      t.integer :jobunit_id, :null => false, :default => 0, :limit => 8
      t.string :week, :null => false, :default => ''
      t.string :month, :null => false, :default => ''
      t.string :day, :null => false, :default => ''
      t.string :hour, :null => false, :default => ''
      t.string :minute, :null => false, :default => ''

      t.index :jobunit_id
    end
  end
end
