# yaPs

yaPs is a peer-to-peer (and hopefully some day peer-to-server) chat client based in a linux terminal.

In the final product, a user will be able to host or join chatrooms, as well as privately message users. It will have a pleasant ui built with the ncurses library.

## Usage

Before you do anything, make sure you compile the source code with `make`.

For a basic demo of the client-server interactions, run `./yaPs`. If this command doesn't work, make sure the script has execute permissions with `chmod +x yaPs`. For a more realistic use case, running the client and servers in separate terminals via `./bin/server` and `./bin/client` also works.

## Installation

1. `git clone git@github.com:ruuffian/yaPs.git` will clone the source code.

2. `make` will build the project binaries.

3. `./bin/server` to run the server, `./bin/client` to run the client. Atm, very simple.


## License

It's an MIT License, go crazy. Open source is good.
