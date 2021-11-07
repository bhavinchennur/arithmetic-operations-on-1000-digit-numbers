


#include <stdlib.h>
#include <stdio.h>
#include <string.h>


static int* diff_calculator(char *intal1, char *intal2, int *difference, int count, int duplicate_len1, int duplicate_len2, int len1, int len2);

static void mergeSort(char **arr, int l, int r);

static void merge(char **arr, int l, int m, int r); 
 
static int min(int a, int b); 

static char* recursive_pow(char *intal, unsigned int y);

static char* gcd_recursive(char *intal1, char* intal2, char* zero);


// Returns the sum of two intals.
char* intal_add(const char* intal1, const char* intal2)
{


int len1 = 0, len2 = 0, i = 0;

while(intal1[i] != '\0')	//finding the lengths of the 2 strings
{
len1 = len1 + 1;
i = i + 1;
}

i = 0;

while(intal2[i] != '\0')
{
len2 = len2 + 1;
i = i + 1;
}
i = 0;	// just a precaution for future use.

// Beware the array size is one less than the length, for example, if the length is 4 the array would span from 0 to 3.

int max_len;

if(len1 > len2)	// finding the maximum length of the two.
max_len = len1;
else
max_len = len2;	// takes care of the equality condition.



int *sum = (int *)calloc((max_len+10), sizeof(int));//I'm taking an extra bit to accomodate the carry generated by the two MSB's.

int min_len;

if(len1 < len2)	// finding the minimum length of the two.
min_len = len1;
else
min_len = len2;

int duplicate_len1 = (len1 - 1);
int duplicate_len2 = (len2 - 1);
int count = max_len; // sum array size is max-len + 1
int carry = 0;

for(i=(min_len - 1); i>=0 ;i--) // because big endian format is used: MSB at the lowest address
{

sum[count] = (intal1[duplicate_len1--] - '0') + (intal2[duplicate_len2--] - '0') + carry;

carry = sum[count] / 10;

sum[count] = sum[count] % 10;

count = count - 1;
}


while(duplicate_len1 != -1)
{
sum[count] = (intal1[duplicate_len1--] - '0') + carry;

carry = sum[count] / 10;

sum[count] = sum[count] % 10;

count = count - 1;
}

while(duplicate_len2 != -1) // ONLY ONE OF THESE 2 BLOCKS WILL BE EXECUTED
{
sum[count] = (intal2[duplicate_len2--] - '0') + carry;

carry = sum[count] / 10;

sum[count] = sum[count] % 10;

count = count - 1;
}


sum[count] = carry; //count should always be 0 here

char *result = (char *)malloc((max_len + 10) * sizeof(char)); // +2 to accomodate carry bit and '\0'

for(i=0; i<=max_len; i++)
result[i] = sum[i] + '0';


result[max_len + 1] = '\0';

//Adjustment for leading 0s and 0 result
if(result[0] == '0' && result[1] == '0' && result[2] == '\0')
{
result[1] = '\0';
}
else
{
while(result[0] == '0') // only if the last carry is 0, when this function is called successively there's possibility of consecutive 0s before the 1st non-zero no., therefore while is used.
{
for(i=1; i<=(max_len + 1); i++) 
result[i-1] = result[i]; // by running the for loop only for the bits that will stay in the end we are avoiding the involvement of garbage values and we are avoiding accessing out of bounds values.
}
}

free(sum);

return result;
}










// Returns the comparison value of two intals.
// Returns 0 when both are equal.
// Returns +1 when intal1 is greater, and -1 when intal2 is greater.
int intal_compare(const char* intal1, const char* intal2)
{


int len1 = 0, len2 = 0, i = 0;

while(intal1[i] != '\0')	//finding the lengths of the 2 strings
{
len1 = len1 + 1;
i = i + 1;
}

i = 0;

while(intal2[i] != '\0')
{
len2 = len2 + 1;
i = i + 1;
}
i = 0;	// just a precaution for future use.


if(len1 > len2)
return 1;

if(len1 < len2)
return -1;

if(len1 == len2)
{

int flag = 0;
i = 0;

while((intal1[i] - '0') == (intal2[i] - '0'))
{

if(intal1[i] == '\0') // NOTE: '0' != '\0', here 0 = '\0' will not happen
{
flag = 1;
break;
}

i = i + 1;
}

if(flag == 1)
return 0; // the 2 intals are equal

if((intal1[i] - '0') > (intal2[i] - '0'))
return 1;

if((intal1[i] - '0') < (intal2[i] - '0'))
return -1;

}

return -9999; // should never get executed
}











// Returns the difference (obviously, nonnegative) of two intals.
char* intal_diff(const char* intal1, const char* intal2)
{

int len1 = 0, len2 = 0, i = 0;

while(intal1[i] != '\0')	//finding the lengths of the 2 strings
{
len1 = len1 + 1;
i = i + 1;
}

i = 0;

while(intal2[i] != '\0')
{
len2 = len2 + 1;
i = i + 1;
}
i = 0;	// just a precaution for future use.

//----- end of calculation of lengths of the 2 i/p's

int max_len;
if(len1 > len2)
max_len = len1;
else
max_len = len2;


int *difference = (int *)calloc(max_len, sizeof(int)); // difference has the same no. of digits as the greatest number.

int count = max_len - 1; // array's last bit is numbered 1 less than it's size.


int duplicate_len1 = (len1 - 1), duplicate_len2 = (len2 - 1);

char *duplicate_intal1 = (char *)calloc((len1+1), sizeof(char));
char *duplicate_intal2 = (char *)calloc((len2+1), sizeof(char));

strcpy(duplicate_intal1, intal1);

strcpy(duplicate_intal2, intal2);

if(len1 > len2)// intal1 will always be greater than intal2
{

difference = diff_calculator(duplicate_intal1, duplicate_intal2, difference, count, duplicate_len1, duplicate_len2, len1, len2);

} // end of if




if(len2 > len1)
{

difference = diff_calculator(duplicate_intal2, duplicate_intal1, difference, count, duplicate_len2, duplicate_len1, len2, len1);


} // end of if



if(len1 == len2)
{
int i = 0;

while((intal1[i] - '0') == (intal2[i] - '0')) 
{

if(intal1[i] == '\0') // both the numbers are the same
{
difference = diff_calculator(duplicate_intal1, duplicate_intal2, difference, count, duplicate_len1, duplicate_len2, len1, len2);
break;
} // end of if

i++;
} // end of while


if((intal1[i] - '0') > (intal2[i] - '0'))
difference = diff_calculator(duplicate_intal1, duplicate_intal2, difference, count, duplicate_len1, duplicate_len2, len1, len2);


if((intal1[i] - '0') < (intal2[i] - '0'))
difference = diff_calculator(duplicate_intal2, duplicate_intal1, difference, count, duplicate_len2, duplicate_len1, len2, len1);

}

char *result = (char *)malloc((max_len + 1) * sizeof(char));


for(i=0; i<max_len; i++)
result[i] = difference[i] + '0';

result[max_len] = '\0';

free(difference); // the integer array that holds the difference is not needed anymore

i = 0;

int j, counter_for_zeros = 0;

while(result[i] == '0' || result[i] == '\0')
{

if(result[i] == '\0')
{
result[0] = '0';
result[1] = '\0';

return result;

} // end of if

counter_for_zeros++;
i++;
} // end of while


for(j=counter_for_zeros; j<=max_len; j++)
{
result[j - counter_for_zeros] = result[j]; // avoiding out of bounds access.
} // end of for


free(duplicate_intal1);
free(duplicate_intal2);

return result;

}


static int* diff_calculator(char *intal1, char *intal2, int *difference, int count, int duplicate_len1, int duplicate_len2, int len1, int len2)
{

int carry_finder = 0; // copies the value of the current position to find the carry bit.

int i = 0;



for(i=(len2-1); i>=0; i--)
{

if((intal1[duplicate_len1] - '0') >= (intal2[duplicate_len2] - '0'))
{

difference[count--] = (intal1[duplicate_len1] - '0') - (intal2[duplicate_len2] - '0'); // the difference bit is always 1 bit, worst case, 9 - 0 = 9.

} // end of if


if((intal1[duplicate_len1] - '0') < (intal2[duplicate_len2] - '0'))
{

carry_finder = duplicate_len1 - 1;

while(intal1[carry_finder] == '0') // IT SHOULD FIND A NON-ZERO BIT
{
carry_finder-- ;
}

intal1[carry_finder] = intal1[carry_finder] - 1; // subtracts one ascii value to get the character of the preceding number.

carry_finder = carry_finder + 1;

while(carry_finder != duplicate_len1)
{
intal1[carry_finder] = '9'; // because this string would have been a '0'
carry_finder++ ; 
}


difference[count--] = (intal1[duplicate_len1] - '0') + 10 - (intal2[duplicate_len2] - '0');

} // end of if

duplicate_len1 = duplicate_len1 - 1;
duplicate_len2 = duplicate_len2 - 1;
} // end of for


// Now we have to deal with the extra bits in intal1, there's nothing carried to these extra bits.

while(duplicate_len1 != -1)
{
difference[count--] = (intal1[duplicate_len1] - '0');
duplicate_len1-- ;
}


return difference;

}















// Returns the product of two intals.
char* intal_multiply(const char* intal1, const char* intal2)
{

if(((intal1[0] == '0') && (intal1[1] == '\0')) || ((intal2[0] == '0') && (intal2[1] == '\0')))
{
char *result = (char*)calloc(2, sizeof(char));

result[0] = '0';
result[1] = '\0';

return result;
}




int len1 = 0, len2 = 0, i = 0;

while(intal1[i] != '\0')	//finding the lengths of the 2 strings
{
len1 = len1 + 1;
i = i + 1;
}

i = 0;

while(intal2[i] != '\0')
{
len2 = len2 + 1;
i = i + 1;
}
i = 0;	// just a precaution for future use.

// Beware the array size is one less than the length, for example, if the length is 4 the array would span from 0 to 3.

// len doesn't include '\0'


//------ end of calculation of the lengths of the 2 intals


int j, carry = 0, k, temp3 = 0;


char **to_be_summed_matrix = (char **)malloc(len2 * sizeof(char *)); 
    for (i=0; i<len2; i++) 
         to_be_summed_matrix[i] = (char *)malloc((len1 + 1 + len2) * sizeof(char)); 

int array_size_counter = 0;

for(i=(len2 - 1); i>=0; i--)
{

int temp_result[len1 + 1 + array_size_counter]; // +1 for the carry, i for the 0s
// this array will be deleted as soon as the for block ends

// 0s are to be added in the end

int count = len1, temp2; // to store elements from units position

carry = 0;

for(j=(len1 - 1); j>=0; j--)
{

temp_result[count] = ((intal2[i] - '0') * (intal1[j] - '0')) + carry;

if(j != 0)
{
carry = temp_result[count] / 10;

temp_result[count] = temp_result[count] % 10;

count = count - 1;
} // end of if
else if(j == 0)
{

temp2 = temp_result[count];

temp_result[count] = temp2 % 10; // count will always be 1 here

count = count - 1;

temp_result[count] = temp2 / 10;

} // end of else



} // end of for

temp3 = len1 + 1;
for(k=temp3; k<(temp3 + array_size_counter); k++)
temp_result[k] = 0;


for(k=0; k<(len1 + 1 + array_size_counter); k++)
to_be_summed_matrix[array_size_counter][k] = temp_result[k] + '0';


to_be_summed_matrix[array_size_counter][len1 + 1 + array_size_counter] = '\0';


array_size_counter++;
} // end of for 



array_size_counter--; // compensating for the adddition in the last iteration. 



// one preceding 0 shouldn't matter for addition

char *result2 = (char *)malloc((len1 + 1 + len2 + 1) * sizeof(char)); // 1 carry bit and one '\0'

result2[0] = '0';
result2[1] = '\0';

char *g;

for(i=0; i<len2; i++)
{
g = result2;
result2 = intal_add(result2, to_be_summed_matrix[i]);
free(g);
}

for (i=0; i<len2; i++) 
free(to_be_summed_matrix[i]); 

free(to_be_summed_matrix);

return result2;


} // end of function














// Returns intal1 mod intal2
// The mod value should be in the range [0, intal2 - 1].
// intal2 > 1
// Implement a O(log intal1) time taking algorithm.
// O(intal1 / intal2) time taking algorithm may exceed time limit.
// O(intal1 / intal2) algorithm may repeatedly subtract intal2 from intal1.
// That will take intal1/intal2 iterations.
// You need to design a O(log intal1) time taking algorithm.
// Generate your own testcases at https://www.omnicalculator.com/math/modulo
char* intal_mod(const char* intal1, const char* intal2)
{
int compare_variable, temp, temp2, i;


compare_variable = intal_compare(intal1, intal2);
if(compare_variable == -1) {
char* answer = (char*)malloc((strlen(intal1) + 1) * sizeof(char));
strcpy(answer, intal1);
return answer;
} else if(compare_variable == 0) {
char* answer = (char*)malloc((2) * sizeof(char));
strcpy(answer, "0");
return answer;
}
temp = strlen(intal2);
temp2 = strlen(intal1);
char* string = (char*)malloc((strlen(intal2) + 2) * sizeof(char));
for(i = 0; i < temp; ++i)
string[i] = intal1[i];
string[temp] = '\0';


do 
{
if(intal_compare(string, intal2) == -1 && temp < temp2) 
{
int length = strlen(string);
if(strcmp(string, "0") == 0)
length = 0;
string[length] = intal1[temp];
string[length + 1] = '\0';
++temp;
}
if(intal_compare(string, intal2) != -1){
char* answer = intal_diff(string, intal2);
while(intal_compare(answer, intal2) > -1) {
char* tmp = answer;
answer = intal_diff(answer, intal2);
free(tmp);
}
  
strcpy(string, answer);
free(answer);
}    
} while(temp < temp2);

string = realloc(string, (strlen(string) + 1) * sizeof(char));
string[strlen(string) + 1] = '\0';
return string;
} // end of function






static char* recursive_pow(char *intal, unsigned int y)
{

char *temp;

if(y == 0)
{
char *result = (char*)calloc(1010, sizeof(char));
result[0] = '1';
result[1] = '\0';
return result;
}

temp = recursive_pow(intal, y/2);

if(y%2 == 0)
return intal_multiply(temp, temp);
else
return intal_multiply((intal_multiply(intal, temp)), temp);

} 







// Returns intal1 ^ intal2.
// Let 0 ^ n = 0, where n is an intal.
// Implement a O(log n) intal multiplications algorithm.
// 2^3000 has less than 1000 decimal digits. 3000 intal multiplications may exceed time limit.
char* intal_pow(const char* intal1, unsigned int n)
{

char *result = (char *)calloc(5, sizeof(char));


if((intal1[0] == '0') && (intal1[1] == '\0')) // 0^0 = 0
{
result[0] = '0';
result[1] = '\0';
return result;
}

if(n == 0)
{
result[0] = '1';
result[1] = '\0';
return result;
}

char *intal1_copy = (char *)calloc(1020, sizeof(char));

strcpy(intal1_copy, intal1);


return recursive_pow(intal1_copy, n);


} // end of function


















static char* gcd_recursive(char *intal1, char* intal2, char* zero)
{

if(intal_compare(intal1, zero) == 0)
return intal2;

return gcd_recursive((intal_mod(intal2, intal1)), intal1, zero);

}













// Returns Greatest Common Devisor of intal1 and intal2.
// Let GCD be "0" if both intal1 and intal2 are "0" even though it is undefined, mathematically.
// Use Euclid's theorem to not exceed the time limit.
char* intal_gcd(const char* intal1, const char* intal2)
{
// we have to make use of intal_mod

if(((intal1[0] == '0') && (intal1[1] == '\0')) && ((intal2[0] == '0') && (intal2[1] == '\0')))
{
char *result = (char*)calloc(2, sizeof(char));

result[0] = '0';
result[1] = '\0';

return result;
}

char *intal1_copy = (char*)calloc(1020, sizeof(char));
strcpy(intal1_copy, intal1);

char *intal2_copy = (char*)calloc(1020, sizeof(char));
strcpy(intal2_copy, intal2);

char *zero = (char*)calloc(3, sizeof(char));
zero[0] = '0';
zero[1] = '\0';



return gcd_recursive(intal1_copy, intal2_copy, zero);


}










// Returns nth fibonacci number.
// intal_fibonacci(0) = intal "0".
// intal_fibonacci(1) = intal "1".
char* intal_fibonacci(unsigned int n)
{

char *result = (char *)calloc(n,sizeof(char));
if(n == 0)
{
result[0] = '0';
result[1] = '\0';
return result;
}


if(n == 1)
{
result[0] = '1';
result[1] = '\0';
return result;
}



char *t1 = (char *)calloc(n,sizeof(char));
char *t2 = (char *)calloc(n,sizeof(char));

t1[0] = '0';
t1[1] = '\0';

t2[0] = '1';
t2[1] = '\0';


int i;

for(i=2; i<=n; i++)
{
strcpy(result, intal_add(t2, t1));
strcpy(t1, t2);
strcpy(t2, result);
} // end of for


free(t1);
free(t2);
return result;


} // end of function









// Returns the factorial of n.
char* intal_factorial(unsigned int n)
{

char *result = (char *)calloc(3000, sizeof(char));


/*result[0] = '1';
result[1] = '\0';
*/  // same as the below statement strcpy..

strcpy(result, "1");

if((n == 1) || (n == 0))
{
return result;
}

int i;
char *c  = (char *)calloc(3000,sizeof(char));

char *t;

for(i=n; i>=2; i--)
{

sprintf(c, "%d", i); // converting integer 'i' to character 'i'
 
t = result;
result =  intal_multiply(result, c);
free(t);
}

free(c);

return result;


} // end of function



static int min(int a, int b) 
{ 
    return (a<b)? a: b; 
}




// Returns the Binomial Coefficient C(n,k).
// 0 <= k <= n
// C(n,k) < 10^1000 because the returning value is expected to be less than 10^1000.
// Use the Pascal's identity C(n,k) = C(n-1,k) + C(n-1,k-1)
// Make sure the intermediate intal values do not cross C(n.k).
// Use Dynamic Programming
char* intal_bincoeff(unsigned int n, unsigned int k)
{

if(k > n)
{
char *res = (char*)calloc(3, sizeof(char));
res[0] = '0';
res[1] = '\0';
return res;
}


if(k == 0)
{
char *res = (char*)calloc(3, sizeof(char));
res[0] = '1';
res[1] = '\0';
return res;
}


int i,j;

char *result = (char*)calloc(1020, sizeof(char));

char*** C = (char***)calloc((n+1), sizeof(char**));

if(C == NULL)
{
printf("Failed to allocate memory\n");
exit(0);
}

for(i=0; i<(n+1); i++)
C[i] = (char**)calloc((k+1), sizeof(char*));


for(i=0; i<=n; i++)
{
for(j=0; j<=min(i,k); j++)
{
C[i][j] = (char*)calloc(1020, sizeof(char)); //10^1000 has 1003 digits
}
}


for(i=0; i<=n; i++)
{
for(j=0; j<=min(i,k); j++)
{

if((j == 0) || (j == i))
{
C[i][j][0] = '1';
C[i][j][1] = '\0';
}
else
{
C[i][j] = intal_add(C[i-1][j-1], C[i-1][j]);
} // end of else

} // end of for 

} // end of for


strcpy(result, C[n][k]);

for(i=0; i<=n; i++)
{
for(j=0; j<=min(i,k); j++)
{
free(C[i][j]);
}
}


for(i=0; i<(n+1); i++)
free(C[i]);


free(C);


return result;


} // end of function






// Returns the offset of the largest intal in the array.
// Return the smallest offset if there are multiple occurrences.
// 1 <= n <= 1000

// offset is the index of the array
int intal_max(char **arr, int n)
{

// array will range from 0 to n-1.

char *max_element = arr[0]; // holds the number

int max_offset = 0; // holds the index

int i, comparison_result = -2;

char *temp_holder;

for(i=1; i<n; i++)
{

temp_holder = arr[i];

comparison_result = intal_compare(temp_holder, max_element); // returns +1 if the first intal is greater.

if(comparison_result == 1) // smallest offset is assigned in case of multiple occurences
{
max_element = temp_holder;
max_offset = i;
} // end of if

} // end of for

return max_offset;


} // end of function











// Returns the offset of the smallest intal in the array.
// Return the smallest offset if there are multiple occurrences.
// 1 <= n <= 1000
int intal_min(char **arr, int n)
{


// array will range from 0 to n-1.

char *min_element = arr[0]; // holds the number

int min_offset = 0; // holds the index

int i, comparison_result = -2;

char *temp_holder;

for(i=1; i<n; i++)
{

temp_holder = arr[i];

comparison_result = intal_compare(temp_holder, min_element); // returns +1 if the first intal is greater.

if(comparison_result == -1) // smallest offset is assigned in case of multiple occurences
{
min_element = temp_holder;
min_offset = i;
} // end of if

} // end of for

return min_offset;

} // end of function














// Returns the offset of the first occurrence of the key intal in the array.
// Returns -1 if the key is not found.
// 1 <= n <= 1000
int intal_search(char **arr, int n, const char* key)
{

int len_of_key = 0, i = 0;

while(key[i] != '\0')	//finding the lengths of the 2 strings
{
len_of_key++ ;
i = i + 1;
}
i = 0;

// len_of_key doesn't include '\0' and the digits range from 0 to len_of_key - 1.


char *copy_of_key = (char *)calloc((len_of_key+1), sizeof(char));

strcpy(copy_of_key, key);


for(i=0; i<n; i++)
{

if(intal_compare(arr[i], copy_of_key) == 0)
{
free(copy_of_key);
return i;
} // end of if

} // end of for

free(copy_of_key);
return -1;

} // end of function









// Returns the offset of the first occurrence of the key intal in the SORTED array.
// Returns -1 if the key is not found.
// The array is sorted in nondecreasing order.
// 1 <= n <= 1000
// The implementation should be a O(log n) algorithm.
int intal_binsearch(char **arr, int n, const char* key)
{

int len_of_key = 0, i = 0;

while(key[i] != '\0')	//finding the lengths of the 2 strings
{
len_of_key++ ;
i = i + 1;
}
i = 0;

// len_of_key doesn't include '\0' and the digits range from 0 to len_of_key - 1.
char *copy_of_key = (char *)calloc((len_of_key+1), sizeof(char));

strcpy(copy_of_key, key);

int low = 0, high = (n-1), mid, temp;

int min = -1;

while(low <= high)
{

mid = (low + high) / 2;

temp = intal_compare(copy_of_key, arr[mid]);

if(temp == 0)
min = mid;

if((temp == 0) || (temp == (-1)))
high = mid - 1;
else
low = mid + 1;


} // end of while


free(copy_of_key);

printf("My answer: %d\n",min);
return min;

} // end of function



// Merges two subarrays of arr[]. 
// First subarray is arr[l..m] 
// Second subarray is arr[m+1..r] 
static void merge(char **arr, int l, int m, int r) 
{ 
    int i, j, k; 
    int n1 = m - l + 1; 
    int n2 =  r - m; 
  
    /* create temp arrays */
char **L = (char **)calloc(n1, sizeof(char *));
char **R = (char **)calloc(n2, sizeof(char *)); 
  
    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++) 
        L[i] = arr[l + i]; 
    for (j = 0; j < n2; j++) 
        R[j] = arr[m + 1+ j];



  
    /* Merge the temp arrays back into arr[l..r]*/
    i = 0; // Initial index of first subarray 
    j = 0; // Initial index of second subarray 
    k = l; // Initial index of merged subarray 

int temp;

    while (i < n1 && j < n2) 
    { 
temp = intal_compare(L[i], R[j]);

        if ((temp == 0) || (temp == -1)) 
        { 
            arr[k] = L[i]; 
            i++; 
        } 
        else
        { 
            arr[k] = R[j]; 
            j++; 
        } 
        k++; 
    } 
  
    /* Copy the remaining elements of L[], if there 
       are any */
    while (i < n1) 
    { 
        arr[k] = L[i]; 
        i++; 
        k++; 
    } 
  
    /* Copy the remaining elements of R[], if there 
       are any */
    while (j < n2) 
    { 
        arr[k] = R[j]; 
        j++; 
        k++; 
    } 
} 



/* l is for left index and r is right index of the 
   sub-array of arr to be sorted */
static void mergeSort(char **arr, int l, int r) 
{ 
    if (l < r) 
    { 
        // Same as (l+r)/2, but avoids overflow for 
        // large l and h 
        int m = l+(r-l)/2; 
  
        // Sort first and second halves 
        mergeSort(arr, l, m); 
        mergeSort(arr, m+1, r); 
  
        merge(arr, l, m, r); 
    } 
} 





// Sorts the array of n intals.
// 1 <= n <= 1000
// The implementation should be a O(n log n) algorithm.
void intal_sort(char **arr, int n)
{

mergeSort(arr, 0, n - 1);

}









// Coin-Row Problem - Dynamic Programming Solution
// There is a row of n coins whose values are some positive integers C[0..n-1].
// The goal is to pick up the maximum amount of money subject to the constraint that
// no two coins adjacent in the initial row can be picked up.
// 1 <= n <= 1000
// The implementation should be O(n) time and O(1) extra space.
// Eg: Coins = [10, 2, 4, 6, 3, 9, 5] returns 25
char* coin_row_problem(char **arr, int n)
{


char **C = (char**)calloc((n+1), sizeof(char*));

int i;



for(i=0; i<n; i++)
C[i+1] = arr[i];


char **F = (char**)calloc((n+1), sizeof(char*));

for(i=0; i<(n+1); i++)
F[i] = (char*)calloc(1010, sizeof(char));


char *result = (char*)calloc(1020, sizeof(char));


F[0][0] = '0';
F[0][1] = '\0';


strcpy(F[1], C[1]);

char *sum;
int temp2;

for(i=2; i<=n; i++)
{
sum = intal_add(C[i], F[i - 2]); // this memory is to be FREED.

temp2 = intal_compare(sum, F[i - 1]);

if(temp2 == -1)
strcpy(F[i], F[i-1]);
else
strcpy(F[i], sum);

free(sum);
} // end of for

strcpy(result, F[n]);

for(i=0; i<(n+1); i++)
{
free(F[i]);
}

free(C);
free(F);


return result;


} // end of function

