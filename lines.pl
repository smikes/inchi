#!/usr/bin/perl -w
use strict;
use warnings;

sub count($) {
    
    my ($str, $l, $s) = (shift);

    $l = ($str =~ tr/\n//);
    $s = ($str =~ tr/;//);

    return ($l, $s);
}

sub get_all($) {
    local $/ = undef;
    my @files = grep { -f } glob(shift);
    join '', map { open my $fh, '<', $_; <$fh> } @files
}

my $csrc = get_all('src/*.cc');
my $ctest = get_all('src/test/*.cc');
my $jssrc = get_all('lib/*.js');
my $jstest = get_all('test/*.js');

my @cs = count($csrc);
my @ct = count($ctest);
my @js = count($jssrc);
my @jt = count($jstest);

format STDOUT_TOP =

         Source   Test    Total
.

my ($label, $a, $b, $c, $d);

format STDOUT =
@<<<<<<< @#####  @#####  @#####  
$label,  $a,     $b,     $c,    
.

$label = 'lines', $a = $cs[0]+$js[0], $b = $ct[0]+$jt[0], $c = $a+$b;
write;
$label = 'semis', $a = $cs[1]+$js[1], $b = $ct[1]+$jt[1], $c = $a+$b;
write;

