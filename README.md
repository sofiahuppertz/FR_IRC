# ft_irc

## Project Overview

**ft_irc** is a project aimed at creating an **IRC** (**Internet Relay Chat**) server using **C++ 98**. This server allows multiple clients to connect, communicate in real-time via private or public messages, and join group channels. The server adheres to solid internet protocols ensuring robust and reliable communication.

## Table of Contents

- [**Project Overview**](#project-overview)
- [**Table of Contents**](#table-of-contents)
- [**Features**](#features)
- [**Requirements**](#requirements)
- [**Installation**](#installation)
- [**Usage**](#usage)
- [**Commands**](#commands)
- [**Testing**](#testing)
- [**Bonus Features**](#bonus-features)
- [**License**](#license)

## Features

- Handle multiple clients simultaneously without hanging.
- Non-blocking I/O operations.
- Authenticate users, set nicknames, join channels, and send/receive private messages.
- Operators and regular users with specific commands for channel operators.

## Requirements

- **C++ 98** compliant compiler
- Non-blocking file descriptors
- Socket programming knowledge

## Installation

Clone the repository and navigate to the project directory:

```bash
git clone <repository-url>
cd ft_irc
make
```

## Usage

Run the server with the following command:
`./ircserv <port> <password>`

- `port`: The port number the IRC server listens to.
- `password`: The connection password required by clients.

## Commands

### User Commands

- **NICK**: Set or change the nickname.
- **USER**: Set the username.
- **JOIN**: Join a channel.
- **PRIVMSG**: Send a private message.

### Operator Commands

- **KICK**: Eject a client from the channel.
- **INVITE**: Invite a client to a channel.
- **TOPIC**: Change or view the channel topic.
- **MODE**: Change the channel’s mode.

For more detailed information on the commands, refer to the [user-commands.md](user-commands.md) file.

## Testing

To test the server, you can use any IRC client or `nc` (Netcat):
`nc 127.0.0.1 <port>`

## Bonus Features

- File transfer capability.
- IRC bot implementation.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for more details.
