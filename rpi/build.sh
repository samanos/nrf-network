#!/usr/bin/env bash

gcc -g -o ptx -std=gnu99 -Wall -I ../bcm2835/src/ \
  ../bcm2835/src/bcm2835.c \
  ../common.c \
  ../transmitter.c \
  platform.c \
  ptx.c

gcc -g -o prx -std=gnu99 -Wall -I ../bcm2835/src/ \
  ../bcm2835/src/bcm2835.c \
  ../common.c \
  ../receiver.c \
  platform.c \
  prx.c
