#!/usr/bin/perl

@bp = ();
$t = 600851475143;

open (P, "./rsrc/primes.txt") or die "no primes: $!\n\n";
while (<P>) {
	push (@bp, (split(/\s/, $_))[1]);
}
close(P);

@bp = reverse(@bp);

foreach (@bp) {
	$d = ($t / $_);
	if (!($d - int($d))) { print "$_\n"; last; }
}
