from caffstore import app
import os

if __name__ == '__main__':
    cert = os.path.join(app.root_path, 'cert\\server.crt')
    key = os.path.join(app.root_path, 'cert\\server.key')
    app.run(debug=True, ssl_context=(cert, key))
