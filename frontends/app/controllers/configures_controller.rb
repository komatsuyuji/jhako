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

class ConfiguresController < ApplicationController
  before_action :logged_in_user
  before_action :admin_user

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
    configures = Configure.all
    data = {
      :total_count => Configure.all.count,
      :configures => configures
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
    configure = Configure.find(params[:id])
    configure.include_root_in_json = true
    render :json => configure.to_json()
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
    configure = Configure.find(params[:id])
    configure.attributes = configure_params

    success_msg = {
      :success => true,
    }
    failure_msg = {
      :success => false,
    }

    if configure.save
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
    def configure_params
      params.require(:configure).permit(:end_period, :error_period, :outschedule_period, :carryover_period, :forcestop_period, :history_period)
    end
end
