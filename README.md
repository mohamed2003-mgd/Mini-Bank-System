# 🏦 Mini Banking System (C++ Console Application)

A fully functional **Console-based Banking System** built using **C++**,  
demonstrating core programming concepts such as file handling, data structures,
authentication, authorization, and role-based access control.

---

## 🚀 Features

### 🔐 Authentication & Authorization
- Secure login system
- Role-based permissions using bitmasking
- Admin and normal users support

### 👥 Client Management
- Add new clients
- Update client information
- Delete clients
- Find clients
- Display clients list

### 💰 Transactions
- Deposit money
- Withdraw money (with balance validation)
- Display total balance of all clients

### 👤 User Management (Admin Only)
- Add new users
- Update user permissions
- Delete users
- Find users
- List all users

---

## 🧠 Permissions System

Each user has specific permissions that control access to system features:

- Show Clients
- Add Client
- Update Client
- Delete Client
- Find Client
- Transactions
- Manage Users
- Full Access (Admin)

Permissions are implemented using **bitmasking**, similar to real-world systems.

---

## 🗂 Data Storage

- Clients data stored in: `Clients Data.txt`
- Users data stored in: `Users.txt`
- Data is persistent using file I/O (no database)

---

## 🛠 Technologies Used

- C++
- STL (vector, string, iostream, fstream)
- File Handling
- Console UI

---

## ▶️ How to Run

1. Clone the repository: git clone https://github.com/mohamed2003-mgd/Mini-Bank-System.git


Username: Admin
Password: 1234
