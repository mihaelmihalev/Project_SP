#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <stddef.h>
#include <unistd.h>
#include <sys/mman.h>
#define DIED 1
#define BORN 0
#define MAX_LIMIT 50
#define BUFFER_SIZE 100000

void addPerson(void);
void getDeadPerson(void);
void printFamilyTree();
void writeFamilyTree();
struct Person replace(struct Person person);

struct DateOfBirthday
{
    int day;
    int month;
    int year;
};

struct DateOfDeath
{
    int day;
    int month;
    int year;
};

struct Person
{
    struct DateOfBirthday birthdayDate;
    struct DateOfDeath date;
    char name[MAX_LIMIT];
    char father[MAX_LIMIT];
    char mother[MAX_LIMIT];
};

int main()
{ 
    writeFamilyTree();
    pid_t proccess;
    int *shem = mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
    if ((proccess = fork()) == 0)
    {
        printf("Choose 1 for add person to the family and 2 for check the death person of family: ");
         
    } else {
    scanf("%d", shem);
    if (*shem == 2)
    {
        getDeadPerson();     
    }
    else if (*shem == 1)
    {     
        addPerson();
    }
    else
    {
        printf("Invalid option,pleace try again!\n");
        return 1;
    }
    }
}

void addPerson()
{
    struct Person human;
    struct Person person;
    FILE *outfile;
    int father = 0;
    int mother = 0;
    int counter = 0;
    outfile = fopen ("FamilyTree.txt", "a+");
    printf("\nEnter the name of the person: ");
    scanf(" %[^\n]", human.name);

    printf("\nEnter %s's date of birth (day month year): ", human.name);
    scanf(" %d %d %d", &human.birthdayDate.day, &human.birthdayDate.month, &human.birthdayDate.year);

    printf("\nWho is %s's father(Name:)? ", human.name);
    scanf(" %[^\n]", human.father);
    
    printf("\nWho is %s's mother(Name:)? ", human.name);
    scanf(" %[^\n]", human.mother);
    
    
       while(fread(&person, sizeof(person), 1, outfile)) {
       if (strcmp(human.father, person.name)== 0)
        { 
          printf("\nFound\n");
          father = 1;
        }
        if (strcmp(human.mother, person.name)== 0)
        { 
          printf("\nFound\n");
          mother = 1;
        }
       }
    
    if (father == 1 && mother == 1) {
       
               fwrite(&human, sizeof(struct Person), 1, outfile);
               if(fwrite != 0)
        printf("\nPerson added to the family.\n");
        fclose(outfile);
        printFamilyTree();

    } if ( mother == 0 || father==0 ) {
    printf("\nNo such person with this name\n");
    fclose(outfile);
   
    }
        
    
    fclose(outfile);
    printFamilyTree();
}

void getDeadPerson()
{
    char name[MAX_LIMIT];
    struct Person person1;
    char dead[] = "X";
    char oldWord[100];
    char buffer[BUFFER_SIZE];
    FILE *outfile;
    FILE* tmp;
    struct Person person2;
    int counter = 0;
    char *filename = "FamilyTree.txt";
    
    outfile = fopen (filename, "a+");
    tmp = fopen ("tmp.text", "a+");

    
    if (outfile == NULL || tmp == NULL)
    {
        printf("\nUnable to open file.\n");
        exit(EXIT_SUCCESS);
    }

    printf("\nEnter the name of the person: ");
    scanf(" %[^\n]", name);

    
     while(fread(&person1, sizeof(person1), 1, outfile)) {
     counter++;
       if (strcmp(name, person1.name)== 0)
        {
          printf("\nFound\n");
          person1 = replace(person1);
          fseek(outfile, counter*(sizeof person1), SEEK_SET);
          fwrite(&person1, sizeof(person1), 1, tmp);
        } else {
			fwrite(&person1, sizeof(struct Person), 1, tmp);
		}
        }
          fclose(outfile);
          fclose(tmp);
	      remove(filename);
	      rename("tmp.text", filename);
          printFamilyTree();
};

void printFamilyTree()
{
    FILE *infile;
    struct Person person;
     
    infile = fopen ("FamilyTree.txt", "r");
    if (infile == NULL)
    {
        fprintf(stderr, "\nError opening file\n");
        exit (1);
    }
     
    printf("\n\nHere is the family tree:\n\n");
    while(fread(&person, sizeof(person), 1, infile)) {
        printf("NAME: %s \n", person.name);
        printf("DATE OF BIRTH: %d %d %d \n",
               person.birthdayDate.day, person.birthdayDate.month,
               person.birthdayDate.year);
        printf("FATHER: %s \n", person.father);
        printf("MOTHER: %s \n\n", person.mother);
    }
 
 
    fclose (infile);
    exit(1);
}

struct Person replace(struct Person person)
{
    char dead[] = " X";
        strcat(person.name, dead), person.name;
        return person;
}

void writeFamilyTree()
{
    FILE *infile;
     
    infile = fopen ("FamilyTree.txt", "w");
    if (infile == NULL)
    {
        fprintf(stderr, "\nError opened file\n");
        exit (1);
    }
    
    struct Person person1 = {{19, 07, 1975}, {0, 0, 0}, "Cvetanka Stefanova", {0}, {0}};
    fwrite(&person1, sizeof(struct Person), 1, infile);
    
    struct Person person2 = {{02, 03, 1970}, {0, 0, 0}, "Stefan Stefanov", {0}, {0}};
    fwrite(&person2, sizeof(struct Person), 1, infile);
    
    struct Person person3 =  {{02, 03, 2000}, {0, 0, 0},
    "Mihael Mihalev", {"Stefan Stefanov"}, {"Cvetanka Stefanova"}};
    fwrite(&person3, sizeof(struct Person), 1, infile);
    
    struct Person person4 =  {{19, 03, 1948}, {0, 0, 0},  "Petur Petrov", {"Ivan Petrov"}, {"Ivanka Petrova"}};
    fwrite(&person4, sizeof(struct Person), 1, infile);
    
    
    struct Person person5 =  {{02, 03, 1950}, {0, 0, 0}, "Test Father", {0}, {0}};
    fwrite(&person5, sizeof(struct Person), 1, infile);
    
    struct Person person6 =  {{18, 11, 1953}, {0, 0, 0},  "Test Mother", {0}, {0}};
    fwrite(&person6, sizeof(struct Person), 1, infile);
    
    struct Person person7 =  {{19, 03, 1968}, {0, 0, 0}, "Iordan Petrov", {"Ivan Petrov"}, {"Ivanka Petrova"}};
    fwrite(&person7, sizeof(struct Person), 1, infile);

    
    
    if(fwrite != 0)
        printf("Successfully writted!\n");
    else
        printf("Error writing file!\n");
 
 
    fclose (infile);
    
}