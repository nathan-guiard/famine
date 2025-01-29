make -C exemple
make re 
clear

echo
strings /tmp/test/hello_world | grep "nguiard"
echo "Hello world (not infected)"
/tmp/test/hello_world

echo
echo "Famine"
./famine

echo
strings /tmp/test/hello_world | grep "nguiard"
echo "Hello world (infected)"
/tmp/test/hello_world

gcc -o /tmp/test/hello exemple/hello_world.c
echo
strings /tmp/test/hello | grep "nguiard"
echo "Hello (not infected)"
/tmp/test/hello

echo
echo "infection de hello"
/tmp/test/hello_world

echo
strings /tmp/test/hello | grep "nguiard"
echo "hello (infected)"
/tmp/test/hello