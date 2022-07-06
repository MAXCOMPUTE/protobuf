Gem::Specification.new do |s|
  s.name        = "google-protobuf"
  s.version     = "3.21.2"
  git_tag       = "v#{s.version.to_s.sub('.rc.', '-rc')}" # Converts X.Y.Z.rc.N to vX.Y.Z-rcN, used for the git tag
  s.licenses    = ["BSD-3-Clause"]
  s.summary     = "Protocol Buffers"
  s.description = "Protocol Buffers are Google's data interchange format."
  s.homepage    = "https://developers.google.com/protocol-buffers"
  s.authors     = ["Protobuf Authors"]
  s.email       = "protobuf@googlegroups.com"
  s.metadata    = { "source_code_uri" => "https://github.com/protocolbuffers/protobuf/tree/#{git_tag}/ruby" }
  s.require_paths = ["lib"]
  s.files       = Dir.glob('lib/**/*.rb') + Dir.glob('ext/**/*')
  s.extensions  = ["Rakefile"] + Dir.glob('ext/**/*')
  s.test_files  = ["tests/basic.rb",
                  "tests/stress.rb",
                  "tests/generated_code_test.rb"]
  s.required_ruby_version = '>= 2.3'
  s.add_dependency 'ffi', '~>1'
  s.add_dependency 'ffi-compiler', '~>1'
  s.add_dependency 'rake', '~>13'
  s.add_development_dependency "rake-compiler-dock", "= 1.2.1"
  s.add_development_dependency "rake-compiler", "~> 1.1.0"
  s.add_development_dependency "test-unit", '~> 3.0', '>= 3.0.9'
end
