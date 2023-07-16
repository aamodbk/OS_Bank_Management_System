# OS MINIPROJECT
## Bank Management System

This project implements a simple banking system using C language. It consists of a server program, a client program, and a database code.

## Server

The server program (`server.c`) is responsible for handling client requests and performing banking operations. It uses sockets for communication and supports multiple client connections simultaneously. The server maintains a user database and allows clients to perform operations such as account creation, balance inquiry, deposit, withdrawal, and transfer.

To compile and run the server program, use the following commands:

```bash
gcc server.c -o server
./server
```

## Client

The client program (`client.c`) allows users to interact with the banking system. It communicates with the server to perform operations on user accounts. Users can create new accounts, log in, and perform various banking operations.

To compile and run the client program, use the following commands:

```bash
gcc client.c -o client
./client
```

## Database Code

The database code (`database.c`) provides functions to manage the user database. It includes functions to add new users, retrieve the next available user ID, and store user information in separate files for normal users, joint users, and administrators.

To compile and run the database code, use the following commands:

```bash
gcc database.c -o database
./database
```

## Usage

1. Start the server by running the `server` executable.
2. Run the `client` executable to connect to the server.
3. Follow the prompts on the client program to create a new account or perform banking operations.

## Dependencies

This project requires the following libraries:

- `stdio.h`
- `unistd.h`
- `fcntl.h`
- `string.h`

Make sure these libraries are available in your C development environment.
