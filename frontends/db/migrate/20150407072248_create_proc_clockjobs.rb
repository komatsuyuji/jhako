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

class CreateProcClockjobs < ActiveRecord::Migration
  def change
    create_table :proc_clockjobs do |t|
      t.integer :proc_jobunit_id, :null => false, :default => 0, :limit => 8
      t.integer :basetime, :null => false, :default => 0
      t.datetime :predetermined_time
      t.integer :day, :null => false, :default => 0
      t.integer :hour, :null => false, :default => 0
      t.integer :minute, :null => false, :default => 0
      t.integer :timeover, :null => false, :default => 0
      t.integer :delay_limit, :null => false, :default => 1

      t.index :proc_jobunit_id
    end
  end
end
