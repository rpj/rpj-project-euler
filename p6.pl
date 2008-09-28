#!/usr/bin/perl

$sqos=0; $sosq=0; for(1..100){$sqos+=$_; $sosq+=($_ * $_);} $sqos*=$sqos; $a=$sqos-$sosq; print "$a\n";
