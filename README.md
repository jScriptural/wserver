# wserver
A simple http  GET server.

## prerequisites
To run this server, you must have the following installed on the system:
* nodejs
* bash shell
* make
* C compiler

## Setting up the server
### step 1:
Clone this repo by running
```
git clone https://github.com/jscriptural/wserver
```
### step 2:
After step 1, a directory name _wserver_ will be created in your current directory.
Change your current directory to _wserver_ by
running:
```
cd wserver
```
### step 3:
Install server software by  running
```
./wserver-install
```
### step 4:
Remove the demo files in the _src_ directory:
```
rm src/*
```
Move  your files into the _src_directory.
The index.html file is necessary and serve as the entry point of your app. 
And it must reside directly in the _src_ directory.

### step 5:
Start the server on a choosen port e.g port 3000
```
wserver 3000
```
### finally:
Go to http://localhost:3000 on your browser to see magic.
