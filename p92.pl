#!/usr/bin/perl
$|++;
$tc = 0;
$c = 1;
$lim = 10e6;
$pp = 100000;

$mh = {};

for (; $c < $lim; $c++) {
	if (!($c % $pp)) { print "".(($c / $lim) * 100)."% done\n"; }
	$t = {};
	$r = $c;
	$g = 1;

	#print "- checking $r\n";
	do {
		$n = 0;
		$t->{$r} = 1;
		$n += ($_ * $_), foreach(split(//, "$r"));
		$r = $n;
		#print "next is $r\n";

		if ($mh->{$r}) {
			$r = $mh->{$r};
			#print "breaking at $r\n";
			$g = 0;
		}
	} while (!($t->{$n}) && $r != 1 && $r != 89 && $g);
	#print "out with $r\n";
	
	$mh->{$c} = $r;
	$mh->{$_} = $r, foreach(keys(%{$t}));
	$tc += 1, if ($r == 89);
}

print "\n$tc\n";
