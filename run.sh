#!/bin/sh
##
#  file: run.sh
#  date: 2017.09.05
#
#

work_dir=$(cd $(dirname $0); pwd)
bin_dir=$work_dir/bin
param_dir=$work_dir/param

analyzer=$bin_dir/AlphaDNN

network=$param_dir/deploy.prototxt
caffemodel=$param_dir/snapshot_iter_43530.caffemodel
mean=$param_dir/mean.jpg
label=$param_dir/labels.txt

if [ -z $1 ]; then
    :
else
    infile=$1
fi

echo
echo "work_dir   : $work_dir"
echo "bin_dir    : $bin_dir"
echo "param_dir  : $param_dir"
echo "analyzer   : $(basename $analyzer)"
echo "network    : $(basename $network)"
echo "caffemodel : $(basename $caffemodel)"
echo "mean       : $(basename $mean)"
echo "label      : $(basename $label)"
echo "infile     : $infile"
echo

$analyzer $network $caffemodel $mean $label $infile
