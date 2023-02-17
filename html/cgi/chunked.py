import os, sys

# filename = "curl_upload"
# path = os.path.abspath(os.getcwd())
# path += "/uploads/"
# if not os.path.exists(path):
# 	os.mkdir(path)
# path += filename
# if (os.path.exists(path)):
# 	content = 'File already exists!'
# else:
# 	with open(path, 'wb') as f:
# 		for line in sys.stdin:
# 			f.write(line)
content = 'File uploaded'

print("""\
<html>
  <head>
    <title>Upload Status</title>
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
    <h1>Status of Upload</h1>
    <p>%s</p>
    <button><a href="javascript:history.back()">Back</a></button>
  </body>
</html>
""" % (content))