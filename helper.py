import os

os.system("rm -f upload.zip") 
os.system("mkdir upload")
os.system("cp AVLTree* MailBox* Makefile upload/")
os.system("zip -r upload.zip upload; rm -rf upload")
