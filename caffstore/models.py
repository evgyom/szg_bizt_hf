import decimal
from datetime import datetime
from flask_login import UserMixin
from caffstore import db, login_manager

@login_manager.user_loader
def load_user(user_id):
    return User.query.get(int(user_id))

class User(db.Model, UserMixin):
    __tablename__ = 'users'
    id = db.Column(db.Integer, primary_key=True)
    username = db.Column(db.String(20), unique=True, nullable=False)
    email = db.Column(db.String(120), unique=True, nullable=False)
    password = db.Column(db.String(60), nullable=False)
    balance = db.Column(db.DECIMAL(), default=decimal.Decimal(0))

    CAFFs = db.relationship('CAFF', backref='uploader', lazy=True)
    Comments = db.relationship('Comment', backref='author', lazy=True)
    roles = db.relationship('Role', secondary='user_roles')

    def __repr__(self):
        return f"User('{self.username}', '{self.email}', '{self.roles}')"

class Role(db.Model):
    __tablename__ = 'roles'
    id = db.Column(db.Integer(), primary_key=True)
    name = db.Column(db.String(50), unique=True)

class UserRoles(db.Model):
    __tablename__ = 'user_roles'
    id = db.Column(db.Integer(), primary_key=True)
    user_id = db.Column(db.Integer(), db.ForeignKey('users.id', ondelete='CASCADE'))
    role_id = db.Column(db.Integer(), db.ForeignKey('roles.id', ondelete='CASCADE'))


class CAFF(db.Model):
    __tablename__ = 'caffs'
    id = db.Column(db.Integer, primary_key=True)
    title = db.Column(db.String(100), nullable=False, default="untitled")
    creator_name = db.Column(db.String(100), nullable=False)
    creation_date = db.Column(db.DateTime, nullable=False, default=datetime.utcnow)
    captions = db.Column(db.String(200))
    tags = db.Column(db.String(200))
    duration = db.Column(db.Integer, nullable=False)

    price = db.Column(db.DECIMAL(), nullable=False)

    comments = db.relationship('Comment', backref='caff_img', lazy=True)

    preview_file = db.Column(db.String(32), nullable=False, default='no_preview.jpg')
    CAFF_file = db.Column(db.String(32), nullable=False, default='asd.CAFF')
    user_id = db.Column(db.Integer, db.ForeignKey('users.id'), nullable=False)

    def __repr__(self):
        return f"CAFF('{self.id}', {self.creator_name} ,'{self.duration}')"


class Comment(db.Model):
    __tablename__ = 'comments'
    id = db.Column(db.Integer, primary_key=True)
    date_posted = db.Column(db.DateTime, nullable=False, default=datetime.utcnow)
    content = db.Column(db.Text, nullable=False)

    user_id = db.Column(db.Integer, db.ForeignKey('users.id'), nullable=False) #author
    CAFF_id = db.Column(db.Integer, db.ForeignKey('caffs.id'), nullable=False) #caff_img

    def __repr__(self):
        return f"Comment('{self.id}', {self.date_posted} ,'{self.content}')"
