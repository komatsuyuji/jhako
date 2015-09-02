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

class Jobunit < ActiveRecord::Base
  # parent and children
  belongs_to :parent, :class_name => "Jobunit"
  has_many :children, :class_name => "Jobunit", :foreign_key => "parent_id", :autosave => true, :dependent => :destroy

  # rootjobnet
  has_one :rootjobnet, :class_name => "Rootjobnet", :autosave => true, :dependent => :destroy
  has_many :schedules, :class_name => "Schedule", :autosave => true, :dependent => :destroy
  has_many :alarms, :class_name => "Alarm", :autosave => true, :dependent => :destroy

  # subjobnet
  has_many :connectors, :class_name => "Connector", :autosave => true, :dependent => :destroy

  # sleepjob
  has_one :sleepjob, :class_name => "Sleepjob", :autosave => true, :dependent => :destroy

  # clockjob
  has_one :clockjob, :class_name => "Clockjob", :autosave => true, :dependent => :destroy

  # datejob
  has_one :datejob, :class_name => "Datejob", :autosave => true, :dependent => :destroy
  has_many :dateconds, :class_name => "Datecond", :autosave => true, :dependent => :destroy

  # varjob vardatum
  has_many :vardata, :class_name => "Vardatum", :autosave => true, :dependent => :destroy

  # sshjob
  has_one :sshjob, :class_name => "Sshjob", :autosave => true, :dependent => :destroy

  # winjob
  has_one :winjob, :class_name => "Winjob", :autosave => true, :dependent => :destroy

  # conditions
  has_many :conditions, :class_name => "Condition", :autosave => true, :dependent => :destroy

  # emailjob
  has_one :emailjob, :class_name => "Emailjob", :autosave => true, :dependent => :destroy

  validates :name,
    :presence => true,
    :length => {:minimum => 3, :maximum => 255},
    :format => {:with => /\A[a-z]\w*\z/i},
    :uniqueness => {:scope => :parent_id}

  validates :description,
    :length => {:maximum => 255}

  def path
    if self.parent_id == 0
      return '/' + self.name
    end
    jobunit = Jobunit.find_by(:id => self.parent_id)
    if jobunit.nil?
      return '/'
    end
    return jobunit.path + '/' + self.name
  end
end
