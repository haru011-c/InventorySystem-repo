import os

def clear_screen():
    os.system("cls" if os.name == "nt" else "clear")

def pause_and_clear():
    input("\n  Press ENTER to continue...")
    clear_screen()

while True:
    choice = input("1. View Catalog\n2. Exit\nEnter choice: ")
    if choice == "2":
        break
    print("\n(catalog output here)")
    pause_and_clear()
