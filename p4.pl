#!/usr/bin/perl

$c = {};
$a = 0;
for ($a1 = 999; $a1 > 99; $a1--) {
	for ($a2 = 999; $a2 > 99; $a2--) {
		$mult = $a1 * $a2;

		if (!(($len = length($mult)) % 2)) {
			$f = substr($mult, 0, ($len / 2));
			$b = reverse(substr($mult, ($len / 2), ($len / 2)));

			$a = $mult, if ($f eq $b && $mult > $a);
		}
	}
}

print "$a\n";
