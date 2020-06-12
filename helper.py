import os

os.system("mkdir upload")
os.system("cp AVLtree* MailBox* Makefile upload/")
os.system("zip -r upload.zip upload; rm -rf upload")
