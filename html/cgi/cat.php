<?php
// Define the ASCII cat image as a multi-line string
$ascii_cat = '
    /\_/\
   ( o.o )
    > ^ <
';

// Generate the HTML page with the ASCII cat image and a back button
$html = "<html><head><title>ASCII Cat</title></head><body>";
$html .= "<pre>" . $ascii_cat . "</pre>";
$html .= "</body></html>";

// Set the HTTP headers to indicate that the response is an HTML page
header('Content-Type: text/html');

// Print the HTML page
echo $html;
?>
