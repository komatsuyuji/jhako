#!/bin/bash

home=`dirname $0`
cd $home

aclocal -I m4
autoheader
automake -a
autoconf -i

./configure
