import pickle
import time
import threading
import random
import tkinter as tk
from tkinter import messagebox
#from datetime import datetime

MAX_ACCOUNTS = 100
MinBal = 5000

class Account:
    def __init__(self, account_number, name, amount, pin):
        self.account_number = account_number
        self.name = name
        self.amount = amount
        self.pin = pin

class Transaction:
    def __init__(self, account_number, transaction_type, amount):
        self.account_number = account_number
        self.transaction_type = transaction_type
        self.amount = amount
        self.timestamp = time.time()

accounts = []
n = 0
lock = threading.Lock()

def load_accounts():
    global accounts, n
    try:
        with open("accounts.dat", "rb") as file:
            accounts = pickle.load(file)
            n = len(accounts)
    except FileNotFoundError:
        accounts = []

def save_accounts():
    with open("accounts.dat", "wb") as file:
        pickle.dump(accounts, file)

def logs(account_number, transaction_type, amount):
    with open("logs.dat", "ab") as file:
        transaction = Transaction(account_number, transaction_type, amount)
        pickle.dump(transaction, file)

def send_charge(amount):
    if 2001 <= amount <= 99999:
        return amount + 1000
    elif 100000 <= amount <= 999999:
        return amount + 3800
    elif amount >= 1000000:
        return amount + 5000
    return amount

def withdraw_charge(amount):
    if 2001 <= amount <= 99999:
        return amount + 900
    elif 100000 <= amount <= 999999:
        return amount + 2800
    elif amount >= 1000000:
        return amount + 4500
    return amount

def generate_otp():
    return str(random.randint(100000, 999999))

def verify_otp(user_input, correct_otp):
    return user_input == correct_otp

def create_account(account_number, name, initial_amount, pin):
    global accounts, n
    if n >= MAX_ACCOUNTS:
        return "Max account limit reached"
    if initial_amount < MinBal:
        return "Initial deposit below minimum"
    new_acc = Account(account_number, name, initial_amount, pin)
    accounts.append(new_acc)
    n += 1
    save_accounts()
    return "Account created successfully"

def find_account(acc_num):
    for acc in accounts:
        if acc.account_number == acc_num:
            return acc
    return None

# ---------- TKINTER GUI -----------

class BankGUI:
    def __init__(self, root):
        self.root = root
        self.root.title("Banking System")
        self.root.geometry("400x450")
        self.current_account = None

        self.menu_frame()

    def menu_frame(self):
        self.clear_frame()
        tk.Label(self.root, text="Welcome to the Bank", font=("Arial", 14)).pack(pady=10)
        tk.Button(self.root, text="Login", command=self.login_frame).pack(pady=5)
        tk.Button(self.root, text="Create Account", command=self.create_frame).pack(pady=5)
        tk.Button(self.root, text="Admin Mode", command=self.admin_frame).pack(pady=5)

    def login_frame(self):
        self.clear_frame()
        tk.Label(self.root, text="Login", font=("Arial", 14)).pack()
        self.acc_input = self.create_input("Account Number")
        self.pin_input = self.create_input("PIN", show='*')
        tk.Button(self.root, text="Login", command=self.do_login).pack()
        tk.Button(self.root, text="Back", command=self.menu_frame).pack(pady=5)


    def create_frame(self):
        self.clear_frame()
        tk.Label(self.root, text="Create Account", font=("Arial", 14)).pack()
        self.new_acc = self.create_input("New Account Number")
        self.new_name = self.create_input("Name")
        self.new_amt = self.create_input("Initial Deposit")
        self.new_pin = self.create_input("Set PIN", show='*')
        tk.Button(self.root, text="Create", command=self.do_create).pack()
        tk.Button(self.root, text="Back", command=self.menu_frame).pack(pady=5)


    def admin_frame(self):
        self.clear_frame()
        tk.Label(self.root, text="Admin Access", font=("Arial", 14)).pack()
        self.admin_pwd = self.create_input("Password", show='*')
        tk.Button(self.root, text="Enter", command=self.admin_login).pack()
        tk.Button(self.root, text="Back", command=self.menu_frame).pack(pady=5)


    def transaction_frame(self):
        self.clear_frame()
        tk.Label(self.root, text=f"Welcome {self.current_account.name}", font=("Arial", 14)).pack()
        tk.Button(self.root, text="Send Money", command=self.send_money).pack(pady=5)
        tk.Button(self.root, text="Withdraw", command=self.withdraw_money).pack(pady=5)
        tk.Button(self.root, text="Check Balance", command=self.show_balance).pack(pady=5)
        tk.Button(self.root, text="Back", command=self.menu_frame).pack(pady=5)

        tk.Button(self.root, text="Logout", command=self.menu_frame).pack(pady=10)

    def create_input(self, label_text, show=None):
        tk.Label(self.root, text=label_text).pack()
        entry = tk.Entry(self.root, show=show)
        entry.pack()
        return entry

    def do_login(self):
        acc_num = self.acc_input.get()
        pin = self.pin_input.get()
        account = find_account(acc_num)
        if account and account.pin == pin:
            self.current_account = account
            self.transaction_frame()
        else:
            messagebox.showerror("Error", "Invalid credentials")

    def do_create(self):
        msg = create_account(
            self.new_acc.get(),
            self.new_name.get(),
            float(self.new_amt.get()),
            self.new_pin.get()
        )
        messagebox.showinfo("Info", msg)
        self.menu_frame()

    def admin_login(self):
        if self.admin_pwd.get() == "admin123":
            self.clear_frame()
            tk.Label(self.root, text="All Accounts", font=("Arial", 14)).pack()
            for acc in accounts:
                tk.Label(self.root, text=f"{acc.account_number} - {acc.name} - UGX.{acc.amount:.0f}").pack()
        else:
            messagebox.showerror("Error", "Wrong password")

    def send_money(self):
        self.clear_frame()
        tk.Label(self.root, text="Send Money", font=("Arial", 14)).pack()
        self.receiver_input = self.create_input("Receiver Account")
        self.amount_input = self.create_input("Amount")
        self.otp = generate_otp()
        messagebox.showinfo("OTP", f"Your OTP is {self.otp}")
        self.otp_input = self.create_input("Enter OTP")
        tk.Button(self.root, text="Send", command=self.process_send).pack()

    def process_send(self):
        receiver_acc = self.receiver_input.get()
        amount = float(self.amount_input.get())
        otp_val = self.otp_input.get()
        receiver = find_account(receiver_acc)
        if not receiver:
            messagebox.showerror("Error", "Receiver not found")
            return
        if not verify_otp(otp_val, self.otp):
            messagebox.showerror("Error", "Incorrect OTP")
            return
        total = send_charge(amount)
        if self.current_account.amount < total + MinBal:
            messagebox.showerror("Error", "Insufficient funds")
            return
        self.current_account.amount -= total
        receiver.amount += amount
        logs(self.current_account.account_number, "Sent", amount)
        save_accounts()
        messagebox.showinfo("Success", f"Money sent. New balance UGX.{self.current_account.amount:.0f}")
        self.transaction_frame()

    def withdraw_money(self):
        self.clear_frame()
        tk.Label(self.root, text="Withdraw Money", font=("Arial", 14)).pack()
        self.withdraw_input = self.create_input("Amount")
        self.otp = generate_otp()
        messagebox.showinfo("OTP", f"Your OTP is {self.otp}")
        self.otp_check = self.create_input("Enter OTP")
        tk.Button(self.root, text="Withdraw", command=self.process_withdraw).pack()

    def process_withdraw(self):
        amount = float(self.withdraw_input.get())
        otp_val = self.otp_check.get()
        if not verify_otp(otp_val, self.otp):
            messagebox.showerror("Error", "Incorrect OTP")
            return
        total = withdraw_charge(amount)
        if self.current_account.amount < total + MinBal:
            messagebox.showerror("Error", "Insufficient funds")
            return
        self.current_account.amount -= total
        logs(self.current_account.account_number, "Withdraw", amount)
        save_accounts()
        messagebox.showinfo("Success", f"Withdrawn UGX.{amount:.0f}. New balance: UGX.{self.current_account.amount:.0f}")
        self.transaction_frame()

    def show_balance(self):
        messagebox.showinfo("Balance", f"UGX.{self.current_account.amount:.0f}")

    def clear_frame(self):
        for widget in self.root.winfo_children():
            widget.destroy()

root = tk.Tk()
app = BankGUI(root)
root.mainloop()

