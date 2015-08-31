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

class CalendarDatesController < ApplicationController
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
    calendar = Calendar.find(params[:calendar_id])
    calendar_dates = calendar.calendar_dates
    render :json => calendar_dates.as_json
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
    calendar_date = CalendarDate.find(params[:id])
    render :json => calendar_date.as_json(:root => true)
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
      :message => "Created a calendar date'"
    }
    failure_msg = {
      :success => false,
      :message => "Failed to create a calendar date"
    }
    calendar = Calendar.find(params[:calendar_id])
    calendar_date = calendar.calendar_dates.create(calendar_date_params)
    if calendar_date.save
      success_msg[:id] = calendar_date.id
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
      :message => "Updated a calendar date"
    }
    failure_msg = {
      :success => false,
      :message => "Failed to update a calendar date"
    }
    calendar_date = CalendarDate.find(params[:id])
    if calendar_date.update_attributes(calendar_date_params)
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
      :message => "Destroyed a calendar date"
    }
    failure_msg = {
      :success => false,
      :message => "Failed to destroy a calendar date"
    }
    calendar_date = CalendarDate.find(params[:id])
    if calendar_date.destroy
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
    def calendar_date_params
      params.require(:calendar_date).permit(:caledate)
    end

end
