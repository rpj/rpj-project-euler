#!/usr/bin/perl
# real	5m8.681s

$c=1;$a=0;while(!$a){$i=1;for(1..20){last, if(($c % $_)&&!($i=0));}$a=$c, if($i);$c++;}print "$a\n";
