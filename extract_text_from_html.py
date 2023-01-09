from bs4 import BeautifulSoup
from sys import argv
import re
import os
import time
from collections import Counter
import pymorphy2

morph = pymorphy2.MorphAnalyzer()
total_tokens = Counter()

def ExtractTokens(filename):
    html_content = ""
    
    with open(filename, "r") as file:
        html_content = file.read()
        
        soup = BeautifulSoup(html_content, 'html.parser')
        text = soup.find_all(text=True)
        title = soup.h1.text
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
            '\s+',
            '  '
        ]
        for rule in regexp_rules:
            reg = re.compile(rule)
            output = reg.sub(' ', output)

        output = output.split()
        counter = Counter()
        tmp = []
        for word in output:
            p = morph.parse(word)[0]
            counter.update([p.normal_form])

        total_tokens.update(counter)

        fileToSave = filename[:-4] + "data"
        i = -1
        while fileToSave[i] != '/':
            i = i - 1

        fileToSave = os.path.join(fileToSave[:i], "data", fileToSave[i+1:])

        if not os.path.exists(fileToSave[:i]):
            os.makedirs(fileToSave[:i])
        with open(fileToSave, "w") as save:
            save.write(title + "\n")
            for key, value in counter.items():
                save.write(key + "\n" + str(value) + "\n")

def main():
    if len(argv) > 1:
        roots = []
        for arg in argv:
            if arg == argv[0]:
                continue
            roots.append(arg)
            
        for root in roots:
            # if not os.path.exists(os.path.join(root, "html")):
            #     os.makedirs(os.path.join(root, "html"))
            
            # if not os.path.exists(os.path.join(root, "data")):
            #     os.makedirs(os.path.join(root, "data"))

            # onlyfiles = [f for f in os.listdir(os.path.join(root, "html")) if os.path.isfile(os.path.join(root, "html", f))]
            # for file in onlyfiles:
            #     print("\33[2K\rReplace old html to root... " + os.path.join(root, "html", file) + " to " + os.path.join(root, file), end="")
            #     os.replace(os.path.join(root, "html", file), os.path.join(root, file))
            # print("")

            # for filename in os.listdir(os.path.join(root, "data")):
            #     print("\33[2K\rRemove old .data files... " + os.path.join(root, "data", filename), end="")
            #     file_path = os.path.join(root, "data", filename)
            #     try:
            #         if os.path.isfile(file_path) or os.path.islink(file_path):
            #             os.unlink(file_path)
            #     except Exception as e:
            #         print('Failed to delete %s. Reason: %s' % (file_path, e))
            # print("")

            print("Start extract...")
            print("========================")
            onlyfiles = [f for f in os.listdir(root) if os.path.isfile(os.path.join(root, f))]
            begin_time = time.time()
            tm_hour    = 0
            tm_min     = 0
            tm_sec     = 0
            for file in onlyfiles:
                delta_time = time.gmtime(time.time() - begin_time)
                tm_hour    = delta_time.tm_hour
                tm_min     = delta_time.tm_min
                tm_sec     = delta_time.tm_sec
                print("\33[2K\r| Total time: {hour:02d}:{min:02d}:{sec:02d} | Extract {path}...".format(hour=tm_hour, min=tm_min, sec=tm_sec, path=os.path.join(root, file)), end="")
                ExtractTokens(os.path.join(root, file))
                os.replace(os.path.join(root, file), os.path.join(root, "html", file))
            print("\33[2K\r| Total time: {hour:02d}:{min:02d}:{sec:02d} |".format(hour=tm_hour, min=tm_min, sec=tm_sec))
            print("========================")
        
        print("Total token count: {total:13d}".format(total=len(total_tokens)))
        with open("total.data", "w") as file:
            for key, value in total_tokens.items():
                file.write(key + " " + str(value) + "\n")

    else:
        print("Example usage: python " + argv[0] + " [dir_path...]")

if __name__ == "__main__":
    main()