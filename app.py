import socket
import time

from flask import Flask, request, render_template, url_for
from flask_wtf import Form
from wtforms import StringField

class SearchForm(Form):
    search = StringField('...')
    
HOST, PORT = "localhost", 50007

tokensToTranslations = dict()
translationsToTokens = dict()
TOTAL_FILEPATH = "total.data"

app = Flask(__name__)
app.config.from_object(__name__)

@app.route('/open_doc/<doc_url>')
def open_doc(doc_url):
    with open("./templates/ruwiki/html/" + doc_url + ".html", "r") as file:
        html_content = file.read()
        print(html_content)
        return html_content

@app.route('/', methods=['GET', 'POST'])
def main():
    search = SearchForm()
    if request.method == 'POST':
        begin_time = time.time()
        to_search = request.form['search'].lower()

        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect((HOST, PORT))
        s.send(bytes(to_search, 'utf-8'))
        tokenAlt = []
        while(True):
            strData = str(s.recv(1000), 'utf-8')
            s.send(bytes("OK", 'utf-8'))
            if (strData == "END"):
                break
            tokenAlt.append(strData)
            
        data = []
        while(True):
            strData = str(s.recv(1000), 'utf-8')
            s.send(bytes("OK", 'utf-8'))
            if (strData == "END"):
                break
            strData = strData.split("&&&")
            data.append({"filepath": strData[0], "title": strData[1], "score": strData[2]})
        s.send(bytes("FIN", 'utf-8'))

        delta_time = time.time() - begin_time
        return render_template("results.html", links=data, dTime=delta_time, alternates=tokenAlt)
    else:
        return render_template("index.html", form=search)

if __name__ == '__main__':
    
    with open(TOTAL_FILEPATH, "r") as file:
        for line in file:
            arr = line.split()
            token = arr[0]
            tr = " ".join(arr[1:-1])
    
    app.run("localhost", port=5001, threaded=False, debug=True)