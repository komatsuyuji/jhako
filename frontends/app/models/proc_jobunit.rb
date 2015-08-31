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

class ProcJobunit < ActiveRecord::Base
  # parent and children
  belongs_to :parent, :class_name => "ProcJobunit"
  has_many :children, :class_name => "ProcJobunit", :foreign_key => "parent_id", :dependent => :destroy

  # rootjobnet
  has_one :rootjobnet, :class_name => "ProcRootjobnet", :dependent => :destroy
  has_many :schedules, :class_name => "ProcSchedule", :dependent => :destroy
  has_many :alarms, :class_name => "ProcAlarm", :dependent => :destroy

  # subjobnet
  has_many :connectors, :class_name => "ProcConnector", :dependent => :destroy

  # variables
  has_many :variables, :class_name => "ProcVariable", :autosave => true, :dependent => :destroy

  # sleepjob
  has_one :sleepjob, :class_name => "ProcSleepjob", :dependent => :destroy

  # clockjob
  has_one :clockjob, :class_name => "ProcClockjob", :dependent => :destroy

  # datejob
  has_one :datejob, :class_name => "ProcDatejob", :dependent => :destroy
  has_many :dateconds, :class_name => "ProcDatecond", :dependent => :destroy

  # varjob vardatum
  has_many :vardata, :class_name => "ProcVardatum", :dependent => :destroy

  # sshjob
  has_one :sshjob, :class_name => "ProcSshjob", :dependent => :destroy

  # winjob
  has_one :winjob, :class_name => "ProcWinjob", :dependent => :destroy

  # conditions
  has_many :conditions, :class_name => "ProcCondition", :dependent => :destroy

  # emailjob
  has_one :emailjob, :class_name => "ProcEmailjob", :dependent => :destroy

  # jobresults
  has_many :jobresults, :class_name => "ProcJobresult", :dependent => :destroy

  # execlogs
  has_many :execlogs, :class_name => "ProcExeclog", :dependent => :destroy
end
