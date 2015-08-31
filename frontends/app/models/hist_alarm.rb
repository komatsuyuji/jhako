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

class HistAlarm < ActiveRecord::Base
  belongs_to :hist_jobunit

  def path
    id = self.jobnet_id
    if id == 0
      return '/'
    end

    jobunit = Jobunit.find_by(:id => id)
    if jobunit.nil?
      return ''
    end

    return jobunit.path
  end

  def as_json
    super(:methods => :path)
  end
end
