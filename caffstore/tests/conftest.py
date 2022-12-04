import pytest
from caffstore import app
from flask import session
from flask_login import login_user
from pprint import pprint
from caffstore.routes import do_login

@pytest.fixture()
def client():
    app.config.update({
        "TESTING": True,
    })

    with app.test_client() as test_client:
        with app.app_context():
            yield test_client


@pytest.fixture()
def client_admin():
    app.config.update({
        "TESTING": True,
    })

    with app.test_client() as test_client:
        with app.app_context():
            test_client.get("login")
            do_login("admin@admin.com", "admin", False)
            yield test_client


@pytest.fixture()
def client_user():
    app.config.update({
        "TESTING": True,
    })

    with app.test_client() as test_client:
        with app.app_context():
            test_client.get("login")
            do_login("user@user.com", "user", False)
            yield test_client
