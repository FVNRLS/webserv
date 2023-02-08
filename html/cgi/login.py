# if cookie is not present, the script will redirect to another html page where user is prompted to log in

import os
import cgi


def main():
    file_path = "../html/startpage_logged.html"
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
