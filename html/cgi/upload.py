import cgi, fileinput, os, sys, http.cookies


cookie = http.cookies.SimpleCookie(os.environ.get("HTTP_COOKIE"))
if 'key' in cookie:
    # Print the HTTP header
    file_path = "html/cgi/src/hering.html"
else:
    # Print the HTTP header
    print("SCRIPT WORKED, no cookies")
    file_path = "html/login.html"
try:
    open(file_path)
except:
    print("HTML NOT FOUND, cwd = ")
    print(os.getcwd())
    exit()
# Open the file in read mode
with open(file_path, 'r') as file:
# Read the contents of the file into a variable
    content = file.read()
# If the requested file was not found, return "other.html"
if content is None:
    content = "COULD NOT READ FROM FILE"

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
		path += fn
		if (os.path.exists(path)):
			message = 'File already exists'
		else:
			open(path, 'wb').write(fileitem.file.read())
	else:
	    content = 'No file was uploaded'
except:
	message = 'EXCEPTION: No file was uploaded'


# todo this is actually already generating a header (Content-Type: .... )
print(content)
