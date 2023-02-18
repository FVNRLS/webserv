import cgi, os

try:
	parsed = cgi.FieldStorage()
	# # message = parsed['name']
	fileitem = parsed['name']
	# # Test if the file was uploaded
	if fileitem.filename:
		# strip leading path from file name to avoid
		# directory traversal attacks
		fn = os.path.basename(fileitem.filename)
		path = os.path.abspath(os.getcwd())
		path += "/uploads/"
		if not os.path.exists(path):
			os.mkdir(path)
		path += fn
		if (os.path.exists(path)):
			content = 'File already exists!'
		else:
			open(path, 'wb').write(fileitem.file.read())
			content = 'Success!'
	else:
	    content = 'File is not uploaded!'
except:
	content = 'File is not uploaded!'


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