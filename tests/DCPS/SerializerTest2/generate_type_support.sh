#!/bin/bash
echo $DDS_ROOT
$DDS_ROOT/bin/opendds_idl --idl-version 4 --unknown-annotations ignore -o . SerializerTest2.idl
