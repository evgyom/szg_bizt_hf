from caffstore import app
from waitress import serve
import os

if __name__ == '__main__':
    #cert = os.path.join(app.root_path, 'cert\\server.crt')
    #key = os.path.join(app.root_path, 'cert\\server.key')
    serve(app, host="0.0.0.0", port=5000)
