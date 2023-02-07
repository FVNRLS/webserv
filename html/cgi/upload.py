import cgi, fileinput, os, sys

try:
	parsed = cgi.FieldStorage()
	# message = parsed['name']
	fileitem = parsed['name']
	# Test if the file was uploaded
	if fileitem.filename:
		# strip leading path from file name to avoid
		# directory traversal attacks
		fn = os.path.basename(fileitem.filename)
		path = os.path.abspath(os.getcwd())
		path += "/uploads/"
		path += fn
		if (os.path.exists(path)):
			message = 'File already exists'
		else:
			open(path, 'wb').write(fileitem.file.read())
			message = 'The file "' + fn + '" was uploaded successfully'

	else:
		message = 'No file was uploaded'
except:
	message = 'EXCEPTION: No file was uploaded'


# todo this is actually already generating a header (Content-Type: .... )
print("""\
<html>
<body>
   <p>%s</p>
</body>
</html>
""" % (message))
