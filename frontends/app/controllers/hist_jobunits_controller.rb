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

class HistJobunitsController < ApplicationController
  before_action :logged_in_user

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
    hist_jobunits = search(HistJobunit.where(:parent_id => parent_id), params[:filter], HistJobunit)
    count = hist_jobunits.count
    hist_jobunits = sort(hist_jobunits, params[:sort], HistJobunit)
    if !(params[:page].nil? || params[:limit].nil?)
      hist_jobunits = hist_jobunits.paginate(:page => params[:page], :per_page => params[:limit])
    end
    data = {
      :total_count => count,
      :hist_jobunits => hist_jobunits
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
    hist_jobunit = HistJobunit.find(params[:id])
    hist_jobunit.include_root_in_json = true
    case hist_jobunit.kind
    when 0..9 then
      # jobgroup
      data = hist_jobunit.to_json()
    when 10..19 then
      # rootjobnet
      data = hist_jobunit.to_json(:include => [:rootjobnet, :schedules, :children, :connectors, :variables])
    when 20..99 then
      # subjobnet
      data = hist_jobunit.to_json(:include => [:children, :connectors, :variables])
    when 100, 101, 102 then
      # startjob, endjob, mergejob
      data = hist_jobunit.to_json()
    when 103 then
      # sleepjob
      data = hist_jobunit.to_json(:include => [:sleepjob])
    when 104 then
      # clockjob
      data = hist_jobunit.to_json(:include => [:clockjob])
    when 105 then
      # datejob
      data = hist_jobunit.to_json(:include => [:datejob, :dateconds])
    when 106 then
      # varjob
      data = hist_jobunit.to_json(:include => [:vardata])
    when 200 then
      # sshjob
      data = hist_jobunit.to_json(:include => [:sshjob, :conditions, :jobresults])
    when 201 then
      # winjob
      data = hist_jobunit.to_json(:include => [:winjob, :conditions, :jobresults])
    else
      data = hist_jobunit.to_json()
    end
    render :json => data
  end

end
