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

class CreateUsers < ActiveRecord::Migration
  def change
    create_table :users do |t|
      t.string :name, :null => false
      t.string :fullname, :null => false, :default => ''
      t.integer :kind, :null => false, :default => 0
      t.string :password_digest, :null => false, :default => ''
      t.string :remember_token, :null => false, :default => ''
      t.string :email, :null => false, :default => ''
      t.string :language, :null => false, :default => 'en_GB'
      t.timestamps :null => false

      t.index :name, :unique => true
      t.index :remember_token
    end
  end
end
