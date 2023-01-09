import socket
import time

from flask import Flask, request, render_template, url_for
from flask_wtf import Form
from wtforms import StringField
import pymorphy2

morph = pymorphy2.MorphAnalyzer()

class SearchForm(Form):
    search = StringField('...')
    
HOST, PORT = "localhost", 50007

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
        to_search = request.form['search'].lower().split()
        corr_search = []
        for token in to_search:
            if token[0] == "(" or token[0] == "!":
                p = morph.parse(token[1:])[0]
                corr_search.append(token[0] + p.normal_form)
            elif token[-1] == ")":
                p = morph.parse(token[:-1])[0]
                corr_search.append(p.normal_form + token[-1])
            else:
                corr_search.append(token)

        to_search = " ".join(corr_search)

        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect((HOST, PORT))
        s.send(bytes(to_search, 'utf-8'))
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
        return render_template("results.html", links=data, dTime=delta_time)
    else:
        return render_template("index.html", form=search)

if __name__ == '__main__':
    
    app.run("localhost", port=5001, threaded=False, debug=True)