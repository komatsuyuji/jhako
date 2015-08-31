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

class CreateEmailjobs < ActiveRecord::Migration
  def change
    create_table :emailjobs do |t|
      t.integer :jobunit_id, :null => false, :default => 0, :limit => 8
      t.string :host, :null => false, :default => '127.0.0.1'
      t.integer :port, :null => false, :default => 25
      t.boolean :auth, :null => false, :default => false
      t.string :username, :null => false, :default => ''
      t.string :password, :null => false, :default => ''
      t.string :mail_from, :null => false, :default => ''
      t.string :mail_to, :null => false, :default => ''
      t.string :subject, :null => false, :default => ''
      t.text :body

      t.index :jobunit_id
    end
  end
end
