#!/bin/sh
JAR="$(ls sbe-all-*.jar)"
java \
  -Dsbe.generate.ir=false \
  -Dsbe.target.language=cpp \
  -Dsbe.target.namespace=codec \
  -Dsbe.cpp.namespaces.collapse=false \
  -Dsbe.output.dir=../ \
  -Dsbe.decode.unknown.enum.values=true \
  -jar $JAR \
  schema.xml
