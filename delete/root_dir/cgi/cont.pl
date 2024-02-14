#!/usr/bin/perl
use CGI;

# Create a new CGI object
my $cgi = CGI->new;

# Print the header with Content-type
print $cgi->header('text/html');

# Get user input from the form
my $username = $cgi->param('username');

# Generate HTML output
print <<HTML;
<html>
<head>
    <title>Greeting CGI Script</title>
</head>
<body>

<h1>Hello, $username!</h1>

</body>
</html>
HTML

