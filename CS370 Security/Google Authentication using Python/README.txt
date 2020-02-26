Hsiang Lo 
Programming Assignment #2 Submission


In the implementation of my project assignment #2, i have decided to utilize python as the programming language of choice since it offers an extensive libraries such as hmac, hashlib and more importantly, it can generate qrcode fairly quickly. Since i've been trying to get better at Python, i decided to use this programming assignment as an opportunity to sharpen and enhance my skillset.

Furthermore, prior to starting out, i took a look at Google Authenticator's github page as suggested to better understand the tool. On there, they also provided a url example for Alice@google.com. I decided to use this url as the format/fit for my submission project. I edited the string to include my OSU Gmail account and changed the encryption/secret_k other than the one provided by Google. (Which i found out later, it is just 9-1 backward lol). I have tested --get-otp myself with DUO authenticator as used by Oregon State University, and can confirm that the numbers do in fact changes in given interval with my email in the informaiton section as well. There is no need for makefile or any other compilations tools.


Please let me know if you have any quesitons or concerns, thank you!

A) In order to generate the QR code, please enter the line below. The qr code will be saved as quickResponseCode.jpg in your directory where submission.py is 

	python submission.py --generate-qr


B) and to get the OTP, which will prompt to the terminal is 


	python submission.py --get-otp

