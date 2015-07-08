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

class CreateConfigures < ActiveRecord::Migration
  def change
    create_table :configures do |t|
      t.integer :end_period, :null => false, :default => 1
      t.integer :error_period, :null => false, :default => 24
      t.integer :outschedule_period, :null => false, :default => 1
      t.integer :carryover_period, :null => false, :default => 1
      t.integer :forcestop_period, :null => false, :default => 1
      t.integer :history_period, :null => false, :default => 30
    end
  end
end
