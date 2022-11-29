import decimal
from flask_wtf import FlaskForm
from flask_wtf.file import FileField, FileAllowed
from wtforms import StringField, SubmitField, PasswordField, BooleanField, TextAreaField, DecimalField
from wtforms.validators import DataRequired, Length, Email, EqualTo, ValidationError
from caffstore.models import User

class UploadCAFFForm(FlaskForm):
    title = StringField('Title', validators=[DataRequired()])
    price = DecimalField(places=2, rounding=decimal.ROUND_UP, validators=[DataRequired()])
    picture = FileField('Upload CAFF file', validators=[FileAllowed(['caff'])])
    submit = SubmitField('Upload')

    def validate_price(self, price):
        if price.data < decimal.Decimal(0):
            raise ValidationError('Price must be positive')


class RegistrationForm(FlaskForm):
    username = StringField('Username', validators=[DataRequired(), Length(min=4, max=24)])
    email = StringField('Email', validators=[DataRequired(), Email()])
    password = PasswordField('Password', validators=[DataRequired()])
    confirm_password = PasswordField('Confirm Password', validators=[DataRequired(), EqualTo('password')])
    submit = SubmitField('Sign Up')

    def validate_username(self, username):
        user = User.query.filter_by(username=username.data).first()
        if user:
            raise ValidationError('That username is taken. Please choose a different one.')

    def validate_email(self, email):
        user = User.query.filter_by(email=email.data).first()
        if user:
            raise ValidationError('That email is taken. Please choose a different one.')

class LoginForm(FlaskForm):
    email = StringField('Email',
                        validators=[DataRequired(), Email()])
    password = PasswordField('Password', validators=[DataRequired()])
    remember = BooleanField('Remember Me')
    submit = SubmitField('Login')

class CommentForm(FlaskForm):
    content = TextAreaField('Post a comment', validators=[DataRequired()])
    submit = SubmitField('Post')