How to compile and run:
First of all, in order to compile it, you need to have readline GNU library ready to be linked. There are no other libraries other than
standard basic libraries (string.h, stdio.h etc) is used in this project. This library is used for command line history management.

HOW TO INSTALL

The basic installation for the library follows like this
1.Way
	just run batch file called getlibraryandcompile.sh in your linux machine. It will automatically downloads and installs required library and compile
	the code. You will see pelinakpinar.out file in the working directory.
2.Way
	First run command 
	"sudo apt-get -y install libreadline-dev"
	to install libreadline library. This command automatically installs required library. 
	After that you can run "gcc main.c -lreadline -o pelinakpinar.out" to compile c file. You can use other compilers also but gcc 
	is mainly used in linux systems and should be already installed.

HOW TO USE

	Usage is fairly simple. Run compiled file and you will see the program opens and waiting for user input. After that you can write
	most of the unix commands and program will run it as you did in classic shell. In addition to that, program also supports history of 
	commands if you use up and down arrows. Also you can pipe your commands as you wish. Pipe number is not limited, you can pipe commands
	as you wish you want but max arguments and line size is limited. But I do not think it will be a problem, it is fairly huge.

CODE FLOW
	There are comments in the code but for general purpose, here is the code flow with explanation. Code is consisted with six functions in 
	addition to the main function. Also uses struct to define unix command. Main function is basically gets input and make it work with passing
	input using infinite loop. While inside that loop, takeAndAddHistory function is called. This function reads the line and passes to the 
	string and also saves to the command history. Saving for the history can be later used when pressing up and down arrows.

execute(char *commands) function
	When user sends input to the program, it will add to the history and pass it to the this function. This function first fork the program. So lets
	say main function as parent and child. Child then goes parseThenRun method with user input and parent will wait for child to finish. Then returns.
	This function make the program not quit when certain things such as errors happened or commands are finished by forking itself. Whatever child do 
	is done in other side and parent will wait it to finish the job, then returns to the main functions infinite loop.
parsethenRun(char *commands) function
	 First of all, this function allocates memory for parsing. After that creates struct list with commands which will be later used for piping and 
	executing. After that, it will use strtok for the tokenizing string, and splits it for the each pipe. If pipe is not exist, then there is no split
	so all the line will be in one string. When doing that, after each split, it reallocates the memory for unlimited pipe functionality.
	After that, each tokenized string means one commands with one or many command arguments. For the commands list, memory is allocated according to
	number of commands and each of the member is filled with the help of parse(tokenizedString, allCommands[j]) line. Parsing will fill the members
	and after that runAll(i,allCommands) is called in order to run all of the commands. i in here is nmber of commands. Lastly, it frees up the memory.
parse(char *cmdline, struct command *cmd)
	This function gets tokenized (user input that is splitted by | character) and a struct command. Function basically tokenize the line with space 
	character and fills the struct. Struct has two members. One is number of arguments, second one is string array which is now filled with tokenized command
	first and arguments rest. Lastly, as required, last argument is added as null and returns.
runAll(int numberOfCommands, struct command** allcommdans)
	This function pases and pipes all he commands but not latest one. Because all of the commands will use file descriptors of the previous command's 
	output as input and outputs is passed to the next command's input. But last one"s output file descriptor wont be piped because it needs to be showed on
	the screen. So in order to achieve that, one loop is focused on all of the commands executing and leaves last command. Lastcommand called with execvp.
	Inside the loop, First, two file descriptor is piped with the help of standart library unistd.h help. After that Runpiped function is called. And after that
	pipe is closed and previous command input file descriptor which named fd[0] is assigned to the other descriptor which named as in. So as loop continues, first 
	input is not described but all of the other ones should work with previous output.
runPiped(int in, int out, struct command cmd)
	this function gets input, output of piped variable and command. It first forks, and sets input descriptor from in variable. After that sets output to the
	file desriptor. After that, executes the command with execvp function. All of the works is done inside child. So as progress in the loop, every child other than first 
	child can get output from previous child and puts input into piped variables.