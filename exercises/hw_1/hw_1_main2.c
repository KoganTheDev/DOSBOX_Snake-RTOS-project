#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define STRSIZE 100

// Standard version
// Counts the amount of comparisons
// Prints each comparison to show that the algorithm isn`t deterministic when the string isn`t a palindrome
int palindrome_test_version_2(char* str, int* wrong_index1, int* wrong_index2, int* total)
{
	size_t len = strlen(str);
	int left_index = 0;
	int right_index = len - 1;
	*total = 0;

	while (left_index < right_index)
	{
		(*total)++;
		if (str[left_index] != str[right_index])
		{
			*wrong_index1 = left_index;
			*wrong_index2 = right_index;
			return 0;
		}
		
		left_index++;
		right_index--;
	}

	return 1; // It is a palindrome
}

// Deterministic version
// Counts the amount of comparisons
// Prints each comparison to show that the algorithm is truly deterministic
int deterministic_palindrome_test_2(char* str, int* wrong_index1, int* wrong_index2, int* total)
{
	size_t len = strlen(str);
	int left_index = 0;
	int right_index = len - 1;
	bool is_palindrome = true;
	bool found_mismatch = false;

	*total = 0;

	while (left_index < right_index)
	{
		(*total)++;
		if (str[left_index] != str[right_index])
		{
			*wrong_index1 = left_index;
			*wrong_index2 = right_index;
			is_palindrome = false;
		}
		
		left_index++;
		right_index--;
	}

	return is_palindrome; // It is a palindrome
}

void result_check(int* flag, char* str, int* wrong_index1, int* wrong_index2, int* total)
{
	if (*flag == 1)
   		printf("%s IS a palindrome\n", str);
  	else
  	{
    	printf("\n%s is NOT a palindrome\n", str);
    	printf("%s[%d] = %c ! = %s[%d] = %c\n\n", 
    		str, wrong_index1, str[*wrong_index1], str, wrong_index2, str[*wrong_index2]);
    } // else
    printf("\nTotal number of comparisons = %d\n\n", *total);

}

void main()
{
	char str[STRSIZE];
	int flag, flag2, wrong_index1, wrong_index2;
    int total1 = 0;
    int total2 = 0;
	printf("Enter a string:\n");
	gets(str);
	flag =  palindrome_test_version_2(str,  &wrong_index1, &wrong_index2, &total1);
	flag2 = deterministic_palindrome_test_2(str, &wrong_index1, &wrong_index2, &total2);
	
	const char* first_message[] = {"Standard Vs. Deterministic\n",
						   		   "---------------------------\n",
								   "Standard version:\n",
								   NULL};

	for (int i = 0; first_message[i] != NULL; i++)
	{
		printf(first_message[i]);
	}
	result_check(&flag, str, &wrong_index1, &wrong_index2, &total1);
	printf("Deterministic version:\n");
	result_check(&flag2, str, &wrong_index1, &wrong_index2, &total2);
}