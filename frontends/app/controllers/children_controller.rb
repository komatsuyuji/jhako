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

class ChildrenController < ApplicationController
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
    children = jobunit.children
    render :json => children.as_json
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
    jobunit = Jobunit.find(params[:id])
    render :json => jobunit.as_json(:root => true)
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
      :message => "Inserted a tree"
    }
    failure_msg = {
      :success => false,
      :message => "Failed to insert a tree"
    }
    
    # get parent, jobunit
    parent_id = (params[:child][:parent_id] == nil) ? 0 : params[:child][:parent_id].to_i

    # copy the jobunit tree 
    jobunit = Jobunit.find(params[:id])
    jobunit.attributes = child_params
    jobunit_id = insert_tree(jobunit, parent_id)
    if jobunit_id == 0
      render :json => failure_msg
    else
      success_msg[:id] = jobunit_id
      render :json => success_msg
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
    def insert_tree(jobunit, parent_id)
      new_jobunit = jobunit.dup
      new_jobunit.parent_id = parent_id

      case jobunit.kind
      when 0..9 then
        # job group
      when 10..19 then
        # rootjobnet
        if jobunit.rootjobnet.nil?
          new_jobunit.build_rootjobnet
        else
          new_jobunit.rootjobnet = jobunit.rootjobnet.dup
        end
        new_jobunit.rootjobnet.disabled = true
        new_jobunit.rootjobnet.user_id = 0
        new_jobunit.schedules.build(jobunit.schedules.as_json(:except => ['id', 'jobunit_id']))
      when 20..99 then
        # jobnet
      when 100, 101, 102 then
        # startjob, endjob, mergejob
      when 103 then
        # sleepjob
        new_jobunit.sleepjob = jobunit.sleepjob.dup
      when 104 then
        # clockjob
        new_jobunit.clockjob = jobunit.clockjob.dup
      when 105 then
        # datejob
        new_jobunit.datejob = jobunit.datejob.dup
        new_jobunit.dateconds.build(jobunit.dateconds.as_json(:except => ['id', 'jobunit_id']))
      when 106 then
        # varjob
        new_jobunit.vardata.build(jobunit.vardata.as_json(:except => ['id', 'jobunit_id']))
      when 200 then
        # sshjob
        new_jobunit.sshjob = jobunit.sshjob.dup
        new_jobunit.conditions.build(jobunit.conditions.as_json(:except => ['id', 'jobunit_id']))
      when 201 then
        # winjob
        new_jobunit.winjob = jobunit.winjob.dup
        new_jobunit.conditions.build(jobunit.conditions.as_json(:except => ['id', 'jobunit_id']))
      else
        return 0
      end

      if !(new_jobunit.save)
        return 0
      end

      if jobunit.kind < 100
        # jobnet connectors
        if jobunit.kind >= 10
          new_jobunit.connectors.build(jobunit.connectors.as_json(:except => ['id', 'jobunit_id']))
        end

        # create children
        jobunit.children.each do |child|
          child_old_id = child.id
          child_new_id = insert_tree(child, new_jobunit.id)
          if child_new_id == 0
            return 0
          end

          # renew jobnet connectors
          if jobunit.kind >= 10
            renew_connectors(jobunit.connectors, new_jobunit.connectors, child_old_id, child_new_id)
          end
        end

        # save new jobunit
        if !(new_jobunit.save)
          return 0
        end
      end
      return new_jobunit.id
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
        if old_conn.prev_jobid == old_id
          new_conn.prev_jobid = new_id
        end
        if old_conn.next_jobid == old_id
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
    def child_params
      params.require(:child).permit(:name, :description, :kind, :parent_id, :x, :y, :hold, :skip, :timeout)
    end

end
