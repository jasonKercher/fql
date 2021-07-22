#!/bin/bash

while read -r TOK; do
  if ! grep -qw "$TOK" TSqlParser.g4; then
    grep -vw "^$TOK:" TSqlLexer.g4 > lex.temp
    mv lex.temp TSqlLexer.g4 
  fi
done < lex.txt
