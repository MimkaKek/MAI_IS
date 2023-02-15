from bs4 import BeautifulSoup
from sys import argv
import re
import os
import time
from collections import Counter
import pymorphy2
import translators.server as tss

morph = pymorphy2.MorphAnalyzer()
total_tokens = Counter()
translated_tokens = dict()
not_translated_tokens = dict()
CHUNK_SIZE = 5000

def main():
    if len(argv) > 1:
        print("Reading {file}...".format(file=argv[1]))
        with open(argv[1], "r") as file:
            for line in file:
                buffer = line.split(" ")
                token = buffer[0]
                count = int(buffer[1])
                total_tokens.update({token: count})
        
        print("Reading {file}...".format(file=argv[2]))
        with open(argv[2], "r") as file:
            for line in file:
                buffer = line.split(" ")
                token = buffer[0]
                tr = " ".join(buffer[1:-1])
                count = int(buffer[-1])
                if total_tokens.get(token) != None:
                    translated_tokens.update({token: [tr, count]})
        
        for key, value in total_tokens.items():
            if translated_tokens.get(key) == None:
                not_translated_tokens.update({key: value})
        
        print(len(not_translated_tokens))
        
        text = ""
        print("Begin translate...")
        begin_time = time.time()
        count = len(translated_tokens)
        with open(argv[2], "a") as file:
            for key, value in not_translated_tokens.items():
            
                newTextSize = len(text + key + "|")
                
                if newTextSize < CHUNK_SIZE:
                    text += key + "\n"
                    count += 1
                else:
                    tr = tss.google(text, from_language="ru", to_language="en")
                    tr = tr.split("\n")
                    text = text.split("\n")
                    
                    pos = 0
                    output = ""
                    
                    while pos < len(tr):
                        output += text[pos].strip() + " " + tr[pos].strip().lower() + " " + str(value) + "\n"
                        pos += 1
                    
                    file.write(output)
                    
                    text = key + "|"
                    count += 1
            
            
            delta_time = time.gmtime(time.time() - begin_time)
            tm_hour    = delta_time.tm_hour
            tm_min     = delta_time.tm_min
            tm_sec     = delta_time.tm_sec
            print("\33[2K\r| Total time: {hour:02d}:{min:02d}:{sec:02d} | Translating {n:10d} of {total:10d}...".format(hour=tm_hour, min=tm_min, sec=tm_sec, n=count, total=len(total_tokens)), end="")
            
            total_tries = 3
            tryN = 0
            
            while True:
                try:
                    tr = tss.google(text, from_language="ru", to_language="en")
                    break
                except Exception as e:
                    if tryN < total_tries:
                        tryN += 1
                        print("Error during translate received:")
                        print(e)
                        print("Retry... {n:1d}".format(n=tryN))
                        time.sleep(1.0)
                    else:
                        break
            
            tr = tr.split("\n")
            text = text.split("\n")
            pos = 0
            output = ""
            
            while pos < len(tr):
                output += text[pos].strip() + " " + tr[pos].strip().lower() + " " + str(value) + "\n"
                pos += 1
            
            file.write(output)

    else:
        print("Example usage: python " + argv[0] + " [dir_path...]")

if __name__ == "__main__":
    main()
