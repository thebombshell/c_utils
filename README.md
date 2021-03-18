# C Utils
 A collection of C89 compliant utilities I've found myself using across multiple projects

## Abstract
 I have found myself writing a lot in C recently, across a number of hobby projects and I've been copying a few files around them, sick of looking for which was most updated and finding them developping in different directions, I've decided to just throw them in their own repository, and they'll be present as a dependancy in the codebases which use them

## Standard
 As of posting the standard is thin and conventions are wobbly between files, what follows is the ideal Standard, not necassarily the standard in use, in time I hope to adhere fully to these standards.
### Target
Target is platform agnostic all warnings, warnings as errors, C89, where platform agnostic is not a capability the platform targets are linux and win32.
### Naming Convention
Names are to be explicit in nature, such that their purpose can be read in as near to plain english as possible, names are underscore_case, global names are tagged by g_, argument names are tagged by t_, pointer *types* are tagged by p_, opaque pointer *types* are tagged by op_, function groups are tagged by group_.

```c
static int g_static_integer;

typedef struct
{
    char* member_string;
    long long int member_long;
} my_structure, *p_my_structure;

typedef void* op_my_structure;

void my_structure_init(op_my_structure* t_out_structure);

void my_structure_final(op_my_structure* t_out_structure);
```
### Typedef Convention
Typedefs are to be used to convey intention in ambiguous circumstances.

```c
typedef float milliseconds;
typedef float seconds;
typedef float minutes;

void set_time_to_wait(milliseconds t_time);

void set_time_to_alarm(seconds t_time);

void set_time_to_breakfast(minutes t_time);
```
### Error Handling Convention
Error handling should be left up to the application, as such failures if they are capable of occuring should be handled via a non-zero, success return, I do not intend to accomadate further elaboration via error codes, rather I intend to accomadate simple readibility.

```c
int thing_do();

int main(int t_argc, char** t_args)
{
    if (thing_do())
    {
        /* thing done */
    }
    else
    {
        /* thing not done */
    }
}
```
