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

class HistJobunit < ActiveRecord::Base
  # parent and children
  belongs_to :parent, :class_name => "HistJobunit"
  has_many :children, :class_name => "HistJobunit", :foreign_key => "parent_id", :dependent => :destroy

  # rootjobnet
  has_one :rootjobnet, :class_name => "HistRootjobnet", :dependent => :destroy
  has_many :schedules, :class_name => "HistSchedule", :dependent => :destroy
  has_many :alarms, :class_name => "HistAlarm", :dependent => :destroy

  # subjobnet
  has_many :connectors, :class_name => "HistConnector", :dependent => :destroy

  # variables
  has_many :variables, :class_name => "HistVariable", :dependent => :destroy

  # sleepjob
  has_one :sleepjob, :class_name => "HistSleepjob", :dependent => :destroy

  # clockjob
  has_one :clockjob, :class_name => "HistClockjob", :dependent => :destroy

  # datejob
  has_one :datejob, :class_name => "HistDatejob", :dependent => :destroy
  has_many :dateconds, :class_name => "HistDatecond", :dependent => :destroy

  # varjob vardatum
  has_many :vardata, :class_name => "HistVardatum", :dependent => :destroy

  # sshjob
  has_one :sshjob, :class_name => "HistSshjob", :dependent => :destroy

  # winjob
  has_one :winjob, :class_name => "HistWinjob", :dependent => :destroy

  # conditions
  has_many :conditions, :class_name => "HistCondition", :dependent => :destroy

  # emailjob
  has_one :emailjob, :class_name => "HistEmailjob", :dependent => :destroy

  # jobresult
  has_many :jobresults, :class_name => "HistJobresult", :dependent => :destroy
end
