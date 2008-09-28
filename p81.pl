#!/usr/bin/perl

$matrix = $ARGV[0] || die "need matrix file.\n\n";

$moveDirs = { 'r' => [1, 0], 'd' => [0, 1], 'l' => [-1, 0], 'u' => [0, -1] };

sub readMatrix($) {
	my $file = shift;
	my $aret = [];
	my $acnt = 0;

	open(F, "$file") or die "cannot open $file: $!\n\n";
	while (<F>) {
		chomp();
		push (@{$aret->[$acnt++]}, split(/,/));
	}
	close(F);

	return $aret;
}

my $dSize = -1;
$daRef = readMatrix($matrix);
$dSize = scalar(@{$daRef});

print "Got matrix $daRef, size $dSize\n";

##
$allowedMoves = [ $moveDirs->{'l'}, $moveDirs->{'u'} ];
@start = (0, 0);
@end = ($dSize - 1, $dSize - 1);
##

$path = [];
$psum = [];
$x = $start[0];
$y = $start[1];

while ($x < $end[0] && $y < $end[1]) {
#while ($x >= $end[0] && $y >= $end[1]) {
	$cur = $daRef->[$x][$y];
	push (@{$path}, $cur);
	push (@{$psum}, $cur + $psum->[(scalar(@{$psum}) - 1)]);
	print "- examining $cur @ [$x, $y]\n";

	$least = 0xffffffff;
	$lleast = 0;
	$lxmod = 0;
	$lymod = 0;

	foreach $move (@{$allowedMoves}) {
		$xmod = $x + $move->[0];
		$ymod = $y + $move->[1];
		print "\t- [$xmod, $ymod] -> $daRef->[$xmod][$ymod] <? $least\n";

		if (($this = $daRef->[$xmod][$ymod]) < $least) {
			print "\t\t- setting...\n";
			$lleast = $least;
			$least = $this;
			$lxmod = $xmod;
			$lymod = $ymod;
		}
	}

	print "least is $least, [$lxmod, $lymod] ... lleast is $lleast\n";
	$x = $lxmod;
	$y = $lymod;
}

use Data::Dumper;
print Dumper(@{$path});
print "\n";
print Dumper(@{$psum});

print "\n" . ($psum->[(scalar(@{$psum}) - 1)]) . "\n";
