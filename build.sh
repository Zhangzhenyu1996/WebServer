#!/bin/sh


set -x

SOURCE_DIR=`pwd`
BUILD_DIR=${BUILD_DIR:-debug}
BUILD_TYPE=${BUILD_TYPE:-Debug}

mkdir -p $BUILD_DIR \
      && cd $BUILD_DIR \
      && cmake $SOURCE_DIR \
      && make $*
