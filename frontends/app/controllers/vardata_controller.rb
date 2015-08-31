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

class VardataController < ApplicationController
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
    vardata = jobunit.vardata
    render :json => vardata.as_json
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
    vardatum = Vardatum.find(params[:id])
    render :json => vardatum.as_json(:root => true)
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
      :message => "Created a vardatum"
    }
    failure_msg = {
      :success => false,
      :message => "Failed to create a vardatum"
    }
    jobunit = Jobunit.find(params[:jobunit_id])
    vardatum = jobunit.vardata.create(vardatum_params)
    if vardatum.save
      success_msg[:id] = vardatum.id
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
      :message => "Updated a vardatum"
    }
    failure_msg = {
      :success => false,
      :message => "Failed to update a vardatum"
    }
    vardatum = Vardatum.find(params[:id])
    if vardatum.update_attributes(vardatum_params)
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
      :message => "Destroyed a vardatum"
    }
    failure_msg = {
      :success => false,
      :message => "Failed to destroy a vardatum"
    }
    vardatum = Vardatum.find(params[:id])
    if vardatum.destroy
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
    def vardatum_params
      params.require(:vardatum).permit(:name, :value, :overwrite)
    end
end
