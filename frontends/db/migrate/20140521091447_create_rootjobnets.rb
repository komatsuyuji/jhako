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

class CreateRootjobnets < ActiveRecord::Migration
  def change
    create_table :rootjobnets do |t|
      t.integer :jobunit_id, :null => false, :default => 0, :limit => 8
      t.integer :user_id, :null => false, :default => 0, :limit => 8
      t.boolean :disabled, :null => false, :default => false
      t.datetime :active_since
      t.datetime :active_till
      t.integer :calendar_id, :null => false, :default => 0, :limit => 8
      t.integer :run_type, :null => false, :default => 0
      t.integer :delay_limit, :null => false, :default => 1
      t.integer :loader_interval, :null => false, :default => 5

      t.index :jobunit_id
      t.index :user_id
    end
  end
end
