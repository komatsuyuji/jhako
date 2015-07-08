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

module ApplicationHelper
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
  def search(object, condition, model)
    return object if condition.nil?

    condition.each do |key, value|
      field = model.arel_table[value[:field]]
      data = value[:data]
      case data[:type]
      when 'string' then
        object = object.where(field.matches('%' + data[:value] + '%'))
      when 'numeric' then
        object = object.where(field.send(data[:comparison], data[:value]))
      when 'boolean' then
        b = (data[:value].downcase() == 'true')? 1 : 0
        object = object.where(field.eq(b))
      when 'datetime'
        dt = Time.parse(data[:value]).to_datetime
        object = object.where(field.send(data[:comparison], dt))
      when 'list'
        object = object.where(field.in(data[:value].split(',')))
      else
      end
    end
    return object
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
  def sort(object, condition, model)
    return object if condition.nil?

    cond = JSON.parse(condition)
    return object if !cond.kind_of?(Array)

    cond.each do |c|
      next if c['property'].nil? || c['direction'].nil?
      field = model.arel_table[c['property']]
      object = object.order(field.send(c['direction'].downcase()))
    end
    return object
  end

end
