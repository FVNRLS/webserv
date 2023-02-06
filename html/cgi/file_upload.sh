#!/bin/bash

echo "Content-Type: text/html"
echo ""

query_string="$QUERY_STRING"
file_encoded="$(echo "$query_string" | sed -n 's/^file=\(.*\)$/\1/p')"

if [ -n "$file_encoded" ]; then
  file_decoded="$(echo "$file_encoded" | tr '+' ' ' | sed 's/%/\\x/g')"
  file_name="$(echo "$query_string" | sed -n 's/^.*filename=\([^&]*\).*$/\1/p')"
  file_path="/tmp/$file_name"
  echo -ne "$file_decoded" > "$file_path"
  message="File uploaded successfully"
else
  message="No file was uploaded"
fi

echo "<html>"
echo "  <head>"
echo "    <title>File Upload Result</title>"
echo "  </head>"
echo "  <body>"
echo "    <p>$message</p>"
echo "  </body>"
echo "</html>"