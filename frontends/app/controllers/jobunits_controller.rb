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

class JobunitsController < ApplicationController
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
    # get the children of jobgroup or jobnet
    parent_id = (params[:node] == nil) ? 0 : params[:node].to_i
    jobunits = search(Jobunit.where(:parent_id => parent_id), params[:filter], Jobunit)
    count = jobunits.count
    jobunits = sort(jobunits, params[:sort], Jobunit)
    if !(params[:page].nil? || params[:limit].nil?)
      jobunits = jobunits.paginate(:page => params[:page], :per_page => params[:limit])
    end
    data = {
      :total_count => count,
      :jobunits => jobunits
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
    id = (params[:id] == nil) ? 0 : params[:id].to_i
    export = (params[:export] == nil) ? 0 : params[:export].to_i
    
    if id == 0
      jobunit = Jobunit.new({
        :id => 0,
        :kind => 0,
        :name => '/'
      })
      filename = 'jhako.json'
    else
      jobunit = Jobunit.find(id)
      filename = jobunit.name + '.json'
    end

    data = export(jobunit, export)
    if export == 0
      render :json => data
    else
      send_data data, :filename => filename
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
  def create
    success_msg = {
      :success => true,
      :message => "Created a jobunit"
    }
    failure_msg = {
      :success => false,
      :message => "Failed to create a jobunit"
    }

    # import params
    import = (params[:import] == nil) ? 0 : params[:import].to_i
    parent_id = (params[:parent_id] == nil) ? 0 : params[:parent_id].to_i
    upload_file = params[:file]

    # Case 1: import 
    if import == 1 
      if upload_file.nil?
        render :json => failure_msg
        return
      end

      # parse upload_file
      begin
        data = JSON.parse(upload_file.read, {:max_nesting => 0})
      rescue
        render :json => failure_msg
        return
      end
   
      # check upload data
      if data['jobunit'].nil?
        render :json => failure_msg
        return
      end

      # check upload path
      kind = data['jobunit']['kind']
      if parent_id > 0
        parent = Jobunit.find(parent_id)

        # can not upload jobgroup under jobnet
        if parent.kind >= 10 && kind < 10
          render :json => failure_msg
          return
        end

        if parent.kind < 10 && kind >= 20 && kind < 100
          # change jobnet to rootjobnet
          data['jobunit']['kind'] = 10
          data['jobunit']['x'] = 0
          data['jobunit']['y'] = 0
        elsif parent.kind >= 10 && parent.kind < 100 && kind >= 10 && kind < 20
          # change rootjobnet to jobnet
          data['jobunit']['kind'] = 20
        end
      else
        # can not upload jobnet under root path
        if kind >= 10
          render :json => failure_msg
          return
        end
      end

      # change name '/' to 'root'
      if data['jobunit']['name'] == '/'
        data['jobunit']['name'] = 'root'
      end

      # json_data -> params
      json_params = ActionController::Parameters.new(data)
      id = import(json_params.require(:jobunit), parent_id)
      if id == 0
        render :json => failure_msg
      else
        render :json => success_msg
      end
      return
    end

    # Case 2: create a jobunit
    jobunit = Jobunit.new(jobunit_params(params.require(:jobunit)))

    # can not create jobnet under root path
    if jobunit.kind >= 10 && jobunit.parent_id <= 0
      render :json => failure_msg
      return
    end

    case jobunit.kind
    when 0..9 then
      # jobgroup
    when 10..19 then
      # rootjobnet
      jobunit.build_rootjobnet
    when 20..99 then
      # subjobnet
    when 100, 101 then
      # startjob, endjob
      render :json => failure_msg
      return
    when 102 then
      # mergejob
    when 103 then
      # sleepjob
      jobunit.build_sleepjob
    when 104 then
      # clockjob
      jobunit.build_clockjob
    when 105 then
      # datejob
      jobunit.build_datejob
    when 106 then
      # varjob
    when 200 then
      # sshjob
      jobunit.build_sshjob
    when 201 then
      # winjob
      jobunit.build_winjob
    else
      render :json => failure_msg
      return
    end

    if !(jobunit.save)
      render :json => failure_msg
      return
    end

    success_msg[:id] = jobunit.id
    # it is not a jobnet
    if !(jobunit.kind >= 10 && jobunit.kind < 100)
      render :json => success_msg
      return
    end

    # create startjob for the new jobnet 
    startjob = Jobunit.new({
      :name => 'start',
      :kind => 100,
      :parent_id => jobunit.id,
      :x => 50,
      :y => 70
    })
    if !(startjob.save)
      jobunit.destroy
      render :json => failure_msg
      return
    end

    # create endjob for the new jobnet 
    endjob = Jobunit.new({
      :name => 'end',
      :kind => 101,
      :parent_id => jobunit.id,
      :x => 450,
      :y => 70
    })
    if !(endjob.save)
      jobunit.destroy
      render :json => failure_msg
      return
    end

    # create a connecctor startjob-endjob for the new jobnet 
    if !(jobunit.connectors.create({:prev_jobid => startjob.id, :next_jobid => endjob.id}))
      jobunit.destroy
      render :json => failure_msg
      return
    end
    render :json => success_msg
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
      :message => "Updated a jobunit"
    }
    failure_msg = {
      :success => false,
      :message => "Failed to update a jobunit"
    }

    jobunit = Jobunit.find(params[:id])
    kind = jobunit.kind
    jobunit.attributes = jobunit_params(params.require(:jobunit))

    # subjobnet -> rootjobnet
    if kind >= 20 && kind < 100 && jobunit.kind >= 10 && jobunit.kind < 20
      jobunit.build_rootjobnet
    end

    if jobunit.save
      # rootjobnet -> subjobnet
      if kind >= 10 && kind < 20 && jobunit.kind >= 20 && jobunit.kind < 100
        jobunit.rootjobnet.destroy
        jobunit.schedules.destroy
      end
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
      :message => "Destroyed a jobunit"
    }
    failure_msg = {
      :success => false,
      :message => "Failed to destroy a jobunit"
    }
    jobunit = Jobunit.find(params[:id])
    if jobunit.kind == 100 || jobunit.kind == 101
      render :json => failure_msg
      return
    end
    if jobunit.destroy
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
  def export(jobunit, flag)
    # add root tag
    if flag == 0
      jobunit.include_root_in_json = true
    end

    case jobunit.kind
    when 0..9 then
      # jobgroup
      if flag == 0
        data = jobunit.to_json(:include => [:children])
      else
        data = jobunit.to_json()
      end
    when 10..19 then
      # rootjobnet
      if flag == 0
        data = jobunit.to_json(:include => [:rootjobnet, :schedules, :connectors, :children])
      else
        data = jobunit.to_json(:include => [:rootjobnet, :schedules, :connectors])
      end
    when 20..99 then
      # jobnet
      if flag == 0
        data = jobunit.to_json(:include => [:connectors, :children])
      else
        data = jobunit.to_json(:include => [:connectors])
      end
    when 100, 101, 102 then
      # startjob, endjob, mergejob
      data = jobunit.to_json()
    when 103 then
      # sleepjob
      data = jobunit.to_json(:include => [:sleepjob])
    when 104 then
      # clockjob
      data = jobunit.to_json(:include => [:clockjob])
    when 105 then
      # datejob
      data = jobunit.to_json(:include => [:datejob, :dateconds])
    when 106 then
      # varjob
      data = jobunit.to_json(:include => [:vardata])
    when 200 then
      # sshjob
      data = jobunit.to_json(:include => [:sshjob, :conditions])
    when 201 then
      # winjob
      data = jobunit.to_json(:include => [:winjob, :conditions])
    else
      data = jobunit.to_json()
    end

    if flag > 0 && jobunit.kind >= 0 && jobunit.kind < 100
      data = data.sub(/\}$/, ',"children":[')
      str = ''
      jobunit.children.each do |child|
        if str != ''
          str << ','
        end
        str << export(child, 2)
      end
      data << str << ']}'
    end

    if flag == 1
      data = '{"jobunit":' + data + '}'
    end
    return data
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
  def import(data, parent_id)
    if data.nil?
      return 0
    end

    jobunit = Jobunit.new(jobunit_params(data))
    jobunit.parent_id = parent_id

    case jobunit.kind
    when 0..9 then
      # jobgroup
    when 10..19 then
      # rootjobnet
      jobunit.build_rootjobnet
      if !(data[:rootjobnet].nil?)
        jobunit.rootjobnet.attributes = rootjobnet_params(data)
      end
      jobunit.rootjobnet.disabled = true
      jobunit.rootjobnet.user_id = 0
      jobunit.schedules.build(schedules_params(data)[:schedules])
    when 20..99 then
      # subjobnet
    when 100, 101, 102 then
      # startjob, endjob, mergejob
    when 103 then
      # sleepjob
      jobunit.build_sleepjob
      jobunit.sleepjob.attributes = sleepjob_params(data)
    when 104 then
      # clockjob
      jobunit.build_clockjob
      jobunit.clockjob.attributes = clockjob_params(data)
    when 105 then
      # datejob
      jobunit.build_datejob
      jobunit.datejob.attributes = datejob_params(data)
      jobunit.dateconds.build(dateconds_params(data)[:dateconds])
    when 106 then
      # varjob
      jobunit.vardata.build(vardata_params(data)[:vardata])
    when 200 then
      # sshjob
      jobunit.build_sshjob
      jobunit.sshjob.attributes = sshjob_params(data)
      jobunit.conditions.build(conditions_params(data)[:conditions])
    when 201 then
      # winjob
      jobunit.build_winjob
      jobunit.winjob.attributes = winjob_params(data)
      jobunit.conditions.build(conditions_params(data)[:conditions])
    else
      return 0
    end

    if !(jobunit.save)
      return 0
    end

    if jobunit.kind < 100
      if jobunit.kind >= 10
        old_conns = connectors_params(data)[:connectors]
        jobunit.connectors.build(old_conns)
      end
      data[:children].each do |child|
        id = import(child, jobunit.id)
        if id == 0
          jobunit.destroy
          return 0
        end
        if jobunit.kind >= 10
          renew_connectors(old_conns, jobunit.connectors, child[:id], id)
        end
      end
    end

    if !(jobunit.save)
      jobunit.destroy
      return 0
    end

    return jobunit.id
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
    def renew_connectors(old_conns, new_conns, old_id, new_id)
      old_conns.zip(new_conns).each do |old_conn, new_conn|
        if old_conn[:prev_jobid] == old_id
          new_conn.prev_jobid = new_id
        end
        if old_conn[:next_jobid] == old_id
          new_conn.next_jobid = new_id
        end
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
    def jobunit_params(input_params)
      input_params.permit(:name, :description, :kind, :parent_id, :x, :y, :hold, :skip, :timeout)
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
    def connectors_params(input_params)
      input_params.permit(:connectors =>[:kind, :prev_jobid, :next_jobid])
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
    def rootjobnet_params(input_params)
      input_params.require(:rootjobnet).permit(:user_id, :disabled, :active_since, :active_till, :calendar_id, :run_type, :delay_limit, :loader_interval)
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
    def schedules_params(input_params)
      input_params.permit(:schedules => [:week, :month, :day, :hour, :minute])
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
    def sleepjob_params(input_params)
      input_params.require(:sleepjob).permit(:hour, :minute, :second)
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
    def clockjob_params(input_params)
      input_params.require(:clockjob).permit(:basetime, :day, :hour, :minute, :delay_limit, :timeover)
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
    def datejob_params(input_params)
      input_params.require(:datejob).permit(:baseday, :beginning_of_week)
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
    def dateconds_params(input_params)
      input_params.permit(:dateconds =>[:kind, :calendar_id, :cond, :negative])
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
    def vardata_params(input_params)
      input_params.permit(:vardata => [:name, :value, :overwrite])
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
    def sshjob_params(input_params)
      input_params.require(:sshjob).permit(:host, :authtype, :user, :password, :port, :privatekey, :command)
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
    def winjob_params(input_params)
      input_params.require(:winjob).permit(:host, :user, :password, :scheme, :port, :path, :auth, :codepage, :command)
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
    def conditions_params(input_params)
      input_params.permit(:conditions =>[:method, :kind, :cond, :negative])
    end
end
