# csgo-webradar-client
A shitty web-based radar client for Counter-Strike: Global Offensive

## Working-in-progress
Writing JSON via jsoncpp is not working currently. If somebody knows what's wrong please make a pull request.

## Compiling

### Build requirements

* libcurl3
* libcurl-dev

### Clone the repository
```bash
git clone https://github.com/w3bn00b/csgo-webradar-client.git
```

### Build the project
```bash
cd csgo-webradar-client
./build.sh
```

## Running
In order to run this, you MUST use sudo/su due to the memory functions.  Once you've compiled the project, you can run it from the build directory:
```bash
./run.sh
```

### Update the project
```bash
./update.sh
```

## Credit
@Teklad for his [csgo-horker](https://github.com/Teklad/csgo-horker) project
