import decimal

from PIL import Image
import os
import secrets
from datetime import datetime
import ctypes

from flask import render_template, url_for, flash, redirect, request, abort
from caffstore import app, db, bcrypt
from caffstore.forms import UploadCAFFForm, RegistrationForm, LoginForm, CommentForm
from caffstore.models import CAFF, Comment, User, Role, UserRoles
from flask_login import login_user, current_user, logout_user, login_required

# c structure for the parser
BUF_SIZE = 512
class struct_out(ctypes.Structure):
    _fields_ = [("n_frames", ctypes.c_int32),  
                ("creator_name_length", ctypes.c_int32),
                ("creator_name", ctypes.c_char * BUF_SIZE),
                ("creation_year", ctypes.c_int32),
                ("creation_month", ctypes.c_int32),
                ("creation_day", ctypes.c_int32),
                ("creation_hour", ctypes.c_int32),
                ("creation_minute", ctypes.c_int32),
                ("captions", ctypes.c_char * BUF_SIZE),
                ("captions_length", ctypes.c_int32),
                ("tags", ctypes.c_char * BUF_SIZE),
                ("tags_length", ctypes.c_int32),
                ("n_tags", ctypes.c_int32),
                ("total_duration", ctypes.c_int32),
                ("frame_height", ctypes.c_int32),
                ("frame_width", ctypes.c_int32)]

PARSER_STATUS_OK = 0
PARSER_FP_NULL = 1
PARSER_BUFFER_SIZE_ERROR = 2
PARSER_EOF_REACHED	= 3
CAFF_FORMAT_ERROR = 4
CAFF_FIELD_SIZE_ERROR = 5
CAFF_UNDEFINED_SECTION = 6
PARSER_GENERAL_ERROR = 7

def check_role(role_name):
    roles = current_user.roles
    role_names = map(lambda r: r.name, roles)
    return role_name in role_names


@app.route("/")
@app.route("/home")
@login_required
def home():
    print(check_role("User"))
    caffs = CAFF.query.all()
    return render_template('home.html', items=caffs, title='About')


@app.route("/register", methods=['GET', 'POST'])
def register():
    if current_user.is_authenticated:
        return redirect(url_for('home'))
    form = RegistrationForm()
    if form.validate_on_submit():
        hashed_password = bcrypt.generate_password_hash(form.password.data).decode('utf-8')
        user = User(username=form.username.data, email=form.email.data, password=hashed_password)
        db.session.add(user)
        db.session.commit()

        role = Role.query.filter_by(name='User').first_or_404()
        user_role = UserRoles(user_id=user.id, role_id=role.id)
        db.session.add(user_role)
        db.session.commit()

        flash('Your account has been created! You are now able to log in', 'success')
        return redirect(url_for('home'))

    return render_template('register.html', title='Register', form=form)

@app.route("/login", methods=['GET', 'POST'])
def login():
    if current_user.is_authenticated:
        return redirect(url_for('home'))

    form = LoginForm()
    if form.validate_on_submit():
        user = User.query.filter_by(email=form.email.data).first()
        if user and bcrypt.check_password_hash(user.password, form.password.data):
            login_user(user, remember=form.remember.data)
            next_page = request.args.get('next')
            return redirect(next_page) if next_page else redirect(url_for('home'))
        else:
            flash('Login Unsuccessful. Please check email and password', 'danger')
    return render_template('login.html', title='Login', form=form)



@app.route("/about")
def about():
    return render_template('about.html', title='About')


def save_CAFF(CAFF_data):
    random_hex = secrets.token_hex(8)
    _, f_ext = os.path.splitext(CAFF_data.filename)

    caff_fn = "CAFF_" + str(random_hex) + f_ext
    picture_path = os.path.join(app.root_path, 'static\\CAFF_files', caff_fn)
    CAFF_data.save(picture_path)
    return caff_fn


@app.route("/upload", methods=['GET', 'POST'])
@login_required
def upload():
    form = UploadCAFFForm()
    if form.validate_on_submit():
        CAFFname = save_CAFF(form.picture.data)
        GIFname = CAFFname.replace("CAFF", "PREVIEW").replace(".caff",".gif")
        caff_in_path = os.path.join(app.root_path, 'static\\CAFF_files', CAFFname)
        gif_out_path = os.path.join(app.root_path, 'static\\Preview_files', GIFname) # Save to separate directory?

        # Load library
        lib_path = os.path.join(app.root_path, "libcaff_parser_shared.dll") 
        caff_parser = ctypes.CDLL(str(lib_path))

        # Define the input types
        caff_parser.parse.argtypes = [ctypes.c_char_p, ctypes.c_long, ctypes.c_char_p,ctypes.POINTER(struct_out)]

        # Call the function
        output_struct = struct_out()
        in_path = str(caff_in_path).encode("utf-8")
        out_path = str(gif_out_path).encode("utf-8")
        file_size = os.path.getsize(caff_in_path)
        retval = caff_parser.parse(in_path, file_size, out_path, ctypes.byref(output_struct))

        if(retval == PARSER_STATUS_OK):
            caff_creator_name = output_struct.creator_name[0:output_struct.creator_name_length].decode("UTF-8")
            caff_duration = output_struct.total_duration
            caff_caption = output_struct.captions[0:output_struct.captions_length].decode("UTF-8")
            caff_tags = output_struct.tags[0:output_struct.tags_length].decode("UTF-8") #.split("\n")

            # Validate max length / max duration ???

            caff = CAFF(title=form.title.data,
                        creator_name=caff_creator_name,
                        creation_date = datetime(output_struct.creation_year, output_struct.creation_month, output_struct.creation_day),
                        captions = caff_caption,
                        tags = caff_tags,
                        duration = caff_duration, 
                        preview_file = GIFname,
                        CAFF_file = CAFFname, 
                        user_id = current_user.id,
                        price = decimal.Decimal(form.price.data))

            db.session.add(caff)
            db.session.commit()
            flash("Upload Successful", "success")

        else:
            # CAFF has to be removed
            os.remove(caff_in_path)
            # If the preview is created, it also has to be removed
            try:
                os.remove(gif_out_path)
            except FileExistsError:
                pass

            if(retval == PARSER_FP_NULL):
                flash("Parser: file pointer error.", "danger")
            elif(retval == PARSER_BUFFER_SIZE_ERROR):
                flash("Parser: buffer size error.", "danger")
            elif(retval == PARSER_GENERAL_ERROR):
                flash("Parser: general error.", "danger")
            else:
                flash("Parser: format error.", "danger")

        return redirect(url_for('home'))
    else:
        pass
        #flash(form.errors)
    return render_template('upload.html', title='Upload CAFF', form=form)

@app.route("/caff_details/<int:caff_id>", methods=['GET', 'POST'])
def caff_details(caff_id):

    item = CAFF.query.get_or_404(caff_id)
    form = CommentForm()

    if form.validate_on_submit():
        comment = Comment(content=form.content.data, CAFF_id=caff_id, user_id=current_user.id)
        db.session.add(comment)
        db.session.commit()


    return render_template('details.html', title='CAFF details', item=item, form=form)


@app.route("/logout")
def logout():
    logout_user()
    return redirect(url_for('home'))

@app.route("/edit_userdata/<int:user_id>", methods=['GET', 'POST'])
def edit_userdata(user_id):
    if user_id != current_user.id and not check_role("Admin"):
        flash("You have to be admin to access this feature", "danger")
        return redirect(url_for('home'))

    print(user_id)
    return redirect(url_for('about'))






# @app.route("/login", methods=['GET', 'POST'])
# def login():
#     if current_user.is_authenticated:
#         return redirect(url_for('home'))
#     form = LoginForm()
#     if form.validate_on_submit():
#         user = User.query.filter_by(email=form.email.data).first()
#         if user and bcrypt.check_password_hash(user.password, form.password.data):
#             login_user(user, remember=form.remember.data)
#             next_page = request.args.get('next')
#             return redirect(next_page) if next_page else redirect(url_for('home'))
#         else:
#             flash('Login Unsuccessful. Please check email and password', 'danger')
#     return render_template('login.html', title='Login', form=form)
#
#
# @app.route("/logout")
# def logout():
#     logout_user()
#     return redirect(url_for('home'))
#
#
# def save_picture(form_picture):
#     random_hex = secrets.token_hex(8)
#     _, f_ext = os.path.splitext(form_picture.filename)
#     picture_fn = random_hex + f_ext
#     picture_path = os.path.join(app.root_path, 'static/profile_pics', picture_fn)
#
#     output_size = (125, 125)
#     i = Image.open(form_picture)
#     i.thumbnail(output_size)
#     i.save(picture_path)
#
#     return picture_fn
#
#
# @app.route("/account", methods=['GET', 'POST'])
# @login_required
# def account():
#     form = UpdateAccountForm()
#     if form.validate_on_submit():
#         if form.picture.data:
#             picture_file = save_picture(form.picture.data)
#             current_user.image_file = picture_file
#         current_user.username = form.username.data
#         current_user.email = form.email.data
#         db.session.commit()
#         flash('Your account has been updated!', 'success')
#         return redirect(url_for('account'))
#     elif request.method == 'GET':
#         form.username.data = current_user.username
#         form.email.data = current_user.email
#     image_file = url_for('static', filename='profile_pics/' + current_user.image_file)
#     return render_template('account.html', title='Account',
#                            image_file=image_file, form=form)
#
#
# @app.route("/post/new", methods=['GET', 'POST'])
# @login_required
# def new_post():
#     form = PostForm()
#     if form.validate_on_submit():
#         post = Post(title=form.title.data, content=form.content.data, author=current_user)
#         db.session.add(post)
#         db.session.commit()
#         flash('Your post has been created!', 'success')
#         return redirect(url_for('home'))
#     return render_template('create_post.html', title='New Post',
#                            form=form, legend='New Post')
#
#
# @app.route("/post/<int:post_id>")
# def post(post_id):
#     post = Post.query.get_or_404(post_id)
#     return render_template('post.html', title=post.title, post=post)
#
#
# @app.route("/post/<int:post_id>/update", methods=['GET', 'POST'])
# @login_required
# def update_post(post_id):
#     post = Post.query.get_or_404(post_id)
#     if post.author != current_user:
#         abort(403)
#     form = PostForm()
#     if form.validate_on_submit():
#         post.title = form.title.data
#         post.content = form.content.data
#         db.session.commit()
#         flash('Your post has been updated!', 'success')
#         return redirect(url_for('post', post_id=post.id))
#     elif request.method == 'GET':
#         form.title.data = post.title
#         form.content.data = post.content
#     return render_template('create_post.html', title='Update Post',
#                            form=form, legend='Update Post')
#
#
# @app.route("/post/<int:post_id>/delete", methods=['POST'])
# @login_required
# def delete_post(post_id):
#     post = Post.query.get_or_404(post_id)
#     if post.author != current_user:
#         abort(403)
#     db.session.delete(post)
#     db.session.commit()
#     flash('Your post has been deleted!', 'success')
#     return redirect(url_for('home'))