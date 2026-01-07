# 🏦 Mini Bank System (C++ Console Application)

A complete **console-based banking system** implemented in **C++**,  
designed to simulate real-world banking operations with **authentication, authorization,
role-based permissions**, and **persistent data storage** using files.

This project demonstrates solid understanding of **C++ fundamentals**,  
**file handling**, **data structures**, and **system design concepts**.

---

## 🚀 Features

### 🔐 Authentication & Authorization
- Secure login system
- Role-based access control using **bitmask permissions**
- Admin and normal users support
- Permission validation before accessing system features

### 👥 Client Management
- Add new clients
- Update client information
- Delete clients
- Find clients by account number
- Display all clients in a formatted table

### 💰 Transactions
- Deposit money
- Withdraw money with balance validation
- Display total balance of all clients

### 👤 User Management (Admin Only)
- Add new users
- Update user permissions
- Delete users
- Find users
- List all users

---

## 🧠 Permissions System

Each user has a set of permissions that controls what actions they can perform.

Permissions include:
- Show Clients List
- Add Client
- Update Client
- Delete Client
- Find Client
- Transactions
- Manage Users
- Full Access (Admin)

Permissions are implemented using **bitmasking**, similar to real banking and enterprise systems.

---

## 🗂 Data Storage

- Clients data stored in: `Clients Data.txt`
- Users data stored in: `Users.txt`
- Data is persistent using **file I/O** (no database required)

---

## 🛠 Technologies Used

- C++
- STL (vector, string, iostream, fstream, iomanip)
- File Handling
- Console-based UI
- Bitmask Permissions System

---

## ▶️ How to Compile & Run

Using **g++**:

```bash
g++ main.cpp -o MiniBankSystem
./MiniBankSystem


Default Login Credentials

Username: Admin
Password: 1234





👨‍💻 Author

Mohamed
C++ Junior Developer / Computer Teacher

