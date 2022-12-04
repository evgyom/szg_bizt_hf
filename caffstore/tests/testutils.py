
def statuscode_is_redirect(statuscode: int):
    return 300 <= statuscode < 400


def statuscode_is_ok(statuscode: int):
    return statuscode == 200


def statuscode_is_success(statuscode: int):
    return 200 <= statuscode < 300
