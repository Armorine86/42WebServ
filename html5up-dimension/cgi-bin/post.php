#!/usr/bin/php
<?php

$name = $_POST['name'];

if ($name == null) {
    $name = 'Default';
}

$email = $_POST['email'];

if ($email == null) {
    $email = 'Default@default.def';
}

$message = $_POST['message'];

if ($message == null) {
    $message = 'These are all Default Values';
}


#print "$name says: $message";
print "<html>";
print "<head>";
print "<title>CGI POST</title>";
print "</head>";
print "<body>";
print "<h1>Here is the informations you have entered</h1>";
print "<h2>Your name is: $name</h2>";
print "<h2>Your email is: $email</h2>";
print "<h2>Message: $message</h2>";
print "</body>";
print "</html>";
