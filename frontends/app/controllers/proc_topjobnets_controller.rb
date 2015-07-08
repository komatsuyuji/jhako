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

class ProcTopjobnetsController < ApplicationController
  before_action :logged_in_user
  before_action :operator_user, :only => [:update]

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
    proc_topjobnets = search(ProcTopjobnet.all, params[:filter], ProcTopjobnet)
    count = proc_topjobnets.count
    proc_topjobnets = sort(proc_topjobnets, params[:sort], ProcTopjobnet)
    if !(params[:page].nil? || params[:limit].nil?)
      proc_topjobnets = proc_topjobnets.paginate(:page => params[:page], :per_page => params[:limit])
    end
    data = {
      :total_count => count,
      :proc_topjobnets => proc_topjobnets
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
  def summarize
    data = []
    Array(0...9).each do |status|
      dat = {}
      dat[:status] =  status
      dat[:hold] = ProcTopjobnet.where(["status = ? AND hold = true", status]).count
      dat[:skip] = ProcTopjobnet.where(["status = ? AND skip = true", status]).count
      dat[:timeout_occur] = ProcTopjobnet.where(["status = ? AND timeout_occur = true", status]).count
      dat[:force_start] = ProcTopjobnet.where(["status = ? AND force_start = true", status]).count
      dat[:force_stop] = ProcTopjobnet.where(["status = ? AND force_stop = true", status]).count
      dat[:total] = ProcTopjobnet.where(["status = ?", status]).count
      data.push(dat)
    end
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
    proc_topjobnet = ProcTopjobnet.find(params[:id])
    proc_topjobnet.include_root_in_json = true
    render :json => proc_topjobnet
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
      :message => "Created a process topjobnet"
    }
    failure_msg = {
      :success => false,
      :message => "Failed to create process topjobnet"
    }
    proc_topjobnet = ProcTopjobnet.new(proc_topjobnet_params)
    if proc_topjobnet.kind < 10 || proc_topjobnet.kind >= 100
      render :json => failure_msg
      return
    end

    if proc_topjobnet.save
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
      :message => "Updated a processs topjobnet"
    }
    failure_msg = {
      :success => false,
      :message => "Failed to update a process topjobnet"
    }
    proc_topjobnet = ProcTopjobnet.find(params[:id])
    if proc_topjobnet.update_attributes(proc_topjobnet_params)
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
    def proc_topjobnet_params
      params.require(:proc_topjobnet).permit(:name, :description, :kind, :hold, :skip, :timeout, :jobunit_id, :mode, :schedule_time, :run_type, :delay_limit, :force_start, :force_stop)
    end
end
