from testutils import *


def test_edit_user_redirect_admin(client_admin):
    res = client_admin.get("edit_user")
    assert statuscode_is_redirect(res.status_code)
    assert b'<a href="/manage_users">' in res.data

