# IMPORT all essentials including qrcode, hmac, time-related and decoding/coding tools
import sys
import datetime
import base64
import qrcode
import time
import hmac
import hashlib


#example provided on the git for Google Authenticator, modified using my email and my own secret key
googleSample = 'otpauth://totp/Example:lohs@oregonstate.edu?secret=ABCDEFGHIJKLMNAO&issuer=Example&period=30'

#If provided input of argument 1 is to get otp
# The cryptographic parts of RFC4226 (counter-based OTP) or draft-mraihi-totp-timebased (time-based OTP)
# are relatively simple:
# 1. Generate a HMAC based on the shared-key and the counter/time
# 2. Truncate it in a secure way
if sys.argv[1] == '--get-otp':

    #use current time as a randomizer
    currentTime = datetime.datetime.now()

    #Objectifying time using timetuple
    objOfTime = time.mktime(currentTime.timetuple())


    # double // for precision
    divByThirty = objOfTime // 30

    finalObj = int(divByThirty)

    #turn into byte array
    final_array = bytearray()
    
    #look through to append 
    while finalObj != 0:

        #Append hexadecimal number FF or integer value of 255
        final_array.append(finalObj & 0xff)

        #Shifting variable to the right by 8 bits
        finalObj >>= 8

    #Fill remaining right side with null
    result = bytes(bytearray(reversed(final_array)).rjust(8, b'\0'))

    #secre key as referenced from the googleSample but modified
    secret_k = base64.b32decode('ABCDEFGHIJKLMNAO')
    
    hashResult = hmac.new(secret_k, result, hashlib.sha1)
    hResult = bytearray(hashResult.digest())
    
    #reference stockoverflow for truncating 
    spaceBetween = hResult[19] & 0xf
    identityC = ((hResult[spaceBetween] & 0x7f) << 24 |
                (hResult[spaceBetween + 1] & 0xff) << 16 |
                (hResult[spaceBetween + 2] & 0xff) << 8 |
                (hResult[spaceBetween + 3] & 0xff))

    #final result 
    finalString = str(identityC % 10 ** 6)

    #increment/fill
    while len(finalString) < 6:
        finalString = '0' + finalString

    #output result
    print(finalString)

#If user ask to generate qr code
#fairly straight forward, use qrcode libaries to call qrcode.make
if sys.argv[1] == '--generate-qr':
    #save the result of the qr code
    googleQrCode = qrcode.make(googleSample)
    googleQrCode.save('quickResponseCode.jpg')
    
