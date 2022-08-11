import smtplib
from email.message import EmailMessage
import imghdr

sender_mail = "nispk1506@gmail.com"
receiver_mail = "nishadkanago@gmail.com"
password = "renukama"

f = open('C:\\Users\\HP\\Documents\\COEP\\C programing\\DSA\\Project\\meeting_clashes\\ids.txt', 'r')
x = f.readline()
l = x.split(' ')

msg = EmailMessage()
msg['Subject'] = 'Meeting Schedule'
msg['From'] = sender_mail
msg['To'] = l
msg.set_content('Hello !\nThis is your todays meeting schedule\n\n   Please have a look as per your userIds')

files = ['C:\\Users\\HP\\Documents\\COEP\\C programing\\DSA\\Project\\meeting_clashes\\t1s.txt', 
'C:\\Users\\HP\\Documents\\COEP\\C programing\\DSA\\Project\\meeting_clashes\\t2s.txt',
'C:\\Users\\HP\\Documents\\COEP\\C programing\\DSA\\Project\\meeting_clashes\\t3s.txt',
'C:\\Users\\HP\\Documents\\COEP\\C programing\\DSA\\Project\\meeting_clashes\\t4s.txt']

i = 0
for file in files:
    with open(file, 'rb') as f:
        file_data = f.read()
        file_name = f"{l[i]}.txt"
    i = i + 1

    msg.add_attachment(file_data, maintype = 'application', subtype = 'octet-stream', filename = file_name)

with smtplib.SMTP('smtp.gmail.com', 587) as smtp:
    smtp.ehlo()
    smtp.starttls()
    smtp.ehlo()
    smtp.login(sender_mail, password)

    smtp.send_message(msg)

print("mail sent")