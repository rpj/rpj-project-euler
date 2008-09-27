#!/usr/bin/perl
$a=0; for(1..999) { $a+=$_,if(!($_ % 3) || !($_ % 5)); } print "$a";
