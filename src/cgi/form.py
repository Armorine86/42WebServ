import cgi

form = cgi.FieldStorage();

name = form.getvalue("name")
email = form.getvalue("email")
message = form.getvalue("message")

length = len(name)
length += len(email)
length += len(message)

# print HEADER fields
print ("HTTP/1.1 200 OK")
print ("Content-type: text/html\r\n\r\n")
print ("Content-Length: %d", length)

# print HTML response

print ("<html>")
print ("<head>")
print ("<title>CGI TEST PAGE</title>")
print ("</head>")
print ("<body>")
print ("<h2>Here are the informations you have entered!</h2>")
print ("<h2>Name: %s \nEmail: %s</h2>", name, email)
print ("<h2>\nMessage: %s</h2>", message)
print ("</body>")
print ("</html>")

