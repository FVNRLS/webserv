#!/usr/bin/env python

# WORK IN PROGRESS!!! SCRIPT NOT FIT FOR USE YET

# If finished, this script will redirect to hering.html is cookie is present
# if cookie is not present, the script will redirect to another html page where user is prompted to log in

import os
import cgi
import http.cookies

def main():
    # Get cookie from the environment
    cookies = http.cookies.SimpleCookie(os.environ.get("HTTP_COOKIE"))

    # Check if the cookie is present
    if "my_cookie" in cookies:
        # Print the HTTP header
        print("Content-Type: text/html")
        print()
        # Print the HTML code
        print("""
        <html>
        <body>
            <img src="image.jpg" alt="My Image">
        </body>
        </html>
        """)
    else:
        # Print an error message
        print("Content-Type: text/html")
        print()
        print("<html><body><h1>Cookie not found!</h1></body></html>")

if __name__ == "__main__":
    main()

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
