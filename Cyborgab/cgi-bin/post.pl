#!/usr/bin/perl

local ($buffer, @pairs, $pair, $name, $value, %FORM);
# Read in text
$ENV{'REQUEST_METHOD'} =~ tr/a-z/A-Z/;

if ($ENV{'REQUEST_METHOD'} eq "POST") {
   read(STDIN, $buffer, $ENV{'CONTENT_LENGTH'});
} else {
   $buffer = $ENV{'QUERY_STRING'};
}

# Split information into name/value pairs
@pairs = split(/&/, $buffer);

foreach $pair (@pairs) {
   ($name, $value) = split(/=/, $pair);
   $value =~ tr/+/ /;
   $value =~ s/%(..)/pack("C", hex($1))/eg;
   $FORM{$name} = $value;
}

$fullname = $FORM{name};
$email  = $FORM{email};
$message  = $FORM{message};

print "<html>";
print "<head>";
print "<title>PERL POST</title>";
print "</head>";
print "<body>";
print "<h1>BASIC PERL POST REQUEST PAGE";
print "<h2>Here are the collected informations";
print "<h3>Name: $fullname</h2>";
print "<h3>Email: $email</h2>";
print "<h3>Message: $message</h2>";
print "</body>";
print "</html>";

