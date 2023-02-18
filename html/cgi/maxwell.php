<?php
// Define the Tenor embed code for the GIF image
$tenor_embed_code = '<div class="tenor-gif-embed" data-postid="27234188" data-share-method="host" data-aspect-ratio="1.50235" data-width="100%"><a href="https://tenor.com/view/maxwell-maxwell-spin-spinning-cat-maxwell-spinning-gif-27234188">Maxwell Maxwell Spin GIF</a>from <a href="https://tenor.com/search/maxwell-gifs">Maxwell GIFs</a></div> <script type="text/javascript" async src="https://tenor.com/embed.js"></script>';

// Generate the HTML page with the embedded GIF image and a back button
$html = "<html><head><title>Maxwell the Dancing Cat</title></head><body>";
$html .= $tenor_embed_code;
$html .= "</body></html>";

// Set the HTTP headers to indicate that the response is an HTML page with an embedded image
header('Content-Type: text/html');

// Print the HTML page
echo $html;
?>
