from bs4 import BeautifulSoup
from sys import argv
import re
import os
from pathlib import Path

if len(argv) > 1:
    html_content = ""
    for arg in argv:
        if arg == argv[0]:
                continue
        try:
            with open(arg, "r") as file:
                html_content = file.read()
        except Exception:
            exit(1)
        
        soup = BeautifulSoup(html_content, 'html.parser')
        text = soup.find_all(text=True)

        output = ''

        blacklist = [
            '[document]',
            'noscript',
            'header',
            'html',
            'meta',
            'head', 
            'input',
            'script',
        ]

        skips = [
            '\n', '|', '=', 'http://', '[', ']', '{', '}', '\\', '*', '_', '/', '.', ','
        ]
        for t in text:
            if t.parent.name not in blacklist:
                output += '{} '.format(t)

        output = output.lower()
        reg = re.compile(' //.*?\n')
        output = reg.sub('', output)
        output = output.replace("\n", " ")

        counter = 0
        tmp = ''
        for ch in output:
            if ch == '{':
                counter += 1
            elif ch == '}':
                counter -= 1
            elif counter < 2:
                tmp += ch
        output = tmp

        regexp_rules = [
            '\[\[(категория:|file:).*?]]',
            '==.*?==',
            '\[http.*?]',
            '[^0-9 а-яё-]',
            '\d+\D*? ',
            '\s+'
        ]
        for rule in regexp_rules:
            reg = re.compile(rule)
            output = reg.sub(' ', output)

        reg = re.compile(' (.|..|...) ')
        for i in range(5):
            output = reg.sub(' ', output)
        
        output = output.split()
        output = list(dict.fromkeys(output))
        fileToSave = arg[:-4] + "data"
        i = -1
        while fileToSave[i] != '/':
            i = i - 1
        fileToSave = fileToSave[:i] + "/data" + fileToSave[i:]
        if not os.path.exists(fileToSave[:i-1] + "/data"):
            os.makedirs(fileToSave[:i-1] + "/data")
        with open(fileToSave, "w") as save:
            for token in output:
                save.write(token + "\n")