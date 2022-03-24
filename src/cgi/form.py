#!/usr/bin/python

import cgi

form = cgi.FieldStorage()

name = form.getvalue('name')
email = form.getvalue('email')
message = form.getvalue('message')

# length = len(name)
# length += len(email)
# length += len(message)

# print ('HTTP/1.1 200 OK')
# print ('Content-Type: text/html')
# print ("Content-Length: %d" % (length))

print ('<html>')
print ('<head>')
print ('<title>CGI GET</title>')
print ('</head>')
print ('<body>')
print ('<h1>Here is the informations you have entered</h1>')
print ('<h2>Your name is: %s</h2>'% (name))
print ('<h2>Your email is: %s</h2>' % (email))
print ('<h2>Message: %s</h2>'% (message))
print ('</body>')
print ('</html>')
