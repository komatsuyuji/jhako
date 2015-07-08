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

class CreateJobunits < ActiveRecord::Migration
  def change
    create_table :jobunits do |t|
      t.string :name, :null => false
      t.string :description, :null => false, :default => ''
      t.integer :kind, :null => false, :default => 0
      t.integer :parent_id, :null => false, :default => 0, :limit => 8
      t.integer :x, :null => false, :default => 0
      t.integer :y, :null => false, :default => 0
      t.boolean :hold, :null => false, :default => false
      t.boolean :skip, :null => false, :default => false
      t.integer :timeout, :null => false, :default => 0
      t.timestamps :null => false

      t.index [:name, :parent_id], :unique => true
      t.index :kind
    end
  end
end
