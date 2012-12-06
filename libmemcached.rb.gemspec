Gem::Specification.new do |s|
  s.name = 'libmemcached.rb'
  s.version = '1.0.0'

  s.summary = 'libmemcached wrapper for ruby'
  s.description = 'simple, clean ruby wrapper for libmemcached 1.0'

  s.homepage = 'http://github.com/tmm1/libmemcached.rb'
  s.has_rdoc = false

  s.authors = ['Aman Gupta']
  s.email = ['aman@tmm1.net']

  s.add_development_dependency 'rake-compiler', '~> 0.7.6'

  s.extensions = ['ext/extconf.rb']
  s.require_paths = ['lib']

  s.files = `git ls-files`.split("\n")
end
