#!/usr/bin/perl

$a = 1;
$b = 2;
$c = 0;

$s = 0;

$t = 4e6;
print "$t\n";
for (; $b < 4e6; $c = $b, $b += $a, $a = $c) {
	if (!($b % 2)) {
		$s += $b;
	}
}

print "$s\n";
