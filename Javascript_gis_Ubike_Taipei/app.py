#TEST

from flask import Flask, render_template
import subprocess

#app = Flask(__name__)
app = Flask(__name__, template_folder="./")

@app.route("/")
def index():
    return render_template("index.html")  # this will load templates/index.html

@app.route("/api/message")
def message():
    #return {"message": "Hello from Flask!"} #response => response.text()
    return "Hello world for python!" #response => response.text()

@app.route("/run")
def run_c():
    # Run the compiled C program and get its output
    #result = subprocess.run(["./hello.o"], capture_output=True, text=True)
    result = subprocess.run(["./url2file.o"], capture_output=True, text=True)
    return result.stdout or result.stderr
    
if __name__ == "__main__":
    app.run(port=8080, debug=True)


