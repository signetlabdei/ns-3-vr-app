## -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-

def build(bld):
    module = bld.create_ns3_module('vr-app', ['core'])
    module.source = [
        'model/burst-generator.cc',
        'model/burst-sink.cc',
        'model/bursty-application.cc',
        'model/my-random-variable-stream.cc',
        'model/seq-ts-size-frag-header.cc',
        'model/simple-burst-generator.cc',
        'model/trace-file-burst-generator.cc',
        'model/vr-burst-generator.cc',
        'helper/burst-sink-helper.cc',
        'helper/bursty-helper.cc',
        ]
    
    headers = bld(features='ns3header')
    headers.module = 'vr-app'
    headers.source = [
        'model/burst-generator.h',
        'model/burst-sink.h',
        'model/bursty-application.h',
        'model/my-random-variable-stream.h',
        'model/seq-ts-size-frag-header.h',
        'model/simple-burst-generator.h',
        'model/trace-file-burst-generator.h',
        'model/vr-burst-generator.h',
        'helper/burst-sink-helper.h',
        'helper/bursty-helper.h',
        ]
    
    if (bld.env['ENABLE_EXAMPLES']):
        bld.recurse('examples')

    bld.ns3_python_bindings()
