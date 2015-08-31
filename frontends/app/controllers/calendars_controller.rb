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

class CalendarsController < ApplicationController
  before_action :logged_in_user
  before_action :resource_user

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
    calendars = search(Calendar.all, params[:filter], Calendar)
    count = calendars.count
    calendars = sort(calendars, params[:sort], Calendar)
    if !(params[:page].nil? || params[:limit].nil?)
      calendars = calendars.paginate(:page => params[:page], :per_page => params[:limit])
    end
    data = {
      :total_count => count,
      :calendars => calendars.as_json
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
    calendar = Calendar.find(params[:id])
    render :json => calendar.as_json(:root => true, :include => [:calendar_dates])
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
      :message => "Created a calendar"
    }
    failure_msg = {
      :success => false,
      :message => "Failed to create a calendar"
    }
    calendar = Calendar.new(calendar_params)
    if calendar.save
      success_msg[:id] = calendar.id
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
      :message => "Updated a calendar"
    }
    failure_msg = {
      :success => false,
      :message => "Failed to update a calendar"
    }
    calendar = Calendar.find(params[:id])
    if calendar.update_attributes(calendar_params)
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
    success_msg = {
      :success => true,
      :message => "Destroyed a calendar"
    }
    failure_msg = {
      :success => false,
      :message => "Failed to destroy a calendar"
    }
    calendar = Calendar.find(params[:id])
    if calendar.destroy
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
    def calendar_params
      params.require(:calendar).permit(:name, :description, :kind)
    end
end
