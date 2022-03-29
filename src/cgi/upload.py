# #!/usr/bin/python

# import os
# # Import modules for CGI handling
# import cgi, cgitb
# import urllib.request

# inFileData = None

# form = cgi.FieldStorage()

# UPLOAD_DIR='upload'

# # IF A FILE WAS UPLOADED (name=file) we can find it here.
# if "file" in form:
#     form_file = form['file']
#     # form_file is now a file object in python
#     if form_file.filename:

#         uploaded_file_path = os.path.join(UPLOAD_DIR, os.path.basename(form_file.filename))
#         with open(uploaded_file_path, 'wb') as fout:
#             # read the file in chunks as long as there is data
#             while True:
#                 chunk = form_file.file.read(200000)
#                 if not chunk:
#                     break
#                 # write the file content on a file on the hdd
#                 fout.write(chunk)

#         # load the written file to display it
#         with open(uploaded_file_path, 'r') as fin:
#             inFileData = ""
#             for line in fin:
#                 inFileData += line

#!/usr/bin/python
import cgi, os
import cgitb; cgitb.enable()
form = cgi.FieldStorage()

#UPLOAD_DIR="/home/biohazard/Documents/42Cursus/HTTPWebServer/upload"

# Get filename here.
fileitem = form['filename']
# Test if the file was uploaded
if fileitem.filename:
   # strip leading path from file name to avoid
   # directory traversal attacks
   fn = os.path.basename(fileitem.filename.replace("\\", "/" ))
   open('upload/' + fn, 'wb').write(fileitem.file.read())
   message = 'The file "' + fn + '" was uploaded successfully'

else:
   message = 'No file was uploaded'
 
print ("""\
Content-Type: text/html\n
<html>
<body>
   <p>%s</p>
</body>
</html>
""" % (message))
