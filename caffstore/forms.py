from flask_wtf import FlaskForm
from flask_wtf.file import FileField, FileAllowed
from wtforms import StringField, SubmitField
from wtforms.validators import DataRequired, Length

class UploadCAFFForm(FlaskForm):
    title = StringField('Title', validators=[DataRequired()])
    picture = FileField('Upload CAFF file', validators=[FileAllowed(['jpg', 'png'])])
    submit = SubmitField('Upload')