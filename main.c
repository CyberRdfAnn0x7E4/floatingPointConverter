#include <stdio.h>
#define MANTISSE_LENGTH_64 52
#define EXPONENT_LENGTH_64 11
#define BIAS_64 1023
#define MANTISSE_LENGTH_32 23
#define EXPONENT_LENGTH_32 8
#define BIAS_32 127
/**
 * @brief getNumberOfDigits - count the number of digits in a number, '123' -> return 3
 * @param number
 * @param factor
 * @return
 */
int unsigned getNumberOfDigits(int unsigned *number, int unsigned *factor){
    int unsigned numberOfDigits = 1;
    *factor = 1;
    while(*number >= 10 * *factor){
        numberOfDigits++;
        *factor *= 10;
    }

    return numberOfDigits;
}

/**
 * @brief decimalToBinaryString - convert a number like 121 to binary
 * @param string
 * @param decimal
 */
void decimalToBinaryString(char *string, int unsigned decimal){
    int i = -1;
    while(string[++i] != '\0');
    i -= 1; // i is max index of string
    while(i >= 0 && decimal != 0){
        string[i--] = (decimal % 2 == 0) ? '0' : '1';
        decimal = decimal/ 2;
    }
}

/**
 * @brief floatToBinaryString - convert a number like 0.242 to binary
 * @param numberStr
 * @param number
 */
void floatToBinaryString(char *numberStr, unsigned int number, unsigned int mantisseLength){

    int unsigned factor, number2Length, i = 0, number2LengthOriginal = getNumberOfDigits(&number, &factor);

    while(i < mantisseLength * 2 && number != 0){
        number = number * 2;
        number2Length = getNumberOfDigits(&number, &factor);
        if(number2LengthOriginal < number2Length){
            numberStr[i++] = '1';
            number -= factor;
        }
        else{
            numberStr[i++] = '0';
        }
    }
}

void fillStringWithZerosAndNull(char *string, unsigned int strLength){
    unsigned int i;
    for(i = 0; i < strLength; i++){
        string[i] = '0';
    }
    string[strLength] = '\0';
}


int main()
{
    int unsigned number1, number2, k, i, MANTISSE_LENGTH, EXPONENT_LENGTH;
    int  number1Original, exponent = 0, BIAS;
    char bit, input, buffer, signedBit, number1Str[MANTISSE_LENGTH_64 + 1], number2Str[MANTISSE_LENGTH_64*2 + 1],
            mantisse[MANTISSE_LENGTH_64 + 1], exponentStr[EXPONENT_LENGTH_64 + 1]; // length plus 1 for '\0'

    printf("\t+-----------------------------------+\n"
           "\t| IEEE-754 Floating Point Converter |\n"
           "\t|           created in C            |\n"
           "\t|            by Dustin              |\n"
           "\t+-----------------------------------+\n\n\n");

    do{
        printf("'1' fuer 32 Bit (float) und '2' fuer 64 Bit (double).\n"
               "Geben Sie Ihre Wahl ein: ");
        scanf("%c", &bit);
        if(bit == '1'){
            MANTISSE_LENGTH = MANTISSE_LENGTH_32;
            EXPONENT_LENGTH = EXPONENT_LENGTH_32;
            BIAS = BIAS_32;
        }
        else{
            MANTISSE_LENGTH = MANTISSE_LENGTH_64;
            EXPONENT_LENGTH = EXPONENT_LENGTH_64;
            BIAS = BIAS_64;
        }

        fillStringWithZerosAndNull(number1Str, MANTISSE_LENGTH);
        fillStringWithZerosAndNull(number2Str, MANTISSE_LENGTH*2);
        fillStringWithZerosAndNull(mantisse, MANTISSE_LENGTH);
        fillStringWithZerosAndNull(exponentStr, EXPONENT_LENGTH);

        printf("Geben sie die zahl vor dem Komma ein: ");
        scanf("%d", &number1Original);
        number1 = (unsigned)(number1Original < 0 ? number1Original * -1 : number1Original);

        printf("\nGeben sie die zahl nach dem Komma ein: ");
        scanf("%d", &number2);

        printf("\n\nSie haben eingeben: %d.%u", number1Original, number2);

        // set signed bit:
        signedBit = number1Original < 0 ? '1' : '0';

        // render number1 to 'binary' string
        decimalToBinaryString(number1Str, number1);

        // render number2 to 'binary' string
        floatToBinaryString(number2Str, number2, MANTISSE_LENGTH);

        printf("\n\nVor den Komma:\t%s", number1Str);
        printf("\nNach den Komma:\t%s", number2Str);

        k = 0;
        if(number1Original == 0){
            // input is 0
            for(i = 0; i < MANTISSE_LENGTH; i++){
                mantisse[i] = '0';
            }
            exponent = -BIAS;

        }else if(number1Original != 0){
            // input is over 0
            while(number1Str[k++] == '0');
            exponent = (signed)(MANTISSE_LENGTH - k);

            for(i = 0; k < MANTISSE_LENGTH; k++, i++){
                mantisse[i] = number1Str[k];
            }
            for(k = 0; i < MANTISSE_LENGTH; i++, k++){
                mantisse[i] = number2Str[k];
            }

        }
        else{
            // input is under 0
            while(number2Str[k++] == '0');
            exponent = (signed)-k;

            for(i = 0; i < MANTISSE_LENGTH; k++, i++){
                mantisse[i] = number2Str[k];
            }
        }

        printf("\nMantisse:\t%s", mantisse);

        exponent += BIAS; //add bias
        decimalToBinaryString(exponentStr, (unsigned)exponent);
        printf("\nExponent:\t%s", exponentStr);

        printf("\nVorzeichen Bit:\t%c", signedBit);
        printf("\nErgebniss:\t%c%s%s", signedBit, exponentStr, mantisse);

        printf("\n\nProgram beenden (y/N)?");
        while ((getchar()) != '\n');
        scanf("%c%c", &input, &buffer);

        printf("\n\n\n\n\n-------------------------------------------------------------------------\n\n\n\n\n");

    }while(input != 'y');

    return 0;
}
