#!/usr/bin/python
import datetime

# Get the current time
current_time = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")

# Create the HTML content
print("""\
<html>
  <head>
    <title>Current Time</title>
    <style>
    body {
      font-family: Arial, sans-serif;
      text-align: center;
    }
    button {
      padding: 10px 20px;
      margin: 10px;
      background-color: #333;
      color: #fff;
      border: none;
      border-radius: 5px;
      cursor: pointer;
    }
    button a {
      color: #fff;
      text-decoration: none;
    }
    </style>
  </head>
  <body>
    <h1>Current Time</h1>
    <p>%s</p>
    <button><a href="javascript:history.back()">Back</a></button>
  </body>
</html>
""" % (current_time))
