#include <stdio.h>
#include <string.h>

char brainfuckCode[20000];
int memory[30000];
int memoryPointer = 0;
int instructionPointer = 0;
int loopStart;
int mustExitLoop;

int bracketCheck() {
    int leftBrace = 0;
    int rightBrace = 0;

    //For each character of the code, check if it is a left or right brace for the purpose of syntax.
    for (int i = 0; brainfuckCode[i] != '\0'; i++) {

        //store the current character in a placeholder variable
        char currentInstruction;
        currentInstruction = brainfuckCode[i];

        //if "[", increment leftBrace
        if (currentInstruction == '[') {
            leftBrace++;
        } 
        // if "]", increment rightBrace
        else if (currentInstruction == ']') {
            rightBrace++;
        }
    }

    //if the user fucked up, tell them how. (dont tell them where, that would be stupid.)
    if (leftBrace != rightBrace) {
        printf("\nSyntax error: brackets mismatch. %d of left braces compared to %d of right braces.", leftBrace, rightBrace);
        return 1;
    }
}

void decoderInterpreted(char currentInstruction) {
    if (mustExitLoop == 1 && currentInstruction != ']') {
        return;
    }

    switch(currentInstruction) {
        case '>': //Increment the memory pointer by 1 - if it overflows, set it to 0
            memoryPointer++;

            if (memoryPointer >= 30000) {
                memoryPointer = 0;
            }

            break;
        
        case '<': //Decrement memory pointer - if it underflows, set to 29999
            memoryPointer--;

            if (memoryPointer < 0) {
                memoryPointer = 29999;
            }

            break;
        
        case '+': //Increment the value stored at the address by 1
            memory[memoryPointer]++;
            break;
        
        case '-': //Decrement the stored value by 1
            memory[memoryPointer]--;
            break;
        
        case '[': //Start / check loops: loop condition is if the value at the pointers current address is NOT 0.
            if (memory[memoryPointer] == 0) {
                mustExitLoop = 1;
                break; //set this to exit the loop. all instructions will be skipped until the ] is reached.
            }

            loopStart = instructionPointer; //remember this position.
            
            break;
        
        case ']':
            //if we need to exit the loop:
            if (mustExitLoop == 1) {
                mustExitLoop = 0;
                break;
            }

            //if we dont want to exit:
            instructionPointer = loopStart - 1; //go back to the start of the loop.
            break;
        
        case ',': //input a char.
            char userInput = getchar();
            memory[memoryPointer] = userInput;
            break;
        
        case '.': //output a char.
            printf("%c", (char)(memory[memoryPointer]));
            break;
    }
}

void compileToC(char currentInstruction) {
    if (mustExitLoop == 1 && currentInstruction != ']') {
        return;
    }

    switch(currentInstruction) {
        case '>': //Increment the memory pointer by 1 - if it overflows, set it to 0
            memoryPointer++;

            if (memoryPointer >= 30000) {
                memoryPointer = 0;
            }

            break;
        
        case '<': //Decrement memory pointer - if it underflows, set to 29999
            memoryPointer--;

            if (memoryPointer < 0) {
                memoryPointer = 29999;
            }

            break;
        
        case '+': //Increment the value stored at the address by 1
            memory[memoryPointer]++;
            break;
        
        case '-': //Decrement the stored value by 1
            memory[memoryPointer]--;
            break;
        
        case '[': //Start / check loops: loop condition is if the value at the pointers current address is NOT 0.
            if (memory[memoryPointer] == 0) {
                mustExitLoop = 1;
                break; //set this to exit the loop. all instructions will be skipped until the ] is reached.
            }

            loopStart = instructionPointer; //remember this position.
            
            break;
        
        case ']':
            //if we need to exit the loop:
            if (mustExitLoop == 1) {
                mustExitLoop = 0;
                break;
            }

            //if we dont want to exit:
            instructionPointer = loopStart - 1; //go back to the start of the loop.
            break;
        
        case ',': //input a char.
            char userInput = getchar();
            memory[memoryPointer] = userInput;
            break;
        
        case '.': //output a char.
            printf("%c", (char)(memory[memoryPointer]));
            break;
    }
}

int main(int argc, char * argv[]) {
    //open the file specified in the cmd line.
    FILE *fptr;
    fptr = fopen(argv[2], "r");

    //Check if file exists
    if (fptr == NULL) {
        printf("Brainfuck file not found.");
        return 1;
    }

    //read the first 20000 chars.
    fgets(brainfuckCode, 20000, fptr);
    bracketCheck();

    //choose compile or interpret
    if (strcmp(argv[1], "compile") == 0) { //if its compile, we compile
        //do compiling here


        //create variables to store the name of the c output file and the bin file
        int lengthOfSrcFileName = strlen(argv[2]);

        char cFileName[lengthOfSrcFileName];
        char binaryFileName[lengthOfSrcFileName + 8];

        strcpy(cFileName, argv[2]);
        strcpy(binaryFileName, argv[2]);

        //turn .bf to .c
        cFileName[strlen(cFileName) - 2] = 'c';
        cFileName[strlen(cFileName) - 1] = '\0';
        
        //turn .bf to .exe CURRENTLY BROKEN
        binaryFileName[strlen(binaryFileName) - 4] = '.'; 
        binaryFileName[strlen(binaryFileName) - 3] = 'e';
        binaryFileName[strlen(binaryFileName) - 2] = 'x';
        binaryFileName[strlen(binaryFileName) - 1] = 'e'; 
        binaryFileName[strlen(binaryFileName) - 0] = '\0'; //redundant -0 for formatting

        printf("%s, %s", cFileName, binaryFileName);
        return 0;

        //i dont know a cleaner way to do this in C.
        //add ".c" to the cfilename:


        while (brainfuckCode[instructionPointer] != '\0') {
            compileToC(brainfuckCode[instructionPointer]);
            instructionPointer++;
        }

        return 0;
    } else if (strcmp(argv[1], "interpret") != 0) { //if it isnt compile and isnt interpret, then its wrong
        printf("Argument 1 must be \"compile\" or \"interpret\"");
        return 1;
    } //it must be interpret, so we ball


    //run the code.
    while (brainfuckCode[instructionPointer] != '\0') {
        decoderInterpreted(brainfuckCode[instructionPointer]);
        instructionPointer++;
    }

    return 0;
}