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

class SessionsController < ApplicationController
#################################################################################
#
# Function:
#
# Purpose:
#
# Parameters:
#
# Return value:
#
# Author: Komatsu Yuji(Zheng Chuyu)
#
#################################################################################
  def show
    remember_token = User.encrypt(cookies[:remember_token])
    user ||= User.find_by(:remember_token => remember_token)
    if user.nil?
      redirect_to root_path
      return
    end
    data = {
      :session => user.as_json
    }
    render :json => data
  end

#################################################################################
#
# Function:
#
# Purpose:
#
# Parameters:
#
# Return value:
#
# Author: Komatsu Yuji(Zheng Chuyu)
#
#################################################################################
  def create
    user = User.find_by(:name => params[:session][:name])
    if user && user.authenticate(params[:session][:password])
      login(user)
      render :json => {
        :success => true,
        :id => user.id,
        :message => "Login '#{user.name}'"
      }
    else
      render :json => {
        :success => false,
        :message => "Invalid username/password combination"
      }
    end
  end

#################################################################################
#
# Function:
#
# Purpose:
#
# Parameters:
#
# Return value:
#
# Author: Komatsu Yuji(Zheng Chuyu)
#
#################################################################################
  def destroy
    logout
    render :json => {
      :success => true,
      :message => "Logout"
    }
  end
end
