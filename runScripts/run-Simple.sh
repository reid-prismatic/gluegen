#!/bin/ksh

JAVA=java
GLUEGEN_JAR=../build/gluegen.jar
ANTLR_JAR=../build/antlr.jar

NAME=`uname`

if [[ $NAME == "Windows*" ]] ; then
  SEP=\;
elif [[ $NAME == "CYGWIN*" ]] ; then
  SEP=\;
else
  SEP=:
fi

echo java -cp $GLUEGEN_JAR$SEP$ANTLR_JAR com.jogamp.gluegen.GlueGen -I. -Ecom.jogamp.gluegen.JavaEmitter -Cfunction.cfg function.h
java -cp $GLUEGEN_JAR$SEP$ANTLR_JAR com.jogamp.gluegen.GlueGen -I. -Ecom.jogamp.gluegen.JavaEmitter -C./simpleTest/function.cfg -O./simpleTest/output ./simpleTest/function.h
