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

class CreateProcSshjobs < ActiveRecord::Migration
  def change
    create_table :proc_sshjobs do |t|
      t.integer :proc_jobunit_id, :null => false, :default => 0, :limit => 8
      t.string :host, :null => false, :default => ''
      t.integer :port, :null => false, :default => 22
      t.integer :authtype, :null => false, :default => 0
      t.string :username, :null => false, :default => ''
      t.string :password, :null => false, :default => ''
      t.text :privatekey
      t.text :command

      t.index :proc_jobunit_id
    end
  end
end
