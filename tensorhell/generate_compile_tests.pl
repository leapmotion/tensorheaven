#!/usr/bin/perl -w
use strict;

sub process_directory;

my $compile_tests_dir = "compile_tests";
my $source_directory = "../include/tenh";
my @compile_test_files;

if (!-d $compile_tests_dir)
{
    mkdir $compile_tests_dir;
}

process_directory("$compile_tests_dir/tenh", $source_directory);

open(my $fh, ">", "$compile_tests_dir/main.cpp");

print $fh <<"EOT";
int main(void)
{
    return 0;
}
EOT

close($fh);

open($fh, ">", "$compile_tests_dir/CMakeLists.txt");

print $fh "set(compile_test_SRCS\nmain.cpp\n";

foreach my $file (@compile_test_files)
{
    $file =~ s/$compile_tests_dir\///;
    print $fh "$file\n";
}

print $fh ")\nadd_executable(compile_test \${compile_test_SRCS})\n";

close($fh);


sub process_directory
{
    my $output_directory = shift;
    my $input_directory = shift;

    if (!-d $output_directory)
    {
        mkdir $output_directory;
    }

    opendir(my $dh, $input_directory) or die "Failed to open directory: $input_directory\n";

FILE:
    while(my $file = readdir($dh))
    {
        next FILE if ($file =~ /^\.\.?$/);
        if (-d "$input_directory/$file")
        {
            process_directory("$output_directory/$file", "$input_directory/$file");
        }
        elsif ($file =~ /.hpp$/)
        {
            my $outfile = $file;
            my $function_name = "$input_directory/$file";
            "$input_directory/$file" =~ /(tenh\/.*)/;
            my $include_path = $1;
            $outfile =~ s/.hpp$/.cpp/;
            $function_name =~ s/\/|\./_/g;
            $function_name =~ s/^_+//;

            open(my $fh, ">", "$output_directory/$outfile") or die "Failed to open file: $output_directory/$outfile\n";
            print $fh <<"EOT";
#include "$include_path"

int $function_name()
{
    return 0;
}
EOT
            close $fh;
            push @compile_test_files, "$output_directory/$outfile";
        }
    }

    closedir $dh;
}