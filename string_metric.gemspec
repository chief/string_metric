# coding: utf-8
lib = File.expand_path('../lib', __FILE__)
$LOAD_PATH.unshift(lib) unless $LOAD_PATH.include?(lib)
require 'string_metric/version'

Gem::Specification.new do |spec|
  spec.name          = "string_metric"
  spec.version       = StringMetric::VERSION
  spec.authors       = ["Giorgos Tsiftsis"]
  spec.email         = ["giorgos.tsiftsis@skroutz.gr"]
  spec.summary       = %q{A simple library with String Metric algorithms}
  spec.description   = %q{A simple library with String Metric algorithms}
  spec.homepage      = "https://github.com/chief/string_metric"
  spec.license       = "MIT"

  spec.files         = `git ls-files`.split($/)
  spec.executables   = spec.files.grep(%r{^bin/}) { |f| File.basename(f) }
  spec.test_files    = spec.files.grep(%r{^(test|spec|features)/})
  spec.require_paths = ["lib"]

  spec.add_development_dependency "bundler", "~> 1.5"
  spec.add_development_dependency "rake", "~> 10.1.1"
  spec.add_development_dependency "rspec", "~> 2.14.1"
  spec.add_development_dependency "text", "~> 1.2.3"

  if RUBY_ENGINE == "ruby"
    spec.add_development_dependency "rake-compiler", "~> 0.9.2"
    spec.extensions << "ext/#{spec.name}/levenshtein/trie_radix_tree/extconf.rb"
    spec.extensions << "ext/#{spec.name}/levenshtein/iterative_with_two_matrix_rows/extconf.rb"

    if RUBY_VERSION > "1.9.3"
      spec.add_development_dependency "pry-byebug", "~> 1.2.1"
    else
      spec.add_development_dependency "pry", "~> 0.9.12.4"
    end
  end
end
