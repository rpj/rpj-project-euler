#!/usr/bin/perl

# first, an in-memory list of the words
# we find the max word length we'll have to deal with,
# to give us an upper limit on triangle number seq generation
@w = ();
$ml = 0;
open(WF, "./rsrc/words.txt") or die "no words: $!\n\n";
{
	$/ = undef;
	foreach (split(/,/, <WF>)) {
		if (/\"(.*?)\"/ig) {
			push(@w, $1);
			$ml = length($1), if (length($1) > $ml);
		}
	}
}
close(WF);

# now generate triangle numbers up to $ml, the max possible
# "word number" we could get from above
# the $th hash is just there to make lookup a bit faster... but
# since there's only 28 trinums up to the max limit in 'words.txt',
# it's not really necessary
$gl = $ml * 26;
$th = {};
$n = 0;
$l = 0;
for (; $l < $gl; $n++) {
	$l = ($n * ($n + 1)) / 2;
	$th->{$l} = 1;
}

# finally, calculate "word numbers" and check the trinum hash
$wc = 0;
foreach (@w) {
	$tn = 0;
	$tn += (ord($_) - 64), foreach(split(//));
	$wc++, if ($th->{$tn});
}

print "$wc\n";
