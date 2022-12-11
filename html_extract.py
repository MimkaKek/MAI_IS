import requests
from bs4 import BeautifulSoup
import time
import os
import re
import json
from mlogger import Logger

class HTMLExtractor:
    
    def __init__(self):
        
        self.buffer          = ""

        self.BASE_DIR        = "ruwiki/"
        self.pathToSave      = "backup.json"

        self.IGNORE_TITLE    = ["Категория:", "Файл:", "Шаблон:", "Википедия:", "Проект:"]
        self.IGNORE_TEXT     = ["#перенаправление", "#Перенаправление", "#ПЕРЕНАПРАВЛЕНИЕ", "#REDIRECT", "#Redirect", "#redirect"]

        self.total_html      = 0
        self.total_pages     = 0
        self.total_lines     = 0
        self.current_line    = 0

    def createHTML(self, title, text, name):

        for ignore_line in self.IGNORE_TITLE:
            if ignore_line in title:
                return
            
        for ignore_line in self.IGNORE_TEXT:
            if ignore_line in text:
                return

        htmlTemplate = "<!DOCTYPE html> <html> <body> <h1>" + title + "</h1> <p>" + text + "</p> </body> </html>"
        if not os.path.exists(self.BASE_DIR):
            os.makedirs(self.BASE_DIR)
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
        if os.path.exists(self.pathToSave):
            with open(self.pathToSave, "r") as backupFile:
                data = json.load(backupFile)
                self.total_html  = data["total_html"]
                self.total_pages = data["total_pages"]
                self.total_lines = data["total_lines"]
                self.buffer      = data["buffer"]

    def start(self):

        self.beforeStart()

        get_page_segment = False

        with open("ruwiki.xml", "r") as xmlf:
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
                        print("\r> | Total pages: " + str(self.total_pages) + " | Total HTML-docs: " + str(self.total_html) + " | Total lines: " + str(self.total_lines) + " |", end="")
                        self.total_pages += 1
                        title, text = self.parseXMLSegment(self.buffer)
                        self.createHTML(title, text, self.total_pages)
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

        print("Finished!\nTotal pages: " + str(self.total_pages) + "\nTotal html-files:" + str(self.total_html))

extractor = HTMLExtractor()
extractor.start()