#!/usr/bin/env python

# WORK IN PROGRESS!!! SCRIPT NOT FIT FOR USE YET

# If finished, this script will redirect to hering.html is cookie is present
# if cookie is not present, the script will redirect to another html page where user is prompted to log in

import os
import cgi
import http.cookies

def main():
    if os.environ.get("HTTP_COOKIE") is not None:
        cookie_string = os.environ.get("HTTP_COOKIE")
        print(cookie_string)
    else:
        print('No cookies env')



    cookie = http.cookies.SimpleCookie(os.environ.get("HTTP_COOKIE"))

    if 'key' in cookie:
        # Print the HTTP header
        print("SCRIPT WORKED, COOKIE FOUND")
        file_path = "html/hering.html"
    else:
        # Print the HTTP header
        print("SCRIPT WORKED, no cookies")
        file_path = "html/login.html"
    try:
        open(file_path)
    except:
        print("HTML NOT FOUND, cwd = ")
        print(os.getcwd())
        return
    # Open the file in read mode
    with open(file_path, 'r') as file:
    # Read the contents of the file into a variable
        content = file.read()

# If the requested file was not found, return "other.html"
    if content is None:
        content = "COULD NOT READ FROM FILE"
    print(content)


if __name__ == "__main__":
    main()



# Get the requested page name from the query string

# page_name = os.environ.get("QUERY_STRING", "")

# # Construct the full file path
# file_path = "pages/{}.html".format(page_name)

# # Read the requested file
# content = read_file(file_path)

# # If the requested file was not found, return "other.html"
# if content is None:
#     file_path = "pages/other.html"
#     content = read_file(file_path)

# # Return the content as an HTTP response
# print("Content-Type: text/html")
# print("")
# print(content)


# TO REDIRECT TO ANOTHER HTML PAGE:
# give me html code to display another html page

# To display another HTML page in an HTML file, you can use an iframe element. An iframe allows you to embed a separate HTML document within the current page. Here's an example:

# css
# Copy code
# <html>
#   <body>
#     <h1>Welcome to my website!</h1>
#     <iframe src="another_page.html"></iframe>
#   </body>
# </html>
# In this example, the src attribute specifies the location of the HTML file to be embedded in the iframe. The content of the file "another_page.html" will be displayed within the iframe element.

# You can also control the size and appearance of the iframe by adding additional attributes, such as width and height.
