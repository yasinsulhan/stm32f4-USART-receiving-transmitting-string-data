# readme
USART with register was explained [before](https://github.com/yasinsulhan/stm32f4-usart-with-register). In this project, the string data is received and transmitted. Actually this project is similar with the other USART project. The only difference is receiving the string data. To do this, a character buffer is generated. The data that is as character coming from shift register to DR register is stored in the buffer. The process is repeated untill the buffer is filled with the characters contituting of the string. The codeblock doing this process is given below.

```
void assign_the_data(char *buffer)
{
    char* ptr;
    ptr = &buffer[0];

    for(int i=0; i<20; i++)
    {
    	while (!(USART2->SR & (1<<5)));
        *(ptr+i) = USART2->DR;

        if(*(ptr+i) == '\n')
        	return;
    }
}
```

In a nutshell, the function takes the buffer array parameter which is created as global variable. The pointer created in inside of the function points the starting of buffer. Then it stores the the characters coming from shift register to the DR register. If the last data is ```'\n'``` which proves the pointer is at the end of the string received. Function is ended properly and the buffer is keept the the data permenantly to use in the condition when the data is received. According to condition, the needed led is lighed up by comparing each character of the buffer in the conditions.

![ispat](https://github.com/yasinsulhan/readme/assets/109728194/d328ee29-8eb5-4a69-a9ec-f559f515b72b)
