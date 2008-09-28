#!/usr/bin/perl

sub prodOfDigits($) {
	my $arg = shift;
	$prod = 1;
	$prod *= $_, foreach(split(//, $arg));
	return $prod;
}

open (F, "rsrc/bignum.txt");
$a = 0;
$stnum = '';

while (<F>) {
	chomp();
	$stnum = $stnum . $_;
}

close (F);

for ($c = 0; $c < (length($stnum) - 5); $c++) {
	$a = $this, if (($this = prodOfDigits(substr($stnum, $c, 5))) > $a);
}

print "$a\n";
