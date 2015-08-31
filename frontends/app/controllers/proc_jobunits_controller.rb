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

class ProcJobunitsController < ApplicationController
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
    parent_id = params[:node].nil? ? 0 : params[:node].to_i
    proc_jobunits = search(ProcJobunit.where(:parent_id => parent_id), params[:filter], ProcJobunit)
    count = proc_jobunits.count
    proc_jobunits = sort(proc_jobunits, params[:sort], ProcJobunit)
    if !(params[:page].nil? || params[:limit].nil?)
      proc_jobunits = proc_jobunits.paginate(:page => params[:page], :per_page => params[:limit])
    end
    data = {
      :total_count => count,
      :proc_jobunits => proc_jobunits.as_json
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
    proc_jobunit = ProcJobunit.find(params[:id])

    case proc_jobunit.kind
    when 0..9 then
      # jobgroup
      data = proc_jobunit.as_json
    when 10..19 then
      # rootjobnet
      data = proc_jobunit.as_json(:include => [:rootjobnet, :schedules, :children, :connectors, :variables, :execlogs])
      data = data.merge({
        :alarms => proc_jobunit.alarms.as_json
      })
    when 20..99 then
      # subjobnet
      data = proc_jobunit.as_json(:include => [:children, :connectors, :variables, :execlogs])
    when 100, 101, 102 then
      # startjob, endjob, mergejob
      data = proc_jobunit.as_json(:include => [:execlogs])
    when 103 then
      # sleepjob
      data = proc_jobunit.as_json(:include => [:sleepjob, :execlogs])
    when 104 then
      # clockjob
      data = proc_jobunit.as_json(:include => [:clockjob, :execlogs])
    when 105 then
      # datejob
      data = proc_jobunit.as_json(:include => [:datejob, :dateconds, :execlogs])
    when 106 then
      # varjob
      data = proc_jobunit.as_json(:include => [:vardata, :execlogs])
    when 200 then
      # sshjob
      data = proc_jobunit.as_json(:include => [:sshjob, :conditions, :jobresults, :execlogs])
    when 201 then
      # winjob
      data = proc_jobunit.as_json(:include => [:winjob, :conditions, :jobresults, :execlogs])
    when 300 then
      # emailjob
      data = proc_jobunit.as_json(:include => [:emailjob, :execlogs])
    else
      data = proc_jobunit.as_json
    end

    proc_data = {
      :proc_jobunit => data
    }
    render :json => proc_data.to_json
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
      :message => "Updated a processs jobunit"
    }
    failure_msg = {
      :success => false,
      :message => "Failed to update a process jobunit"
    }
    proc_jobunit = ProcJobunit.find(params[:id])
    if proc_jobunit.update_attributes(proc_jobunit_params)
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
    def proc_jobunit_params
      params.require(:proc_jobunit).permit(:hold, :skip, :force_start, :force_stop)
    end

end
