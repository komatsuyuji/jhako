#################################################################################
#
# $Date:: 2015-01-13 11:56:17 #$
# $Rev: 471 $
# $Author: komatsu2055 $
#
#################################################################################

# Set up gems listed in the Gemfile.
ENV['BUNDLE_GEMFILE'] ||= File.expand_path('../../Gemfile', __FILE__)

require 'bundler/setup' if File.exist?(ENV['BUNDLE_GEMFILE'])
