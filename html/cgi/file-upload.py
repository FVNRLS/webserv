#!/usr/bin/python

import cgi, cgitb
import os
import sys

cgitb.enable()  # for troubleshooting

# Get the data to be written to the file
raw_data = sys.stdin.read()

# Extract the filename from the raw data
if "filename=" in raw_data:
    file_name = raw_data.split("filename=")[1].split("\r\n")[0].strip('"')

    # Get the actual data to be written to the file
    if "\r\n\r\n" in raw_data:
        file_data = raw_data.split("\r\n\r\n")[1]
    else:
        file_data = raw_data

    # Write the data to the file
    with open(file_name, 'w') as f:
        f.write(file_data)

    # Confirm that the file was written successfully
    print("Content-type: text/html\n")
    print("<html>")
    print("<head>")
    print("<title>File written successfully</title>")
    print("</head>")
    print("<body>")
    print("<h1>File written successfully</h1>")
    print("</body>")
    print("</html>")
else:
    # Report that the filename was not provided
    print("Content-type: text/html\n")
    print("<html>")
    print("<head>")
    print("<title>Error</title>")
    print("</head>")
    print("<body>")
    print("<h1>Error: filename not provided</h1>")
    print("</body>")
    print("</html>")
