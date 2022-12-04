from testutils import *


def test_opening(client):
    res = client.get('/')
    assert statuscode_is_redirect(res.status_code)
    assert b"Redirecting" in res.data
    assert b"/login" in res.data


def test_register(client):
    res = client.get("register")
    assert statuscode_is_ok(res.status_code)
    assert b"Join Today" in res.data
    assert b"Confirm Password" in res.data
    assert b"Already Have An Account?" in res.data


def test_about(client):
    res = client.get('about')
    assert statuscode_is_success(res.status_code)
    assert b"About Page" in res.data

