#!/bin/bash

meson compile -C builddir
GTK_DEBUG=interactive ./builddir/src/{{FILENAME}}