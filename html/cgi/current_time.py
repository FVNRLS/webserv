#!/usr/bin/python3

import os
import time

print("Content-type: text/html\n")
print("<html>")
print("<head>")
print("<title>Current Time</title>")
print("</head>")
print("<body>")
print("<h1>Current Time</h1>")
print("<p>The current time is: %s</p>" % time.ctime())
print("<br>")
homepage = os.environ.get("QUERY")
if homepage:
    print("<a href='%s'>Homepage</a>" % homepage)
else:
    print("<p>Homepage URL not set</p>")
print("</body>")
print("</html>")