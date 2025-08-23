#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define STRSIZE 100

// Standard version
int palindrome_test(char* str, int* wrong_index1, int* wrong_index2)
{
	size_t len = strlen(str);
	int left_index = 0;
	int right_index = len - 1;

	while (left_index <= right_index)
	{
		if (str[left_index] != str[right_index])
		{
			*wrong_index1 = left_index;
			*wrong_index2 = right_index;
			return 0; // Not palindrome
		}
		
		left_index++;
		right_index--;
	}

	return 1; // It is a palindrome
}

// Deterministic version
// returns the first case of inequality if there are any
int deterministic_palindrome_test(char* str, int* wrong_index1, int* wrong_index2)
{
	size_t len = strlen(str);
	int left_index = 0;
	int right_index = len - 1;
	bool found_mismatch = false;
	bool is_palindrome = true;
	
	while (left_index <= right_index)
	{
		if (str[left_index] != str[right_index] && !found_mismatch)
		{
			*wrong_index1 = left_index;
			*wrong_index2 = right_index;
			is_palindrome = false;
			found_mismatch = true;
		}
		
		left_index++;
		right_index--;
	}

	return is_palindrome; // It is a palindrome
}

void result_check(int* flag, char* str, int* wrong_index1, int* wrong_index2)
{
	if (*flag == 1)
   		printf("%s IS a palindrome\n\n", str);
  	else
  	{
    	printf("\n%s is NOT a palindrome\n", str);
    	printf("%s[%d] = %c ! = %s[%d] = %c\n\n", 
    		str, wrong_index1, str[*wrong_index1], str, wrong_index2, str[*wrong_index2]);
    } // else
}

void main()
{
	char str[STRSIZE];
	int flag, flag2, wrong_index1, wrong_index2;

	printf("Enter a string:\n");
	gets(str);
	flag =  palindrome_test(str,  &wrong_index1, &wrong_index2);
	flag2 = deterministic_palindrome_test(str, &wrong_index1, &wrong_index2);
	
	const char* first_message[] = {"Standard Vs. Deterministic\n",
						   		   "---------------------------\n",
								   "Standard version:\n",
								   NULL};

	for (int i = 0; first_message[i] != NULL; i++)
	{
		printf(first_message[i]);
	}
	result_check(&flag, str, &wrong_index1, &wrong_index2);
	printf("Deterministic version:\n");
	result_check(&flag2, str, &wrong_index1, &wrong_index2);

} /* main */
