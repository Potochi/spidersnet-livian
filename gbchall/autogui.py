import os
import subprocess

import pyautogui


def send(bits):
    for letter in bits:
        pyautogui.keyDown(letter)
        pyautogui.sleep(0.01)
        pyautogui.keyUp(letter)
        pyautogui.sleep(0.01)


def send_string(string):
    print(f"Sending {string}...")
    for ch in string:
        bits = "".join(["x" if ch == "1" else "z" for ch in f"{ord(ch):08b}"])
        send(bits)

    send("zzzzzzzz")

    print("Done")


if __name__ == "__main__":
    pyautogui.sleep(3)
    send("xzxxzzxx")

    send_string("cat flag.pdf")
    pyautogui.sleep(5)
    send_string("ls")
    pyautogui.sleep(5)
    send_string("su")
    pyautogui.sleep(5)
    send_string("Cr0wD57rIk3")
    pyautogui.sleep(6)
    send_string("cat flag.pdf")

    pyautogui.sleep(1000)
