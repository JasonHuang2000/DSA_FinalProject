from google.google import search
import sys

class WebSite:

    def __init__(self, subject):
        self.subject = subject
        target = subject + ' wikipedia'
        self.results = search(target, 1)
        if len(self.results) == 0:
            print("Not Found")
        else:
            self.url = self.results[0].link


test = WebSite(sys.argv[1])
print(test.url)
