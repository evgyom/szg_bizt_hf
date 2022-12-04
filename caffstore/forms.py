import decimal
from flask_wtf import FlaskForm
from flask_wtf.file import FileField, FileAllowed
from wtforms import StringField, SubmitField, PasswordField, BooleanField, TextAreaField, DecimalField
from wtforms.validators import DataRequired, Length, Email, EqualTo, ValidationError
from caffstore.models import User
from flask_login import current_user

class UploadCAFFForm(FlaskForm):
    title = StringField('Title', validators=[DataRequired()])
    price = DecimalField(places=2, rounding=decimal.ROUND_UP, validators=[DataRequired()])
    picture = FileField('Upload CAFF file', validators=[FileAllowed(['caff'])])
    submit = SubmitField('Upload')

    def validate_price(self, price):
        Validators.validate_price(price.data)

class EditCAFFForm(FlaskForm):
    title = StringField('Title', validators=[DataRequired()])
    price = DecimalField(places=2, rounding=decimal.ROUND_UP, validators=[DataRequired()])
    submit = SubmitField('Update CAFF')

    def validate_price(self, price):
        Validators.validate_price(price.data)


class EditUserdataForm(FlaskForm):
    username = StringField('Username', validators=[DataRequired(), Length(min=4, max=24)])
    email = StringField('Email', validators=[DataRequired(), Email()])
    password = PasswordField('New password')
    confirm_password = PasswordField('Confirm new password', validators=[EqualTo('password', message="Passwords must match!")])
    submit = SubmitField('Submit')
    delete = SubmitField('Delete Account')

    def validate_username(self, username):
        Validators.validate_username(username.data)

    def validate_email(self, email):
        Validators.validate_email(email.data)

    def validate_password(self, password):
        Validators.validate_password(password.data)

class EditUserdataAdminForm(FlaskForm):
    username = StringField('Username', validators=[DataRequired(), Length(min=4, max=24)])
    email = StringField('Email', validators=[DataRequired(), Email()])
    password = PasswordField('New password')

    balance = DecimalField(places=2, rounding=decimal.ROUND_UP)
    admin = BooleanField('Is admin')

    submit = SubmitField('Submit')

    def validate_username(self, username):
        Validators.validate_username(username.data)

    def validate_email(self, email):
        Validators.validate_email(email.data)

    def validate_password(self, password):
        Validators.validate_password(password.data)

class CreateUserForm(FlaskForm):
    username = StringField('Username', validators=[DataRequired(), Length(min=4, max=24)])
    email = StringField('Email', validators=[DataRequired(), Email()])
    password = PasswordField('Password', validators=[DataRequired()])
    confirm_password = PasswordField('Confirm Password', validators=[DataRequired(), EqualTo('password')])
    balance = DecimalField(places=2, rounding=decimal.ROUND_UP, default=0)
    submit = SubmitField('Create')

    def validate_username(self, username):
        Validators.validate_username(username.data)

    def validate_email(self, email):
        Validators.validate_email(email.data)

    def validate_password(self, password):
        Validators.validate_password(password.data)

class RegistrationForm(FlaskForm):
    username = StringField('Username', validators=[DataRequired(), Length(min=4, max=24)])
    email = StringField('Email', validators=[DataRequired(), Email()])
    password = PasswordField('Password', validators=[DataRequired()])
    confirm_password = PasswordField('Confirm Password', validators=[DataRequired(), EqualTo('password')])
    submit = SubmitField('Sign Up')

    def validate_username(self, username):
        Validators.validate_username(username.data)

    def validate_email(self, email):
        Validators.validate_email(email.data)

    def validate_password(self, password):
        Validators.validate_password(password.data)

class LoginForm(FlaskForm):
    email = StringField('Email',
                        validators=[DataRequired(), Email()])
    password = PasswordField('Password', validators=[DataRequired()])
    remember = BooleanField('Remember Me')
    submit = SubmitField('Login')

class CommentForm(FlaskForm):
    content = TextAreaField('Post a comment', validators=[DataRequired()])
    submit = SubmitField('Post')

class SearchForm(FlaskForm):
    search_key = StringField('Search for a CAFF', validators=[DataRequired()], render_kw={"placeholder": "Enter CAFF name"})
    submit = SubmitField('Search')

class Validators():
    def validate_password(pw):
        l, u, d = 0, 0, 0
        message = ""
        if len(pw) < 6 or len(pw) > 20:
            message += 'The password must be between 6 and 20 characters long! '
        for c in pw:
            if c.islower():
                l += 1
            if c.isupper():
                u += 1
            if c.isdigit():
                d += 1
        if l == 0:
            message += 'The password must contain at least 1 lowercase letter! '
        if u == 0:
            message += 'The password must contain at least 1 uppercase letter! '
        if d == 0:
            message += 'The password must contain at least 1 digit! '
        if message != "":
            raise ValidationError(message)

    def validate_username(username):
        user = User.query.filter_by(username=username).first()
        if user:
            raise ValidationError('That username is taken. Please choose a different one.')

    def validate_email(email):
        user = User.query.filter_by(email=email).first()
        if user:
            raise ValidationError('That email is taken. Please choose a different one.')

    def validate_price(price):
        if price.data < decimal.Decimal(0):
            raise ValidationError('Price must be positive')