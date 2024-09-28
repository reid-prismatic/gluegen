#!/bin/bash

JAVA=java
GLUEGEN_JAR=../../build/gluegen.jar
ANTLR_JAR=../../build/antlr.jar

NAME=`uname`

if [[ $NAME == "Windows*" ]] ; then
  SEP=\;
elif [[ $NAME == "CYGWIN*" ]] ; then
  SEP=\;
else
  SEP=:
fi

# Base command
CMD="$JAVA -cp $GLUEGEN_JAR$SEP$ANTLR_JAR com.jogamp.gluegen.GlueGen"
CMD+=" -I./define_platform.h"

# Base include directory
INCLUDE_BASE="./include"

# Find all directories in the include base and add them as -I options
for dir in $(find $INCLUDE_BASE -type d); do
    CMD+=" -I$dir"
done

# GLDocHeader ./define_platform.h

# Add system include directories for standard headers
if [[ $NAME == "Darwin" ]]; then
  # macOS specific include paths  
  # CMD+=" -I/usr/include -I/usr/local/include -I$(xcrun --show-sdk-path)/usr/include"
  CMD+=" -I/usr/include -I/usr/local/include"  
elif [[ $NAME == "Linux" ]]; then
  # Linux specific include paths
  CMD+=" -I/usr/include -I/usr/local/include"
elif [[ $NAME == "CYGWIN" || $NAME == "Windows" ]]; then
  # Windows/Cygwin specific include paths
  CMD+=" -IC:/path/to/msys64/usr/include -IC:/path/to/msys64/usr/local/include"
fi

CMD+=" -I/Users/reid/Projects/prismatic/gluegen/make/stub_includes/gluegen \
-I/Users/reid/Projects/prismatic/gluegen/make/stub_includes/platform \
-I/Users/reid/Projects/prismatic/gluegen/make/stub_includes/os \
-I/Users/reid/Projects/prismatic/gluegen/make/stub_includes/jni
"

# Add the remaining GlueGen options
# CMD+=" -Ecom.jogamp.gluegen.JavaEmitter -CangleTest.cfg -Ooutput ./include/EGL/egl.h"
CMD+=" -Ecom.jogamp.gluegen.JavaEmitter -CangleTest.cfg -Ooutput ./include/GLES2/gl2.h"

# Execute the command
echo $CMD
eval $CMD

# #!/bin/ksh

# JAVA=java
# GLUEGEN_JAR=../../build/gluegen.jar
# ANTLR_JAR=../../build/antlr.jar

# NAME=`uname`

# if [[ $NAME == "Windows*" ]] ; then
#   SEP=\;
# elif [[ $NAME == "CYGWIN*" ]] ; then
#   SEP=\;
# else
#   SEP=:
# fi

# # Base command
# CMD="java -cp $GLUEGEN_JAR$SEP$ANTLR_JAR com.jogamp.gluegen.GlueGen"

# # Base include directory
# INCLUDE_BASE="./include"

# # Find all directories in the include base and add them as -I options
# for dir in $(find $INCLUDE_BASE -type d); do
#     CMD+=" -I$dir"
# done

# # Add the remaining GlueGen options
# CMD+=" -Ecom.jogamp.gluegen.JavaEmitter -CangleTest.cfg -Ooutput ./include/EGL/egl.h"

# # Execute the command
# echo $CMD
# eval $CMD
