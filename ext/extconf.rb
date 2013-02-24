CWD = File.expand_path(File.dirname(__FILE__))

def sys(cmd)
  puts "  -- #{cmd}"
  unless ret = xsystem(cmd)
    raise "#{cmd} failed, check #{CWD}/src/mkmf.log and #{CWD}/src/libmemcached-1.0.14/config.log"
  end
  ret
end

require 'mkmf'
require 'fileutils'

xsystem('pwd') # to create mkmf.log before the chdir

unless File.exists?('librubymemcached.a')
  lib = File.basename(Dir["#{CWD}/src/libmemcached-1.*.tar.gz"].first)
  dir = File.basename(lib, '.tar.gz')

  puts "(I'm about to compile libmemcached.. this will definitely take a while)"

  Dir.chdir("#{CWD}/src") do
    FileUtils.rm_rf(dir) if File.exists?(dir)
    sys("tar zpxvf #{lib}")
    Dir.chdir(dir) do
      sys("./configure --with-pic --prefix=#{CWD}/dst --without-memcached --disable-dependency-tracking --disable-shared")
      sys("make install noinst_PROGRAMS= check_PROGRAMS=")
    end
  end

  FileUtils.cp "#{CWD}/dst/lib/libmemcached.a", 'librubymemcached.a'
end

File.open("return_codes.h", "w") do |f|
  f.puts "#include <ruby.h>"
  f.puts "#include <libmemcached/memcached.h>"
  f.puts "#include <libmemcached-1.0/return.h>"
  f.puts
  f.puts "extern VALUE return_codes[MEMCACHED_MAXIMUM_RETURN+1];"
  f.puts "void Init_return_codes();"
end
File.open("return_codes.c", "w") do |f|
  f.puts "#include \"return_codes.h\""
  f.puts
  f.puts "VALUE return_codes[MEMCACHED_MAXIMUM_RETURN+1];"
  f.puts "void"
  f.puts "Init_return_codes()"
  f.puts "{"
  File.read("#{CWD}/src/libmemcached-1.0.14/libmemcached/strerror.cc").scan(/case MEMCACHED_(\w+):/).each do |name, *|
    f.puts "  return_codes[MEMCACHED_#{name}] = ID2SYM(rb_intern(\"#{name.downcase}\"));"
  end
  f.puts "}"
end

$INCFLAGS << " -I#{CWD}/dst/include "
$LIBPATH << CWD
$libs = append_library($libs, 'rubymemcached')
def add_define(name)
  $defs.push("-D#{name}")
end

CONFIG['LDSHARED'] = "$(CXX) " + CONFIG['LDSHARED'].split[1..-1].join(' ')
create_makefile 'libmemcached_ext'
