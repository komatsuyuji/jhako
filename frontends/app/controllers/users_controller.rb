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

class UsersController < ApplicationController
  before_action :logged_in_user
  before_action :admin_user, :only => [:index, :create, :destroy]

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
  def index
    users = search(User.all, params[:filter], User)
    count = users.count
    users = sort(users, params[:sort], User)
    if !(params[:page].nil? || params[:limit].nil?)
      users = users.paginate(:page => params[:page], :per_page => params[:limit])
    end
    data = {
      :total_count => count,
      :users => users.as_json
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
  def show
    user = User.find(params[:id])
    if current_user.kind != 9 && current_user.id != user.id
      redirect_to root_path
      return
    end 

    render :json => user.as_json(:root => true)
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
    success_msg = {
      :success => true,
      :message => "Created a user"
    }
    failure_msg = {
      :success => false,
      :message => "Failed to create a user"
    }
    user = User.new(user_params)
    if user.save
      success_msg[:id] = user.id
      render :json => success_msg
    else 
      render :json => failure_msg
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
  def update
    success_msg = {
      :success => true,
      :message => "Updated a user"
    }
    failure_msg = {
      :success => false,
      :message => "Failed to update a user"
    }

    user = User.find(params[:id])
    if current_user.kind != 9 && current_user.id != user.id
      redirect_to root_path
      return
    end 

    user.attributes = user_params
    if current_user.id == user.id
      user.name = current_user.name
      user.kind = current_user.kind
    end

    if user.save
      render :json => success_msg
    else 
      render :json => failure_msg
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
    id = (params[:id] == nil) ? 0 : params[:id].to_i
    if id == 1
      render :json => {
        :success => false,
        :message => "Can not destroy admin user"
      }
      return 
    end

    success_msg = {
      :success => true,
      :message => "Destroyed a user"
    }
    failure_msg = {
      :success => false,
      :message => "Failed to destroy a user"
    }
    user = User.find(params[:id])
    if user.destroy
      render :json => success_msg
    else 
      render :json => failure_msg
    end
  end

  private
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
    def user_params
      id = (params[:id] == nil) ? 0 : params[:id].to_i
      if id == 1
        params.require(:user).permit(:password, :password_confirmation, :language, :email)
      else 
        params.require(:user).permit(:name, :fullname, :kind, :password, :password_confirmation, :language, :email)
      end
    end
end
