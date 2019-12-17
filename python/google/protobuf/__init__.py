# Protocol Buffers - Google's data interchange format
# Copyright 2008 Google Inc.  All rights reserved.
# https://developers.google.com/protocol-buffers/
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
#
#     * Redistributions of source code must retain the above copyright
# notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above
# copyright notice, this list of conditions and the following disclaimer
# in the documentation and/or other materials provided with the
# distribution.
#     * Neither the name of Google Inc. nor the names of its
# contributors may be used to endorse or promote products derived from
# this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

# Copyright 2007 Google Inc. All Rights Reserved.

import sys

__version__ = '3.11.0rc1'

if __name__ != '__main__':
  try:
    __import__('pkg_resources').declare_namespace(__name__)
  except ImportError:
    __path__ = __import__('pkgutil').extend_path(__path__, __name__)


def protos(*args, **kwargs):
  raise NotImplementedError("The protos function is only available when using the cpp implementation"
                            "on a 3.X interpreter.")

if sys.version_info[0] > 2:
  try:
    from google.protobuf.pyext_protoc import _protoc
  except ImportError as e:
    pass
  else:
    import contextlib
    import importlib
    import importlib.machinery
    import threading
    import os

    _PROTO_MODULE_SUFFIX = "_pb2"

    def _module_name_to_proto_file(suffix, module_name):
        components = module_name.split(".")
        proto_name = components[-1][:-1 * len(suffix)]
        return os.path.sep.join(components[:-1] + [proto_name + ".proto"])

    def _proto_file_to_module_name(suffix, proto_file):
        components = proto_file.split(os.path.sep)
        proto_base_name = os.path.splitext(components[-1])[0]
        return ".".join(components[:-1] + [proto_base_name + suffix])

    @contextlib.contextmanager
    def _augmented_syspath(new_paths):
        original_sys_path = sys.path
        if new_paths is not None:
            sys.path = sys.path + new_paths
        try:
            yield
        finally:
            sys.path = original_sys_path

    # TODO: Investigate making this even more of a no-op in the case that we have
    # truly already imported the module.
    def protos(protobuf_path, include_paths=None):
        with _augmented_syspath(include_paths):
            module_name = _proto_file_to_module_name(_PROTO_MODULE_SUFFIX,
                                                     protobuf_path)
            module = importlib.import_module(module_name)
            return module

    _proto_code_cache = {}
    _proto_code_cache_lock = threading.RLock()

    class ProtoLoader(importlib.abc.Loader):

        def __init__(self, suffix, code_generator, module_name, protobuf_path,
                     proto_root):
            self._suffix = suffix
            self._code_generator = code_generator
            self._module_name = module_name
            self._protobuf_path = protobuf_path
            self._proto_root = proto_root

        def create_module(self, spec):
            return None

        def _generated_file_to_module_name(self, filepath):
            components = filepath.split(os.path.sep)
            return ".".join(
                components[:-1] + [os.path.splitext(components[-1])[0]])

        def exec_module(self, module):
            assert module.__name__ == self._module_name
            code = None
            with _proto_code_cache_lock:
                if self._module_name in _proto_code_cache:
                    code = _proto_code_cache[self._module_name]
                    exec(code, module.__dict__)
                else:
                    files = _protoc.get_protos_from_generator(
                        self._code_generator,
                        self._protobuf_path.encode('ascii'),
                        [path.encode('ascii') for path in sys.path])
                    # NOTE: The files are returned in topological order of dependencies. Each
                    # entry is guaranteed to depend only on the modules preceding it in the
                    # list and the last entry is guaranteed to be our requested module. We
                    # cache the code from the first invocation at module-scope so that we
                    # don't have to regenerate code that has already been generated by protoc.
                    for f in files[:-1]:
                        module_name = self._generated_file_to_module_name(
                            f[0].decode('ascii'))
                        if module_name not in sys.modules:
                            if module_name not in _proto_code_cache:
                                _proto_code_cache[module_name] = f[1]
                            importlib.import_module(module_name)
                    exec(files[-1][1], module.__dict__)

    class ProtoFinder(importlib.abc.MetaPathFinder):

        def __init__(self, suffix, code_generator):
            self._suffix = suffix
            self._code_generator = code_generator

        def find_spec(self, fullname, path, target=None):
            filepath = _module_name_to_proto_file(self._suffix, fullname)
            for search_path in sys.path:
                try:
                    prospective_path = os.path.join(search_path, filepath)
                    os.stat(prospective_path)
                except (FileNotFoundError, NotADirectoryError):
                    continue
                else:
                    return importlib.machinery.ModuleSpec(
                        fullname,
                        ProtoLoader(self._suffix, self._code_generator, fullname,
                                    filepath, search_path))

    sys.meta_path.extend([
        ProtoFinder(_PROTO_MODULE_SUFFIX, _protoc.code_generator),
    ])

