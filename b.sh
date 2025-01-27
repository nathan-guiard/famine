make -C exemple
make fclean prod 
clear

echo
echo "Hello world (not infected)"
/tmp/test/hello_world

echo
echo "Famine"
./famine

echo
echo "Hello world (infected)"
/tmp/test/hello_world

gcc -o /tmp/test/hello exemple/hello_world.c
echo
echo "Hello (not infected)"
/tmp/test/hello

# echo
# echo "infection de hello"
# ./famine

# echo
# echo "hello (infected)"
# /tmp/test/hello
