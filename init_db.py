from caffstore import db, bcrypt, app
from caffstore.models import Role, User, UserRoles

ctx = app.app_context()
ctx.push()

db.drop_all()
db.create_all()
db.session.commit()

admin = Role(name='Admin')
user = Role(name='User')

db.session.add(admin)
db.session.add(user)
db.session.commit()

hashed_password = bcrypt.generate_password_hash('admin').decode('utf-8')
admin_user = User(username='admin', email='admin@admin.com', password=hashed_password)

db.session.add(admin_user)
db.session.commit()

role = UserRoles(user_id=admin_user.id, role_id=admin.id)
db.session.add(role)
db.session.commit()