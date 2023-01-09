from bs4 import BeautifulSoup
import time
import os
import json
from sys import argv

class HTMLExtractor:
    
    def __init__(self):
        
        self.buffer          = ""

        self.BASE_DIR        = "./ruwiki/"
        self.pathToSave      = "backup.json"

        self.IGNORE_TITLE    = ["Категория:", "Файл:", "Шаблон:", "Википедия:", "Проект:"]
        self.IGNORE_TEXT     = ["#перенаправление", "#Перенаправление", "#ПЕРЕНАПРАВЛЕНИЕ", "#REDIRECT", "#Redirect", "#redirect"]

        self.total_html      = 0
        self.total_pages     = 0
        self.total_lines     = 0
        self.current_line    = 0

        self.begin_time      = time.time()

    def createHTML(self, title, text, name):

        for ignore_line in self.IGNORE_TITLE:
            if ignore_line in title:
                return
            
        for ignore_line in self.IGNORE_TEXT:
            if ignore_line in text:
                return

        htmlTemplate = "<!DOCTYPE html> <html> <body> <h1>" + title + "</h1> <p>" + text + "</p> </body> </html>"
        with open(self.BASE_DIR + str(name) + ".html", "w") as file:
            file.write(htmlTemplate)
        
        self.total_html += 1
        return


    def parseXMLSegment(self, buffer):
        soup = BeautifulSoup(buffer, "xml")
        title = soup.find("title")
        text = soup.find("text")
        return title.text, text.text

    def beforeStart(self):
        if not os.path.exists(self.BASE_DIR):
            os.makedirs(self.BASE_DIR)

        if os.path.exists(self.pathToSave):
            with open(self.pathToSave, "r") as backupFile:
                data = json.load(backupFile)
                self.total_html  = data["total_html"]
                self.total_pages = data["total_pages"]
                self.total_lines = data["total_lines"]
                self.buffer      = data["buffer"]

    def start(self, files):

        self.beforeStart()

        get_page_segment = False
        for file in files:
            with open(file, "r") as xmlf:
                try:
                    for line in xmlf:
                        if self.current_line < self.total_lines:
                            print("\r> Skip line: " + str(self.current_line), end="")
                            self.current_line += 1
                            continue
                        if "<page>" in line:
                            get_page_segment = True
                        if get_page_segment:
                            self.buffer += line
                        if "</page>" in line:
                            get_page_segment = False
                            delta_time = time.gmtime(time.time() - self.begin_time)
                            self.total_pages += 1
                            print("\33[2K\r| Total time: {hour:02d}:{min:02d}:{sec:02d} | Total pages: {total_p:10d} | Total HTML-docs: {total_h:10d} | Total lines: {total_l:15d} |".format(hour=delta_time.tm_hour, min=delta_time.tm_min, sec=delta_time.tm_sec, total_p=self.total_pages, total_h=self.total_html, total_l=self.total_lines), end="")
                            title, text = self.parseXMLSegment(self.buffer)
                            self.createHTML(title, text, self.total_html)
                            self.buffer = ""
                        
                        self.current_line += 1
                        self.total_lines += 1

                except KeyboardInterrupt as e:
                    print("")
                    print("Stopped!")
                    print(e)
                    save = json.dumps(self.__dict__)
                    with open(self.pathToSave, "w") as backupFile:
                        backupFile.write(save)
                    return 1

                except BaseException as e:
                    print("")
                    print("Error while getting document. Exit!")
                    print(e)
                    save = json.dumps(self.__dict__)
                    with open(self.pathToSave, "w") as backupFile:
                        backupFile.write(save)
                    return 1

        print("Finished!\nTotal pages: {total_p:10d}\nTotal html-files: {total_h:10d}\n".format(total_p=self.total_pages, total_h=self.total_html))

if __name__ == "__main__":
    if len(argv) > 1:
        extractor = HTMLExtractor()
        extractor.start(argv[1:])
    else:
        print("Example usage: python " + argv[0] + " [dir_path...]")
