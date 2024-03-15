#!/bin/perl

open FA, '<', 'actualTestPredictions' or die "Couldn't have opened the computed results.";
open FB, '<', 'MNIST_DATA/mnist_test_labels.csv' or die "Couldn't have opened the expected results.";

my $na, $nb, $count=0, $correct=0;

while($na = <FA> and $nb = <FB>){
    $count++;
    chomp $na;
    chomp $nb;
    $correct++ if $na == $nb;
}
print "Correctly recognised $correct out of $count test cases.\n";
