import os
import cgi
import http.cookies



cookie = http.cookies.SimpleCookie(os.environ.get("HTTP_COOKIE"))
if 'key' in cookie:
    # Print the HTTP header
    file_path = "html/cgi/src/logged.html"
else:
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
print(content)

