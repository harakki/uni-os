#!/bin/bash

# Получение списка экспортируемых функций динамической библиотеки
nm -D ../build/libdynlist.so | grep T
readelf -s ../build/libdynlist.so > /dev/null
