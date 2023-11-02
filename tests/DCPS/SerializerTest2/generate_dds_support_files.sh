#!/bin/bash

$ACE_ROOT/bin/tao_idl --idl-version 4 --unknown-annotations ignore -o . ./SerializerTest2.idl

$ACE_ROOT/bin/tao_idl --idl-version 4 --unknown-annotations ignore -o . ./SerializerTest2TypeSupport.idl
