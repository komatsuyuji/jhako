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

class CreateProcConnectors < ActiveRecord::Migration
  def change
    create_table :proc_connectors do |t|
      t.integer :proc_jobunit_id, :null => false, :default => 0, :limit => 8
      t.integer :kind, :null => false, :default => 0
      t.integer :prev_jobid, :null => false, :limit => 8
      t.integer :next_jobid, :null => false, :limit => 8

      t.index :proc_jobunit_id
      t.index :prev_jobid
    end
  end
end
