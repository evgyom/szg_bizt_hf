from testutils import *


def test_edit_user_redirect_user(client_user):
    res = client_user.get("edit_user")
    assert statuscode_is_redirect(res.status_code)
    assert b'<a href="/edit_userdata">' in res.data


def test_logout_user(client_user):
    res = client_user.get("logout")
    assert statuscode_is_redirect(res.status_code)
    res = client_user.get("edit_userdata")
    assert not statuscode_is_success(res.status_code)
    assert b"Please log in to access this page" in res.data  # TODO: Follow the redirect and arrive here

