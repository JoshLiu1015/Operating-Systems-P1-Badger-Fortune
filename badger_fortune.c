#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>


int main(int argc, char *argv[])
{   
    
    size_t len = 0;
    size_t nread;
    int totalNumFortunes;
    int countNumFortune = 0;
    int maxLength;
    int inputNumber;

    int option;
    int fFlag = 0;
    int bFlag = 0;
    int nFlag = 0;
    int oFlag = 0; 
    
    FILE* fortuneFile;
    FILE* outFile;
    FILE* batchFile;


    //error case - less than 5 arguments
    if(argc < 5){
        printf("USAGE: \n\tbadger-fortune -f <file> -n <number>" 
        " (optionally: -o <output file>) \n\t\t OR"
        " \n\tbadger-fortune -f <file> -b <batch file> (optionally: -o <output file>)\n");
        exit(1);
    }


    while((option = getopt(argc, argv, ":f:n:b:o:")) != -1){
        switch(option){      
            case 'n':
                //the order of -n and -b matter, different orders cause different error cases
                nFlag = 1;
                //error case - using -b then -n
                if(bFlag == 1){
                    printf("ERROR: You can't specify a specific fortune number in conjunction with batch mode\n");
                    exit(1);
                }
                break;
            case 'b':
                //the order of -n and -b matter, different orders cause different error cases
                bFlag = 1;

                //error case - using "-n" then "-b"
                if(nFlag == 1){
                    printf("ERROR: You can't use batch mode when specifying a fortune number using -n\n");
                    exit(1);
                }

                //open the batch file
                batchFile = fopen(optarg, "r");
                //if can't open batch file, error case - can't open batch file
                if(batchFile == NULL){
                    printf("ERROR: Can't open batch file\n");
                    exit(1);
                }
                
                //fseek sets the file position of the stream to the given offset
                //(SEEK_END is the position, 0 is the offset)
                fseek(batchFile, 0, SEEK_END);
                //ftell returns the current file position of the given stream
                int lenB = ftell(batchFile);
                
                //if the batch file is empty, error case - batch file empty
                if(lenB == 0){
                    printf("ERROR: Batch File Empty\n");
                    exit(1);
                }
                //sets the file position back to the first char
                fseek(batchFile, 0, SEEK_SET);
                break;

            case 'o':
                oFlag = 1;

                outFile = fopen(optarg,"w");
                break;
            case 'f':
                fFlag = 1;

                fortuneFile = fopen(optarg, "r");

                //error case - can't open fortune file
                if(fortuneFile == NULL){
                    printf("ERROR: Can't open fortune file\n");
                    exit(1);
                }

                //fseek sets the file position of the stream to the given offset
                //(SEEK_END is the position, 0 is the offset)
                fseek(fortuneFile, 0, SEEK_END);
                //ftell returns the current file position of the given stream
                int lenF = ftell(fortuneFile);

                //error case - when fortune file is empty
                if(lenF == 0){
                    printf("ERROR: Fortune File Empty\n");
                    exit(1);
                }
                //sets the file position back to the first char
                fseek(fortuneFile, 0, SEEK_SET);

                break;
            
            //it returns ':' when there's a missing option argument
            case ':':
                printf("Option needs a value %s\n", optarg);
                exit(1);

            //it returns '?' when there's an invalid option type 
            case '?':
                printf("ERROR: Invalid Flag Types\n");
                exit(1);
        }
    }


    if(fFlag == 0){
        printf("ERROR: No fortune file was provided\n");
        exit(1);
    }


    char *line = (char*) malloc(len);

    //get the first number in the fortune file which is the total number of fortunes 
    if((nread = getline(&line, &len, fortuneFile)) != -1){       
        //convert it to an integer
        sscanf(line, "%d", &totalNumFortunes);
    }

    //the second line is the max length of a fortune
    if((nread = getline(&line, &len, fortuneFile)) != -1){
        sscanf(line, "%d", &maxLength);
    }


    //reset getopt
    optind = 1;

    

    while((option = getopt(argc, argv, ":f:n:b:o:")) != -1){
        switch(option){
            case 'b':
                //read the batch file
                while ((nread = getline(&line, &len, batchFile)) != -1) {
                    //sets the fortuneFile position of the stream to the beginning after running a batch number
                    fseek(fortuneFile, 0, SEEK_SET);

                    //convert the number from the batch file to an integer
                    sscanf(line, "%d", &inputNumber);

                    //error case - if a batch number is <= 0
                    if(inputNumber <= 0){
                        printf("ERROR: Invalid Fortune Number\n\n");
                    }

                    //error case - if a batch number is > total number of fortunes
                    if(inputNumber > totalNumFortunes){
                        printf("ERROR: Invalid Fortune Number\n\n");
                    }

                    //read the fortune file line by line
                    while ((nread = getline(&line, &len, fortuneFile)) != -1) {
                        //if the line is "%"
                        if (*line == '%'){
                            //+1 to the integar countNumFortune
                            countNumFortune++;
                        }
                        //if the line is not "%" and the number of "%" added is the same as the batch number
                        else if (countNumFortune == inputNumber){       
                            if(oFlag == 1){
                                //output these lines until encountering another "%"
                                fprintf(outFile, "%s", line);
                            }
                            else{
                                //output the lines to the terminal
                                printf("%s", line);   
                            }              
                                                                            
                        }
                        //if the number of "%" counted is more than a batch number, break the while loop
                        else if(countNumFortune > inputNumber){
                            break;
                        }            
                    }

                    //if the error message is printed, no need to add newline
                    if(inputNumber > 0 && inputNumber <= totalNumFortunes){
                        if(oFlag == 1){
                            fprintf(outFile, "\n\n");
                        }
                        else{
                            printf("\n\n");
                        }
                        
                    }
                    //reset the number of fortunes to 0
                    countNumFortune = 0;
                }
                
                break;
            
            case 'n':
                //convert the number to an integer
                sscanf(optarg, "%d", &inputNumber);

                //if the number is <= 0, error case - invalid fortune number
                if(inputNumber <= 0){
                    printf("ERROR: Invalid Fortune Number\n");
                    exit(1);
                }
                
                //if the number is > the total number of fortunes, error case - invalid fortune number 
                if(inputNumber > totalNumFortunes){
                    printf("ERROR: Invalid Fortune Number\n");
                    exit(1);
                }


                //read the fortune file
                while ((nread = getline(&line, &len, fortuneFile)) != -1) {
                    //if the line is "%"
                    if (*line == '%'){
                        //+1 to the integar countNumFortune
                        countNumFortune++;
                    }
                    //if the line is not "%" and the number of "%" added is the same as the input number
                    else if (countNumFortune == inputNumber){
                        if(oFlag == 1){
                            //output these lines until encountering another "%"
                            //output the lines to the output file
                            fprintf(outFile, "%s", line);
                        }
                        else{
                            
                            //output the lines to the terminal
                            printf("%s", line);
                        }
                        
                    }
                    //if the number of "%" counted is more than the input number, break the while loop
                    else if(countNumFortune > inputNumber){
                        break;
                    }  
                }
                
                break;
        }
    }
    free(line);

    if(oFlag == 1){
        fclose(outFile);
    }
    if(bFlag == 1){
        fclose(batchFile);
    }
    fclose(fortuneFile);    
    
    return 0;
}
