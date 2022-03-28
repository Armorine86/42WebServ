#!/usr/bin/python

import cgi

form = cgi.FieldStorage()

name = form.getvalue('name')
email = form.getvalue('email')
message = form.getvalue('message')

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
