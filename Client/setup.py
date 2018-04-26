# Written by Jan "Duchy" Neduchal 2018
import getpass # password prompt
import sys

from Crypto import Random
from Crypto.Cipher import AES
import base64

from main import copy_to_clipboard, hash_SHA256, xor


def encrypt(message, passphrase):
    IV = Random.new().read(BLOCK_SIZE)
    aes = AES.new(passphrase[:32], AES.MODE_CFB, IV)
    return base64.b64encode(aes.encrypt(message))


def main():
    print("Hello and welcome to RFID PW Manager ver 1.0")
    print("Now you're gonna setup a master password")
    print("it will be cryptographically combined with the selected UID")
    while True:
        print()
        print("Please enter the password now")
        password_plain = getpass.getpass("> ")
        if len(password_plain) < 6:
            print()
            print("The selected password is too short and thus insecure.")
            continue
        print("Re-enter the password")
        pass_reenter = getpass.getpass("> ")
        if password_plain == pass_reenter:
            break
        else:
            print()
            print("Passwords dont't match, try again.")
    print("For security pourposes, you need to hardcode the password hash to the arduino manually")
    print("Here is the SHA256 hash of the entered password, write it to the sketch ASAP")
    print("Without it you wont't be able to decrypt the passwords")
    password_hashed = hash_SHA256(password_plain)
    while True:
        choice = input("Do you want me to copy it to clipboard or print it?  c/p\n >")
        if choice == "c" or "C":
            copy_to_clipboard(password_hashed)
        elif choice == "p" or "P":
            print(password_hashed)
        else:
            continue
    print("Now we can encrypt the passwords using the master key and an UID")
    print("Use a default example sketch in the arduino PN532 library to read the UID")
    print("Please enter the UID you want to use to encrypt your passwords")
    while True:
        uid = input("> ")
        if len(uid) == 8:
            break
        else:
            print("Wrong UID format, it needs to be XXXX (4 bytes)")
    print("Hashing and generating the SECRET to encrypt the passwords with...")
    SECRET = xor(hash_SHA256(uid), password_hashed)
    print("Secret generated, now let's encrypt the passwords with it!")
    print("CTRL + C to exit")
    dict={}
    while True:
        try:
            name = input("Enter a name for the entry: ")
            if dict.has_key:
                print("Name is already on the list, enter another one!")
                continue
            while True:
                password_plain = getpass.getpass("Enter a corresponding password: ")
                pass_reenter = getpass.getpass("Enter a corresponding password again: ")
                if password_plain == pass_reenter:
                    dict[name] = encrypt(password_plain, SECRET)
                    break
                else:
                    print("Passwords don't match, try again!\n")
                    continue
        except KeyboardInterrupt:
            with open("config.py") as f:
                f.write("pass_dict = "+dict)
                print("Saved!")
                sys.exit(0)


if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        sys.exit(0)
