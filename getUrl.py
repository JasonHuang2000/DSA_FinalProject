from google.google import search


class WebSite:

    def __init__(self, subject):
        self.subject = subject
        self.results = search('wikipedia ' + subject, 1)
        self.url = self.results[0].link


test = WebSite('Minas-Rio')
print(test.url)
