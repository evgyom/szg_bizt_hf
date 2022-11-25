from datetime import datetime
from caffstore import db

class CAFF(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    title = db.Column(db.String(100), nullable=False, default="untitled")
    creator_name = db.Column(db.String(100), nullable=False)
    creation_date = db.Column(db.DateTime, nullable=False, default=datetime.utcnow)
    captions = db.Column(db.String(200))
    tags = db.Column(db.String(200))
    duration = db.Column(db.Integer, nullable=False)

    comments = db.relationship('Comment', backref='caff_img', lazy=True)

    preview_file = db.Column(db.String(32), nullable=False, default='no_preview.jpg')
    CAFF_file = db.Column(db.String(32), nullable=False, default='asd.CAFF')
    #user_id = db.Column(db.Integer, db.ForeignKey('user.id'), nullable=False)

    def __repr__(self):
        return f"CAFF('{self.id}', {self.creator_name} ,'{self.duration}')"


class Comment(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    date_posted = db.Column(db.DateTime, nullable=False, default=datetime.utcnow)
    content = db.Column(db.Text, nullable=False)

    #user_id = db.Column(db.Integer, db.ForeignKey('user.id'), nullable=False)
    CAFF_id = db.Column(db.Integer, db.ForeignKey('caff.id'), nullable=False)

    def __repr__(self):
        return f"Comment('{self.id}', {self.date_posted} ,'{self.content}')"