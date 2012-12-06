require 'rubygems'
require 'rubygems/package_task'
require 'rake/extensiontask'
require 'rake/testtask'

GEMSPEC = Gem::Specification.load('libmemcached.rb.gemspec')

Gem::PackageTask.new(GEMSPEC) do |pkg|
end

Rake::ExtensionTask.new('libmemcached_ext', GEMSPEC) do |ext|
  ext.ext_dir = 'ext'
end

Rake::TestTask.new 'test' do |t|
  t.test_files = FileList['test/test_*.rb']
  t.ruby_opts = ['-rubygems']
end

task :test => :compile
task :default => :test
