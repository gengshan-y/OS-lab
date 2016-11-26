#!/bin/bash

function count {
  echo -n "Number of matches for $1:"  # $1 is the first arguments
  ls $1 | wc -l  # count for files in the dictionary
}
