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

class SchedulesController < ApplicationController
  before_action :logged_in_user
  before_action :power_user

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
    jobunit = Jobunit.find(params[:jobunit_id])
    schedules = jobunit.schedules
    render :json => schedules.as_json
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
    schedule = Schedule.find(params[:id])
    render :json => schedule.as_json(:root => true)
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
      :message => "Created a schedule"
    }
    failure_msg = {
      :success => false,
      :message => "Failed to create a schedule"
    }
    jobunit = Jobunit.find(params[:jobunit_id])
    schedule = jobunit.schedules.create(schedule_params)
    if schedule.save
      success_msg[:id] = schedule.id
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
      :message => "Updated a schedule"
    }
    failure_msg = {
      :success => false,
      :message => "Failed to update a schedule"
    }
    schedule = Schedule.find(params[:id])
    if schedule.update_attributes(schedule_params)
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
      :message => "Destroyed a schedule"
    }
    failure_msg = {
      :success => false,
      :message => "Failed to destroy a schedule"
    }
    schedule = Schedule.find(params[:id])
    if schedule.destroy
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
    def schedule_params
      params.require(:schedule).permit(:week, :month, :day, :hour, :minute)
    end

end
