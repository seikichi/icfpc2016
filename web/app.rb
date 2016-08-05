#!/usr/bin/env ruby
# coding: utf-8

require 'rubygems'
require 'sinatra'

get '/' do
  "Hello, world\n"
end

get '/test' do
  "this is a test action.\n"
end
